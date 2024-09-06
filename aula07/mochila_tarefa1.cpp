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

void embaralha_vetores(vector<int>& pesos, vector<int>& valores){

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, pesos.size() - 1);
    auto dice = std::bind ( distribution, generator );

    std::vector<int> indices(pesos.size());
    for (int i = 0; i < indices.size(); ++i) {
        indices[i] = i;
    }

    for (int i = indices.size() - 1; i > 0; --i) {
        int j = dice() % (i + 1);  
        std::swap(indices[i], indices[j]);
    }

    std::vector<int> pesos_embaralhados(pesos.size());
    std::vector<int> valores_embaralhados(valores.size());
    
     for (int i = 0; i < indices.size(); ++i) {
        pesos_embaralhados[i] = pesos[indices[i]];
        valores_embaralhados[i] = valores[indices[i]];
    }

    pesos = pesos_embaralhados;
    valores = valores_embaralhados;

    
}

vector<int> enche_mochila(const vector<int>& pesos, const vector<int>& valores, int capacidade) {

    vector<int> resultados;
    int peso_total = 0;
    int valor_total = 0;

    for (int i = 0; i < pesos.size(); i++){
        //cout << capacidade << "\n";
        if (capacidade > pesos[i]){
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

void real_main(vector<int>& vecpesos, vector<int>& vecvalores, int capacidade){

    embaralha_vetores(vecpesos, vecvalores);

    vector<int> resultado;
    resultado = enche_mochila(vecpesos, vecvalores, capacidade);

    
    cout << "melhor peso:";
    cout << resultado[0];
    cout << "melhor valor:";
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

    for (int i = 0; i <= 1000000; i++){
        real_main(vecpesos, vecvalores, capacidade);
    }

    auto end = chrono::high_resolution_clock::now(); 

    chrono::duration<double> duration = end - start;
    cout << "Tempo de execução: " << duration.count() << " segundos" << endl;

    return 0;
}
