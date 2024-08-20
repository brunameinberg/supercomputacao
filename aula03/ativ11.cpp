#include <iostream>   // Inclui a biblioteca padrão de entrada e saída, usada para operações como std::cout.
#include <fstream>    // Inclui a biblioteca para manipulação de arquivos, usada para ler e escrever arquivos.
#include <vector>     // Inclui a biblioteca de vetores, usada para armazenar e manipular listas de números.
#include <sstream>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

void calcular_estatisticas(const string& arquivo_entrada, const string& arquivo_saida) {
    ifstream entrada(arquivo_entrada);

    string linha;
    unordered_map<string, int> frequencias;
    int qtd_palavras = 0;
    int qtd_linhas = 0;

    while (getline(entrada, linha)) {
        qtd_linhas++;
        istringstream iss(linha);
        string palavra;
        while (iss >> palavra) {
            frequencias[palavra]++;
            qtd_palavras++;
        }
    }

    string palavra_mais_frequente;
    int max_frequencia = 0;
    for (const auto& item : frequencias) {
        if (item.second > max_frequencia) {
            max_frequencia = item.second;
            palavra_mais_frequente = item.first;
        }
    }

    double media_palavras_por_linha = qtd_linhas > 0 ? static_cast<double>(qtd_palavras) / qtd_linhas : 0;

    ofstream saida(arquivo_saida);

    saida << "Número total de palavras: " << qtd_palavras << endl;
    saida << "Número total de linhas: " << qtd_linhas << endl;
    saida << "Número médio de palavras por linha: " << media_palavras_por_linha << endl;
    saida << "Palavra mais frequente: " << palavra_mais_frequente << " (Frequência: " << max_frequencia << ")" << endl;

    entrada.close();
    saida.close();
}

int main() {
    string arquivo_entrada = "entrada.txt";
    string arquivo_saida = "saida.txt";

    calcular_estatisticas(arquivo_entrada, arquivo_saida);

    cout << "Resultados em:  " << arquivo_saida << endl;

    return 0;
}
