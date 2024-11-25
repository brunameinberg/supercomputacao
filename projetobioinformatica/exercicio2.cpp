#include <iostream>
#include <fstream>    // Inclui a biblioteca para manipulação de arquivos, usada para ler e escrever arquivos.
#include <vector>     // Inclui a biblioteca de vetores, usada para armazenar e manipular listas de números.
#include <sstream>    // Inclui a biblioteca para manipulação de strings como fluxos.
#include <string>     // Inclui a biblioteca para manipular strings.
#include <algorithm>  // Inclui a biblioteca para algoritmos de uso geral, como find, sort, etc.
#include <numeric>
#include <omp.h>
#include <mpi.h>

using namespace std;


void LerArquivo(const string& nomeArquivo, vector<char>& buffer) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        exit(EXIT_FAILURE);
    }

    string linha;

    while (getline(arquivo, linha)) {
        if (!linha.empty() && linha[0] != '>') { 
            buffer.insert(buffer.end(), linha.begin(), linha.end());
        }
    }


    arquivo.close();

    cout << "Arquivo lido: primeiras 5 letras do buffer: ";
    for (size_t i = 0; i < min(buffer.size(), size_t(5)); ++i) {
        cout << buffer[i];
    }
    cout << endl;
    cout << "\nTamanho total do buffer: " << buffer.size() << endl;
}


void Transcricao(const vector<char>& inputBuffer, vector<char>& outputBuffer) {
    #pragma omp parallel
    {
        vector<char> localBuffer;
        #pragma omp for nowait
        for (size_t i = 0; i < inputBuffer.size(); ++i) {
            char base = inputBuffer[i];
            if (base == 'N' || base == 'n') continue;

            char transcribedBase;
            if (base == 'A' || base == 'a') {
                transcribedBase = 'U';
            } else if (base == 'T' || base == 't') {
                transcribedBase = 'A';
            } else if (base == 'C' || base == 'c') {
                transcribedBase = 'G';
            } else if (base == 'G' || base == 'g') {
                transcribedBase = 'C';
            } else {
                continue;
            }

            localBuffer.push_back(transcribedBase);
        }

        #pragma omp critical
        outputBuffer.insert(outputBuffer.end(), localBuffer.begin(), localBuffer.end());
    }

    cout << "Transcrição concluída: primeiras 5 letras do buffer transcrito: ";
    for (size_t i = 0; i < min(outputBuffer.size(), size_t(5)); ++i) {
        cout << outputBuffer[i];
    }
    cout << endl;
}

void SalvarRNA(const string& nomeArquivo, const vector<char>& buffer) {
    ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao salvar o arquivo: " << nomeArquivo << endl;
        exit(EXIT_FAILURE);
    }

    for (char c : buffer) {
        arquivo << c;
    }

    arquivo.close();

    cout << "RNA salvo: primeiras 5 letras do buffer: ";
    for (size_t i = 0; i < min(buffer.size(), size_t(5)); ++i) {
        cout << buffer[i];
    }
    cout << endl;
}



int main(int argc, char* argv[]){

    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<char> buffer;  
    vector<char> localBuffer;      
    vector<char> transcribedBuffer;        
       
    int totalSize = 0;
    vector<int> sendCounts(size, 0);
    vector<int> displs(size, 0);

    if (rank == 0){
        string nomeArquivo = "dados/chr1.subst.fa";
        LerArquivo(nomeArquivo, buffer);

        totalSize = buffer.size();
        int baseSize = totalSize / size;
        int remainder = totalSize % size;

        int currentDisplacement = 0;

        for (int i = 0; i < size; ++i) {
            if (i < remainder) {
                sendCounts[i] = baseSize + 1;  
            } else {
                sendCounts[i] = baseSize;     
            }
            displs[i] = currentDisplacement;

            currentDisplacement += sendCounts[i];
        }

        cout << "Configuração inicial no processo 0:" << endl;
        cout << "sendCounts: ";
        for (int count : sendCounts) cout << count << " ";
        cout << endl;

        cout << "displs: ";
        for (int disp : displs) cout << disp << " ";
        cout << endl;
    }

    MPI_Bcast(&totalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(sendCounts.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(displs.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
    
    int localBufferSize = sendCounts[rank];
    localBuffer.resize(localBufferSize);

    MPI_Scatterv(buffer.data(), sendCounts.data(), displs.data(), MPI_CHAR,
                 localBuffer.data(), localBufferSize, MPI_CHAR, 0, MPI_COMM_WORLD);

    cout << "Processo " << rank << ": primeiras 5 letras do buffer local recebido: ";
    for (size_t i = 0; i < min(localBuffer.size(), size_t(5)); ++i) {
        cout << localBuffer[i];
    }
    cout << endl;

    Transcricao(localBuffer, transcribedBuffer);

    vector<int> transcribedSizes(size);
    int localTranscribedSize = transcribedBuffer.size();

    MPI_Gather(&localTranscribedSize, 1, MPI_INT, transcribedSizes.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    vector<int> transcribedDispls(size, 0);
    if (rank == 0) {
        for (int i = 1; i < size; ++i) {
            transcribedDispls[i] = transcribedDispls[i - 1] + transcribedSizes[i - 1];
        }
    }

    vector<char> finalBuffer;
    if (rank == 0) {
        finalBuffer.resize(accumulate(transcribedSizes.begin(), transcribedSizes.end(), 0));
    }

    MPI_Gatherv(transcribedBuffer.data(), localTranscribedSize, MPI_CHAR,
                finalBuffer.data(), transcribedSizes.data(), transcribedDispls.data(), MPI_CHAR, 0, MPI_COMM_WORLD);


    if (rank == 0) {
        string nomeArquivoRNA = "dados/saidas/saida_rna_ex_2.fa";
        SalvarRNA(nomeArquivoRNA, finalBuffer);
        cout << "Transcrição completa. RNA salvo em: " << nomeArquivoRNA << endl;
    }

    MPI_Finalize();
    return 0;



}

