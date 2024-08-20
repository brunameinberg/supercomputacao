#include <iostream>
using namespace std;

bool eh_palindromo(string palavra);

int main(){
    string palavra;
    cout << "Digite uma palavra: ";
    cin >> palavra; 

    bool resultado = eh_palindromo(palavra);

    if (resultado == true){
        cout << "É palindromo! " << endl;
    }
    else{
        cout << "Não é palindromo! " << endl;
    }

    return 0;
}

bool eh_palindromo(string palavra){

    int tamanho = palavra.length();
    bool result = true;
    int j = tamanho - 1;

    for (int i = 0; i < tamanho; i++){
        if (palavra[i] != palavra[j]){
            result = false;
        }
        j = j - 1;
    }

    return result;
}

