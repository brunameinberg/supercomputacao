#include <iostream>   // Inclui a biblioteca padrão de entrada e saída, usada para operações como std::cout.
#include <fstream>    // Inclui a biblioteca para manipulação de arquivos, usada para ler e escrever arquivos.
#include <vector>     // Inclui a biblioteca de vetores, usada para armazenar e manipular listas de números.
#include <sstream>    // Inclui a biblioteca para manipulação de strings como fluxos.
#include <string>     // Inclui a biblioteca para manipular strings.
#include <algorithm>  // Inclui a biblioteca para algoritmos de uso geral, como find, sort, etc.
#include <random>
#include <functional> // Para std::bind
#include <chrono> //para seed aleatória

using namespace std; 

void construa_vetores(const string& nome_arquivo, vector<int>& pesos, vector<int>& valores, int& capacidade) {
    ifstream entrada(nome_arquivo);

    string linha;
    int N;
    //peganbdo somente a primeira linha
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


vector<int> enche_mochila(const vector<int>& pesos, const vector<int>& valores, int capacidade, default_random_engine& generator) {

    vector<int> resultados;
    int peso_total = 0;
    int valor_total = 0;

    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    auto dice = std::bind ( distribution, generator );

    for (int i = 0; i < pesos.size(); i++){
        //cout << capacidade << "\n";

        // gerando um número de probabilidade aleatório
        double valor_aleatorio = distribution(generator);
        //cout << valor_aleatorio << "\n";

        if (capacidade > pesos[i] && valor_aleatorio > 0.5){
            //cout << "entrei aqui \n";
            peso_total = peso_total + pesos[i];
            valor_total = valor_total + valores[i];
            capacidade = capacidade - pesos[i];
        }
    }

    //cout << "peso total: " << peso_total << "\n";
    //cout << "valor total: " << valor_total << "\n";

    resultados.push_back(peso_total);
    resultados.push_back(valor_total);

    return resultados;

    
}

void real_main(vector<int>& vecpesos, vector<int>& vecvalores, int capacidade, default_random_engine& generator){


    vector<int> resultado;
    resultado = enche_mochila(vecpesos, vecvalores, capacidade, generator);

    cout << "melhor peso: ";
    cout << resultado[0];
    cout << "melhor valor: ";
    cout << resultado[1] << endl;

}

int main() {

    auto start = chrono::high_resolution_clock::now();

    // Variáveis
    string nomedoarquivo;
    vector<int> vecpesos;
    vector<int> vecvalores;
    int capacidade;

    string numarquivo = "1";
    //cout << "Qual entrada você quer? (coloque somente o número) \n";
    //cout << "1- entrada1 \n2- entrada2 \n3- entrada3 \n4- entrada4 \n";
    //cin >> numarquivo;

    if (numarquivo < "1" || numarquivo > "4") {
        cout << "Esse arquivo não está disponível!" << endl;
        return 0;
    }

    nomedoarquivo = "entrada" + numarquivo + ".txt";

    construa_vetores(nomedoarquivo, vecpesos, vecvalores, capacidade);

    //coloquei essa parte na main, porque não estava sendo aleatório entre as rodagens
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);

    for (int i = 0; i < 1000000; i++){
        real_main(vecpesos, vecvalores, capacidade, generator);
    }

    auto end = chrono::high_resolution_clock::now(); 

    chrono::duration<double> duration = end - start;
    cout << "Tempo de execução: " << duration.count() << " segundos" << endl;


    return 0;
}