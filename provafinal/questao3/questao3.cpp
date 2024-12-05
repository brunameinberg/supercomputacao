#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <omp.h>
#include <vector>

using namespace std;

double calcula_media(vector<double>& vector){

    double soma = 0.0;
    double media = 0.0;

    #pragma omp parallel for reduction(+:soma)
    for (int i = 0; i < vector.size(); i++) {
       soma = soma + vector[i];
    }

    media = soma/vector.size();

    return media;
}

double calcula_maior_valor(vector<double>& vector){

    double maior_valor = -9999999.0;

    #pragma omp parallel for reduction(max:maior_valor)
    for (int i = 0; i < vector.size(); i++) {
       if (vector[i] > maior_valor){
        maior_valor = vector[i];
       }
    }

    return maior_valor;

}

double calcula_produto(vector<double>& vector){

    double produto = 1.0;

    #pragma omp parallel for reduction(*:produto)
    for (int i = 0; i < vector.size(); i++) {
       produto *= vector[i];
    }

    return produto;

}

int main(){
    vector<double> vector = {1.0, 2.0, 3.0};

    double media = calcula_media(vector);
    double maior_valor = calcula_maior_valor(vector);
    double produto = calcula_produto(vector);

    cout << "Média: " << media << endl;
    cout << "Maior valor: " << maior_valor << endl;
    cout << "Produto: " << produto << endl;

    return 0;
}

