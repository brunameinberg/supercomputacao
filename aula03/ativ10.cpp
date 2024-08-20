#include <iostream>
#include <vector>
using namespace std;

void visualizar_itens(const vector<string>& vec);
void adiciona_item(vector<string>& vec); 
void remove_item(vector<string>& vec);  

int main() {
    vector<string> itens;
    int opcao;

    cout << "O que você deseja fazer? \n";
    cout << "1- Visualizar os itens \n";
    cout << "2- Adicionar um item \n";
    cout << "3- Remover algum item \n";
    cin >> opcao;

    if (opcao == 1) {
        visualizar_itens(itens);
    } else if (opcao == 2) {
        adiciona_item(itens);
    } else if (opcao == 3) {
        remove_item(itens);
    } else {
        cout << "Essa opção não existe!" << endl;
    }

    return 0;
}

void visualizar_itens(const vector<string>& vec) {
    int tamanho = vec.size();
    for (int i = 0; i < tamanho; i++) {
        cout << i << ": " << vec[i] << endl;
    }
    cout << endl;
}

void adiciona_item(vector<string>& vec) { // Remover const
    string item;
    cout << "Qual item você deseja adicionar? ";
    cin.ignore(); // Ignorar o newline do cin anterior
    getline(cin, item); // Usar getline para permitir espaços no item
    vec.push_back(item);
}

void remove_item(vector<string>& vec) { // Remover const
    int indice;
    cout << "Qual índice do item você deseja remover? \n";
    visualizar_itens(vec);
    cin >> indice;

    if (indice >= 0 && indice < vec.size()) {
        vec.erase(vec.begin() + indice);
    } else {
        cout << "Índice inválido!" << endl;
    }
}
