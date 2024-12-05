#include <iostream>

#include <fstream>    // Inclui a biblioteca para manipulação de arquivos, usada para ler e escrever arquivos.
#include <vector>     // Inclui a biblioteca de vetores, usada para armazenar e manipular listas de números.
#include <sstream>    // Inclui a biblioteca para manipulação de strings como fluxos.
#include <string>     // Inclui a biblioteca para manipular strings.
#include <algorithm>  // Inclui a biblioteca para algoritmos de uso geral, como find, sort, etc.
#include <omp.h>
#include <mpi.h>

using namespace std;


void LerArquivo(const string& nomeArquivo, vector<char>& buffer, vector<int>& lengths, vector<int>& displs) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        exit(EXIT_FAILURE);
    }

    string linha;
    string sequence = "";
    int deslocamento = 0;

    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        if (linha[0] == '>') { 
            if (!sequence.empty()) {
                for (char c : sequence) buffer.push_back(c); 
                lengths.push_back(sequence.size());          
                displs.push_back(deslocamento);              
                deslocamento += sequence.size();             
                sequence.clear();
            }
        } else {
            sequence += linha;  
        }
    }

    if (!sequence.empty()) {
        for (char c : sequence) buffer.push_back(c); 
        lengths.push_back(sequence.size());
        displs.push_back(deslocamento);
    }

    arquivo.close();
}


void ContarBases(const vector<char>& buffer, int start, int length, vector<int>& contagemBases) {

    int localA = 0, localT = 0, localC = 0, localG = 0;

    #pragma omp parallel for reduction(+:localA, localT, localC, localG)
    for (int i = 0; i < length; ++i) {

        char base = buffer[start + i];
        if (base == 'A' || base == 'a'){
            localA++;
        }
        else if (base == 'T' || base == 't'){
            localT++;
        } 
        else if (base == 'C' || base == 'c'){
            localC++;
        }
        else if (base == 'G' || base == 'g'){
            localG++;
        }
    }

    contagemBases[0] = localA;
    contagemBases[1] = localT;
    contagemBases[2] = localC;
    contagemBases[3] = localG;
}





void ProcessarArquivo(int rank, int size, const string& nomeArquivo) {
    vector<char> buffer;        
    vector<int> lengths;       
    vector<int> displs;         
    vector<char> localBuffer;   

    vector<int> localCount(4, 0);
    vector<int> globalCount(4, 0);

    // Lendo o arquivo
    LerArquivo(nomeArquivo, buffer, lengths, displs);

    // Distribuindo os dados entre os processos
    int N = lengths.size();
    vector<int> sendCounts(size, 0); 

    for (int i = 0; i < size; ++i) {
        if (i == size - 1) {
            sendCounts[i] = buffer.size() - displs[i];
        } else {
            sendCounts[i] = displs[i + 1] - displs[i];
        }
    }

    // Inicializando o buffer local
    int localBufferSize = sendCounts[rank];
    localBuffer.resize(localBufferSize);

    // Distribuindo os dados entre os processos
    MPI_Scatterv(buffer.data(), sendCounts.data(), displs.data(), MPI_CHAR,
                 localBuffer.data(), localBufferSize, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Contando as bases
    ContarBases(localBuffer, 0, localBuffer.size(), localCount);

    // Reduzindo os resultados para o rank 0
    MPI_Reduce(localCount.data(), globalCount.data(), 4, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Imprimindo os resultados no rank 0
    if (rank == 0) {
        cout << "Contagem de bases no arquivo " << nomeArquivo << ":" << endl;
        cout << "A: " << globalCount[0] << endl;
        cout << "T: " << globalCount[1] << endl;
        cout << "C: " << globalCount[2] << endl;
        cout << "G: " << globalCount[3] << endl;
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Definindo a lista de arquivos a serem processados
    int numArquivos = 22;
    vector<string> arquivos(numArquivos);
    for (int i = 0; i < numArquivos; ++i) {
        arquivos[i] = "dados/chr" + to_string(i + 1) + ".subst.fa";
    }

    // Processando cada arquivo
    for (const string& arquivo : arquivos) {
        ProcessarArquivo(rank, size, arquivo);
    }

    MPI_Finalize();
    return 0;
}

