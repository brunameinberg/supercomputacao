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
    double media;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 100;
    int array[N];


    if (rank == 0) {

        for (int i = 0; i < N; i++) {
            array[i] = i;
            std::cout << " " << i;

        }
        std::cout << " "<< std:: endl;

    }

    int chunk_size = N / size;
    int recv_array[chunk_size];

    MPI_Scatter(array, chunk_size, MPI_INT, recv_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    double soma_local = 0.0;
    
    #pragma omp parallel for reduction(+:soma_local)
    for (int i = 0; i < chunk_size; i++) {
       soma_local = soma_local + recv_array[i];
    }



    double somas[size];
   
    MPI_Gather(&soma_local, 1, MPI_DOUBLE, somas, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    
    if (rank == 0) {

        double soma_total = 0.0;

        for (int i = 0; i < size; i++) {
            soma_total += somas[i];
        }

        double media = soma_total / N;
        std::cout << "Média: " << media << std::endl;

    }

        

    MPI_Finalize();
    return 0;
}
