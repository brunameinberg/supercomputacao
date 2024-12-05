#include <iostream>

#include <fstream>    // Inclui a biblioteca para manipulação de arquivos, usada para ler e escrever arquivos.
#include <vector>     // Inclui a biblioteca de vetores, usada para armazenar e manipular listas de números.
#include <sstream>    // Inclui a biblioteca para manipulação de strings como fluxos.
#include <string>     // Inclui a biblioteca para manipular strings.
#include <algorithm>  // Inclui a biblioteca para algoritmos de uso geral, como find, sort, etc.
#include <omp.h>
#include <mpi.h>

using namespace std;


void LerArquivoRNA(const string& nomeArquivo, vector<char>& buffer, vector<int>& lengths, vector<int>& displs) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        exit(EXIT_FAILURE);
    }

    string linha;
    int deslocamento = 0;

    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        for (char c : linha) buffer.push_back(c);
        lengths.push_back(linha.size());
        displs.push_back(deslocamento);
        deslocamento += linha.size();
    }

    arquivo.close();
}

int ContarAUG(const vector<char>& buffer, int start, int length) {
    int count = 0;

    #pragma omp parallel for reduction(+:count)
    for (int i = start; i < start + length - 2; ++i) {
        if (buffer[i] == 'A' && buffer[i + 1] == 'U' && buffer[i + 2] == 'G') {
            count++;
        }
    }

    return count;
}



int main(int argc, char* argv[]){

    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<char> buffer;        
    vector<int> lengths;       
    vector<int> displs;         
    vector<char> localBuffer;   


    vector<int> sendCounts(size, 0);

    int localCount = 0;
    int globalCount = 0;

    if (rank == 0){
        string nomeArquivo = "dados/saidas/saida_rna_subst.fa";
        LerArquivoRNA(nomeArquivo, buffer, lengths, displs);

        for (int i = 0; i < size; ++i) {
            if (i == size - 1) {
                sendCounts[i] = buffer.size() - displs[i];
            } else {
                sendCounts[i] = displs[i + 1] - displs[i];
            }
        }
    }

    MPI_Bcast(sendCounts.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
    
    int localBufferSize = sendCounts[rank];
    localBuffer.resize(localBufferSize);

    MPI_Scatterv(buffer.data(), sendCounts.data(), displs.data(), MPI_CHAR,
                 localBuffer.data(), localBufferSize, MPI_CHAR, 0, MPI_COMM_WORLD);

    localCount = ContarAUG(localBuffer, 0, localBuffer.size());

    MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Número total de proteínas inicializadas (AUG): " << globalCount << endl;
    }

    MPI_Finalize();
    return 0;


}

