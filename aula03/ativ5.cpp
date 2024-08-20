#include <iostream>
#include <vector>
using namespace std;

int main(){
    vector<int> vetor;
    int resultado = 0;

    for (int i = 0; i < 5; i++){
        int numero;
        cout << "Digite um número: ";
        cin >> numero;
        vetor.push_back(numero);
        resultado = resultado + numero;
    }

    cout << "resultado: " << resultado << endl;

    return 0;
}