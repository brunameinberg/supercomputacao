#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    int i;
    int maximo = -9999999;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int ARRAY_SIZE = 100;
    int array[ARRAY_SIZE];


    if (rank == 0) {

        std::random_device rd;
        std::mt19937 generator(rd() ^ omp_get_thread_num()); 
        std::uniform_int_distribution<int> distribution(-9999998, 9999998);

        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = distribution(generator);
        }

        #pragma omp parallel for reduction(max:maximo)
        for (int i = 0; i < ARRAY_SIZE; i++) {
            if (array[i] > maximo) {
                maximo = array[i];
            }
        }

        std::cout << "maximo:  "<< maximo << std::endl;


    }

    int chunk_size = ARRAY_SIZE / size;
    int recv_array[chunk_size];

    MPI_Scatter(array, chunk_size, MPI_INT, recv_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    double soma = 0;
    double media = 0;

    for (int i = 0; i < chunk_size; i++) {
       soma = soma + recv_array[i];
    }


    media = soma / chunk_size;


    double soma_quadrados = 0.0;

    for (int i = 0; i < chunk_size; i++) {
        soma_quadrados += (recv_array[i]  - media) * (recv_array[i]  - media);
    }

    double variancia = 0;
    variancia = soma_quadrados / chunk_size;


    double medias[size];
    double variancias[size];

   
    MPI_Gather(&media, 1, MPI_DOUBLE, medias, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(&variancia, 1, MPI_DOUBLE, variancias, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    
    if (rank == 0) {

        double media_global = 0.0;

        for (int i = 0; i < size; i++) {
            media_global += medias[i];
        }

        media_global = media_global / size;

        double variancia_global = 0.0;

        for (int i = 0; i < size; i++) {
            variancia_global += variancias[i] + (medias[i] - media_global) * (medias[i] - media_global);
        }
        variancia_global = variancia_global / size;

        double desvio_padrao_global = std::sqrt(variancia_global);
        
        std::cout << "Desvio padrão global: " << desvio_padrao_global << std::endl;

    }
        

    MPI_Finalize();
    return 0;
}
