#include <iostream>   
#include <fstream>    
#include <vector>     
#include <sstream>    
#include <string>     
#include <algorithm>  
#include <random>
#include <functional> 
#include <chrono>

using namespace std; 

void construa_vetores(const string& nome_arquivo, vector<int>& pesos, vector<int>& valores, int& capacidade) {
    ifstream entrada(nome_arquivo);

    string linha;
    int N;
    if (getline(entrada, linha)) {
        istringstream iss(linha);
        iss >> N >> capacidade;
    }
    
    pesos.resize(N);
    valores.resize(N);
    
    for (int i = 0; i < N; ++i) {
        if (getline(entrada, linha)) {
            istringstream iss(linha);
            int peso, valor;
            if (iss >> peso >> valor) {
                pesos[i] = peso;
                valores[i] = valor;
            }
        }
    }
    entrada.close(); 
}


int calcula_valor(const vector<int>& pesos, const vector<int>& valores, const string& solucao, int capacidade) {
    int valor_total = 0;
    int peso_total = 0;
    for (int i = 0; i < solucao.size(); ++i) {
        if (solucao[i] == '1') {
            peso_total += pesos[i];
            if (peso_total > capacidade) {
                return 0; // Mochila estoura capacidade, solução inválida
            }
            valor_total += valores[i];
        }
    }
    return valor_total;
}

vector<string> gera_vizinhos(const string& solucao) {
    vector<string> vizinhos;
    for (int i = 0; i < solucao.size(); ++i) {
        string novo_vizinho = solucao;
        novo_vizinho[i] = (solucao[i] == '0') ? '1' : '0'; // Alterna o bit
        vizinhos.push_back(novo_vizinho);
    }
    return vizinhos;
}


string hill_climbing(const vector<int>& pesos, const vector<int>& valores, int capacidade) {
    // Gerar uma solução inicial aleatória respeitando a capacidade
    string solucao(pesos.size(), '0');
    random_device rd;
    mt19937 gen(rd());

    int peso_atual = 0;
    for (int i = 0; i < solucao.size(); ++i) {
        if (gen() % 2 == 0 && peso_atual + pesos[i] <= capacidade) {
            solucao[i] = '1';
            peso_atual += pesos[i];
        }
    }

    int melhor_valor = calcula_valor(pesos, valores, solucao, capacidade);
    bool encontrou_melhor = true;

    while (encontrou_melhor) {
        encontrou_melhor = false;
        vector<string> vizinhos = gera_vizinhos(solucao);
        
        for (const auto& vizinho : vizinhos) {
            int valor_vizinho = calcula_valor(pesos, valores, vizinho, capacidade);
            if (valor_vizinho > melhor_valor) {
                melhor_valor = valor_vizinho;
                solucao = vizinho;
                encontrou_melhor = true;
                break; // Recomeça o processo com a nova solução
            }
        }
    }

    return solucao;
}


void real_main(vector<int>& vecpesos, vector<int>& vecvalores, int capacidade) {
    string melhor_solucao = hill_climbing(vecpesos, vecvalores, capacidade);
    int melhor_valor = calcula_valor(vecpesos, vecvalores, melhor_solucao, capacidade);
    
    cout << "Melhor valor após Hill Climbing: " << melhor_valor << endl;
    cout << "Solução: " << melhor_solucao << endl;
}

int main() {

    auto start = chrono::high_resolution_clock::now();

    // Variáveis
    string nomedoarquivo;
    vector<int> vecpesos;
    vector<int> vecvalores;
    int capacidade;

    string numarquivo = "3";
    //cout << "Qual entrada você quer? (coloque somente o número) \n";
    //cout << "1- entrada1 \n2- entrada2 \n3- entrada3 \n4- entrada4 \n";
    //cin >> numarquivo;

    if (numarquivo < "1" || numarquivo > "4") {
        cout << "Esse arquivo não está disponível!" << endl;
        return 0;
    }

    nomedoarquivo = "entrada" + numarquivo + ".txt";

    construa_vetores(nomedoarquivo, vecpesos, vecvalores, capacidade);

    for (int i = 0; i <= 9; i++){
        real_main(vecpesos, vecvalores, capacidade);
    }

    auto end = chrono::high_resolution_clock::now(); 

    chrono::duration<double> duration = end - start;
    cout << "Tempo de execução: " << duration.count() << " segundos" << endl;

    return 0;
}