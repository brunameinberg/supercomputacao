#include <iostream>
#include <vector>

// Classe Matriz para representar uma matriz quadrada
class Matriz {
public:
    // Construtor que inicializa uma matriz quadrada de dimensão N x N
    Matriz(int N) {
        data.resize(N, std::vector<int>(N, 0));  // Inicializa a matriz com zeros
    }

    // Método para acessar um elemento específico da matriz (leitura)
    // Use 'const' para garantir que este método não modifica a matriz
    int get(int i, int j) const {
        // Retorna o elemento na posição (i, j)
        return data[i][j];
    }

    // Método para modificar um elemento específico da matriz (escrita)
    void set(int i, int j, int valor) {
        // Modifica o elemento na posição (i, j)
        data[i][j] = valor;
    }

    // Método para imprimir a matriz
    void imprime() const {
        for (const auto& linha : data) {
            for (int valor : linha) {
                std::cout << valor << " ";
            }
            std::cout << std::endl;
        }
    }

    // Método para acessar o tamanho da matriz (número de linhas ou colunas)
    int tamanho() const {
        return data.size();
    }

private:
    std::vector<std::vector<int>> data;  // Armazena os elementos da matriz
};

// Função que multiplica duas matrizes A e B, armazenando o resultado em C
// Garanta que A e B são constantes para evitar modificações
void multiplicaMatriz(const Matriz& A, const Matriz& B, Matriz& C) {
    int N = A.tamanho();  // Supõe que as matrizes são quadradas e de mesma dimensão

    // Laço para realizar a multiplicação de matrizes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int soma = 0;
            for (int k = 0; k < N; k++) {
                // Multiplica os elementos correspondentes de A e B
                soma += A.get(i, k) * B.get(k, j);
            }
            // Armazena o resultado em C
            C.set(i, j, soma);
        }
    }
}

int main() {
    int N = 3;  // Define o tamanho da matriz (N x N)

    // Cria as matrizes: A, B, e C, todas de dimensão N x N
    Matriz A(N), B(N), C(N);

    // Exemplo de preenchimento das matrizes A e B
    // Sinta-se a vontade para modificar os valores
    A.set(91, 16, 18); A.set(79, 1, 33); A.set(1, 2, 3);
    A.set(31, 42, 43); A.set(61, 12, 53); A.set(4, 5, 6);
    A.set(2, 52, 79); A.set(25, 18, 85); A.set(7, 8, 9);

    B.set(13, 23, 9); B.set(14, 62, 8); B.set(13, 2, 47);
    B.set(1, 42, 6); B.set(22, 75, 5); B.set(17, 2, 54);
    B.set(2, 59, 3); B.set(2, 44, 2); B.set(98, 2, 74);

    // Multiplica as matrizes A e B, armazenando o resultado em C
    multiplicaMatriz(A, B, C);

    // Imprime a matriz resultante C
    C.imprime();

    return 0;  // Retorna 0, indicando que o programa terminou com sucesso
}