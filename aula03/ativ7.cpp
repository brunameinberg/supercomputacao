#include <iostream>
#include <vector>
using namespace std;

// Função para ordenar o vetor usando o algoritmo Bubble Sort
void bubbleSort(vector<int>& vec) { // passando o objeto por referencia
    bool trocou;
    int num = vec.size();
    for (int i = 0; i < num; i++) {
        trocou = false;
        for (int j = 0; j < num - i - 1; j++) {
            if (vec[j] > vec[j + 1]) {
                swap(vec[j], vec[j + 1]);
                trocou = true;
            }
        }
        if (!trocou) {
            break;
        }
    }
}

// Função para imprimir o vetor
void printArray(const vector<int>& vec) {
    for (int num : vec) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    vector<int> vec; // Vetor dinâmico
    int tamanho;
    cout << "Qual o tamanho do seu vetor? ";
    cin >> tamanho;

    // Recebe os números e adiciona ao vetor
    for (int i = 0; i < tamanho; i++) {
        int numero;
        cout << "Digite um número: ";
        cin >> numero;
        vec.push_back(numero);
    }

    // Ordena o vetor
    bubbleSort(vec);

    // Imprime o vetor ordenado
    cout << "Vetor ordenado: \n";
    printArray(vec);

    return 0;
}
