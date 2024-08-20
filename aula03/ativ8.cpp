#include <iostream>
#include <vector>
using namespace std;

// declarando a função de print
void printMatrix(const vector<vector<int>>& mat);

int main() {

    int linha = 3;
    int coluna = 3;
    vector<vector<int>> matriz(linha, vector<int>(coluna));
    float soma_diag_principal = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++){
            cout << "Digite um número: ";
            cin >> matriz[i][j];
            if (i == j){
                soma_diag_principal = soma_diag_principal + matriz[i][j];
            }
        }
    }

    printMatrix(matriz);

    cout << "A soma da diagonal principal: " << soma_diag_principal << endl;

    return 0;
}

void printMatrix(const vector<vector<int>>& mat)
{
    cout << "\n A matriz : \n";
    for(int i = 0; i < mat.size(); i++)
    {
        for(int j = 0; j < mat[i].size(); j++)
            cout << mat[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}
