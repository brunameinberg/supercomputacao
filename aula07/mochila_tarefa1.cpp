#include <iostream>   // Inclui a biblioteca padrão de entrada e saída, usada para operações como std::cout.
#include <fstream>    // Inclui a biblioteca para manipulação de arquivos, usada para ler e escrever arquivos.
#include <vector>     // Inclui a biblioteca de vetores, usada para armazenar e manipular listas de números.
#include <sstream>    // Inclui a biblioteca para manipulação de strings como fluxos.
#include <string>     // Inclui a biblioteca para manipular strings.
#include <algorithm>  // Inclui a biblioteca para algoritmos de uso geral, como find, sort, etc.

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

void mochila_recursiva(int index, int capacidade_restante, int peso_atual, int valor_atual, int& melhor_peso, int& melhor_valor, const vector<int>& pesos, const vector<int>& valores) {
    
    //condição de parada da recursão
    if (index >= pesos.size() || capacidade_restante <= 0) {
        if (valor_atual > melhor_valor) {
            melhor_valor = valor_atual;
            melhor_peso = peso_atual;
        }
        return;
    }

    //não incluindo -> apenas chama a função novamente só aumentando o índice, mas sem alterar mais nada!!!!
    mochila_recursiva(index + 1, capacidade_restante, peso_atual, valor_atual, melhor_peso, melhor_valor, pesos, valores);

    //incluindo -> chama a novamente a função só que além de aumentar o índex, também ajusta os outros atributos!!
    if (pesos[index] <= capacidade_restante) {
        mochila_recursiva(index + 1, capacidade_restante - pesos[index], peso_atual + pesos[index], valor_atual + valores[index], melhor_peso, melhor_valor, pesos, valores);
    }
}

void embaralha_vetores(vector<int>& pesos, vector<int>& valores){

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1,6);
    int dice_roll = distribution(generator);
    auto dice = std::bind ( distribution, generator );
    int wisdom = dice()+dice()+dice();

    
}

vector<int> mochila_busca_exaustiva(const vector<int>& pesos, const vector<int>& valores, int capacidade) {
    int melhor_peso = 0;
    int melhor_valor = 0;
    mochila_recursiva(0, capacidade, 0, 0, melhor_peso, melhor_valor, pesos, valores);
    
    vector<int> resultado;
    resultado.push_back(melhor_peso);
    resultado.push_back(melhor_valor);
    return resultado;
}

int main() {

    // Variáveis
    string nomedoarquivo;
    vector<int> vecpesos;
    vector<int> vecvalores;
    int capacidade;

    string numarquivo;
    cout << "Qual entrada você quer? (coloque somente o número) \n";
    cout << "1- entrada1 \n2- entrada2 \n3- entrada3 \n4- entrada4 \n";
    cin >> numarquivo;

    if (numarquivo < "1" || numarquivo > "4") {
        cout << "Esse arquivo não está disponível!" << endl;
        return 0;
    }

    nomedoarquivo = "entrada" + numarquivo + ".txt";

    construa_vetores(nomedoarquivo, vecpesos, vecvalores, capacidade);

    vector<int> resultado;
    resultado = mochila_busca_exaustiva(vecpesos, vecvalores, capacidade);

    //mostrando o resultado
    cout << "melhor peso:";
    cout << resultado[0];
    cout << "melhor valor:";
    cout << resultado[1] << endl;

    return 0;
}
