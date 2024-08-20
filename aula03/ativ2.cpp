#include <iostream>
using namespace std;

int main(){
    float num1, num2;
    char op;
    float resultado;

    cout << "Digite o primeiro número: ";
    cin >> num1;
    cout << "Digite o segundo número: ";
    cin >> num2;
    cout << "Digite a operação (+, -, *, /): ";
    cin >> op;


    if (op == '+'){
        resultado = num1 + num2;
    }
    else if (op == '-'){
        resultado = num1 - num2;
    }
    else if (op == '*'){
        resultado = num1 * num2;
    }
    else if (op == '/'){
        if (num2 != 0){
            resultado = num1 / num2;
        }
        else{
            cout << "Erro: Divisão por zero!" << endl;
            return 1; // Retorna um código de erro
        }
    }
    cout << "Resultado: " << resultado << endl;
    return 0;

}