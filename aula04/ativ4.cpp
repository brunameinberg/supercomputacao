#include <iostream>
#include <vector>

class Vector {
public:
    Vector(int tamanho);
    ~Vector();
    void inicializa(int valor);
    int get(int index) const;
    void set(int index, int valor);
    void inserir(int index, int valor);
    void remover(int index);
    void imprime() const;

private:
    int* dados;
    int tam;
    int capacidade;
    void redimensiona(int novaCapacidade);
};

// Implementação das funções...

int main() {
    Vector vec(5);
    vec.inicializa(0);
    vec.imprime();

    return 0;
}