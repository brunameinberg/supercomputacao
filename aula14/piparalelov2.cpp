#include <iostream>
#include <random>  
#include <cmath>
#include <chrono>
#include <omp.h>

using namespace std;

int main() {
    
    auto start = chrono::high_resolution_clock::now();

    int n_pontos = 100000;  
    int pontosdentro = 0;

    #pragma omp parallel reduction(+:pontosdentro)
    {
        std::random_device rd;
        std::mt19937 generator(rd() ^ omp_get_thread_num()); 
        std::uniform_real_distribution<double> distribution(0.0, 1.0);

        #pragma omp for
        for (int i = 0; i < n_pontos; i++) {
            double x = distribution(generator);
            double y = distribution(generator);

            if (x * x + y * y <= 1.0) {
                pontosdentro++;
            }
        }
    }
    
    double pi_estimado = 4.0 * pontosdentro / n_pontos;

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;
    
    cout << "Valor estimado de Pi (com melhoria de paralelização): " << pi_estimado << endl;
    cout << "Tempo de execução (com melhoria de paralelização): " << duration.count() << " segundos" << endl;

    return 0;
}
