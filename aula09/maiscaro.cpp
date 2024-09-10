#include <iostream>   // Inclui a biblioteca padrão de entrada e saída, usada para operações como std::cout.
#include <fstream>    // Inclui a biblioteca para manipulação de arquivos, usada para ler e escrever arquivos.
#include <vector>     // Inclui a biblioteca de vetores, usada para armazenar e manipular listas de números.
#include <sstream>    // Inclui a biblioteca para manipulação de strings como fluxos.
#include <string>     // Inclui a biblioteca para manipular strings.
#include <algorithm>  // Inclui a biblioteca para algoritmos de uso geral, como find, sort, etc.
#include <random>
#include <functional> // Para std::bind
#include <chrono>

using namespace std; 

struct Item {
    int peso;
    int valor;
};

void construa_vetores(const string& nome_arquivo, vector<Item>& itens, int& capacidade) {
    ifstream entrada(nome_arquivo);

    string linha;
    int N;
    //peganbdo somente a primeira linha
    if (getline(entrada, linha)) {
        istringstream iss(linha);
        iss >> N >> capacidade;
    }
    
    itens.resize(N);
    
    for (int i = 0; i < N; ++i) {
        if (getline(entrada, linha)) {
            istringstream iss(linha);
            int peso, valor;
            if (iss >> peso >> valor) {
                itens[i] = {peso, valor};
            }
        }
    }
    entrada.close(); 
}

bool compara_valor(const Item& item1, const Item& item2) {
    return item1.valor > item2.valor; // Ordena por valor decrescente
}

vector<int> enche_mochila(const vector<Item>& itens, int capacidade) {

    vector<int> resultados;
    int peso_total = 0;
    int valor_total = 0;

    for (const auto& item : itens){
        //cout << capacidade << "\n";
        if (capacidade >= item.peso){
            //cout << "entrei aqui \n";
            peso_total = peso_total + item.peso;
            valor_total = valor_total + item.valor;
            capacidade = capacidade - item.peso;
        }
    }

    //cout << "peso total: " << peso_total << "\n";
    //cout << "valor total: " << valor_total << "\n";

    resultados.push_back(peso_total);
    resultados.push_back(valor_total);

    return resultados;

    
}



int main() {

    // Variáveis
    string nomedoarquivo;
    vector<Item> itens;
    int capacidade;

    string numarquivo = "";
    cout << "Qual entrada você quer? (coloque somente o número) \n";
    cout << "1- entrada1 \n2- entrada2 \n3- entrada3 \n4- entrada4 \n";
    cin >> numarquivo;

    if (numarquivo < "1" || numarquivo > "4") {
        cout << "Esse arquivo não está disponível!" << endl;
        return 0;
    }

    auto start = chrono::high_resolution_clock::now();


    nomedoarquivo = "entrada" + numarquivo + ".txt";

    

    construa_vetores(nomedoarquivo, itens, capacidade);

    //função para ordenar
    sort(itens.begin(), itens.end(), compara_valor);

    vector<int> resultado;
    resultado = enche_mochila(itens, capacidade);

    cout << "Melhor peso: " << resultado[0] << "\n";
    cout << "Melhor valor: " << resultado[1] << endl;

    auto end = chrono::high_resolution_clock::now(); 

    chrono::duration<double> duration = end - start;
    cout << "Tempo de execução: " << duration.count() << " segundos" << endl;

    return 0;
}
