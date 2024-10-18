#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>

using namespace std; 



int main() { 

    auto start = chrono::high_resolution_clock::now();

    int n_pontos;
    n_pontos = 100000;  

    int pontosdentro; 
    pontosdentro= 0;
    
    srand(static_cast<unsigned int>(time(0)));
    
    for (int i = 0; i < n_pontos; i++) {

        double x = static_cast<double>(rand()) / RAND_MAX;
        double y = static_cast<double>(rand()) / RAND_MAX;
        
        if (x * x + y * y <= 1.0) {
            pontosdentro++;
        }
    }
    
    // Estima o valor de Pi
    double pi_estimado = 4.0 * pontosdentro / n_pontos;

    auto end = chrono::high_resolution_clock::now(); 

    chrono::duration<double> duration = end - start;
    
    // Exibe o resultado
    cout << "Valor estimado de Pi: " << pi_estimado << endl;
    
    cout << "Tempo de execução: " << duration.count() << " segundos" << endl;

    
    return 0;
}
