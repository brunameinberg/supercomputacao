#include <iostream>
using namespace std;

int main(){
    int num;

    cout << "Digite um número: ";
    cin >> num;

    if (num % 2 > 0){
        cout << "É impar!" << endl;
    }
    else{
        cout << "É par!" << endl;
    }
}