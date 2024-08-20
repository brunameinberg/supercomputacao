#include <iostream>
#include <vector>
using namespace std;

int main(){
    vector<int> vetor;
    int maior = -2147483648;

    for (int i = 0; i < 10; i++){
        int numero;
        cout << "Digite um número: ";
        cin >> numero;
        vetor.push_back(numero);
        if (numero > maior){
            maior = numero;
        }
    }

    cout << "maior número: " << maior << endl;

    return 0;
}