#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <omp.h>
#include <mpi.h>
#include <numeric>  

using namespace std;

map<string, char> criarTabelaAminoacidos() {
    return {
        {"CCA", 'P'}, {"CCG", 'P'}, {"CCU", 'P'}, {"CCC", 'P'}, 
        {"UCU", 'S'}, {"UCA", 'S'}, {"UCG", 'S'}, {"UCC", 'S'}, 
        {"CAG", 'G'}, {"CAA", 'G'},
        {"ACA", 'T'}, {"ACC", 'T'}, {"ACU", 'T'}, {"ACG", 'T'}, 
        {"AUG", 'M'}, 
        {"UGA", 'C'}, 
        {"UGC", 'I'}, {"UGU", 'I'},
        {"GUG", 'V'}, {"GUA", 'V'}, {"GUC", 'V'}, {"GUU", 'V'}
    };
}

void TraduzirRNA(const vector<char>& rnaBuffer, vector<char>& proteinBuffer, int start, int length) {
    map<string, char> tabelaAminoacidos = criarTabelaAminoacidos();

    #pragma omp parallel for
    for (int i = start; i < start + length - 2; i += 3) {
        string codon = string(1, rnaBuffer[i]) + rnaBuffer[i+1] + rnaBuffer[i+2];

        if (tabelaAminoacidos.find(codon) != tabelaAminoacidos.end()) {
            #pragma omp critical
            proteinBuffer.push_back(tabelaAminoacidos[codon]); 
        }
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<char> rnaBuffer;
    vector<char> localBuffer;
    vector<char> proteinBuffer;

    vector<int> sendCounts(size, 0);
    vector<int> displs(size, 0);

    int totalSize = 0;

    if (rank == 0) {
        ifstream arquivo("dados/saidas/saida_rna_subst.fa");
        if (!arquivo.is_open()) {
            cerr << "Erro ao abrir o arquivo" << endl;
            MPI_Finalize();
            return 1;
        }

        string linha;
        while (getline(arquivo, linha)) {
            if (!linha.empty() && linha[0] != '>') { 
                rnaBuffer.insert(rnaBuffer.end(), linha.begin(), linha.end());
            }
        }
        totalSize = rnaBuffer.size();
        arquivo.close();
    }

    MPI_Bcast(&totalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

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

    localBuffer.resize(sendCounts[rank]);
    MPI_Scatterv(rnaBuffer.data(), sendCounts.data(), displs.data(), MPI_CHAR,
                 localBuffer.data(), sendCounts[rank], MPI_CHAR, 0, MPI_COMM_WORLD);

    TraduzirRNA(localBuffer, proteinBuffer, 0, localBuffer.size());

    vector<int> proteinSizes(size);
    int localProteinSize = proteinBuffer.size();
    MPI_Gather(&localProteinSize, 1, MPI_INT, proteinSizes.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    vector<int> proteinDispls(size, 0);
    if (rank == 0) {
        for (int i = 1; i < size; ++i) {
            proteinDispls[i] = proteinDispls[i - 1] + proteinSizes[i - 1];
        }
    }

    vector<char> finalProteinBuffer;
    if (rank == 0) {
        finalProteinBuffer.resize(accumulate(proteinSizes.begin(), proteinSizes.end(), 0));
    }

    MPI_Gatherv(proteinBuffer.data(), localProteinSize, MPI_CHAR,
                finalProteinBuffer.data(), proteinSizes.data(), proteinDispls.data(), MPI_CHAR, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        ofstream outputFile("dados/saidas/proteina_ex_4.fa");
        for (char c : finalProteinBuffer) {
            outputFile << c;
        }
        outputFile.close();
        cout << "Tradução concluída. Proteína salva em 'proteina_ex_4.fa'" << endl;
    }

    MPI_Finalize();
    return 0;
}
