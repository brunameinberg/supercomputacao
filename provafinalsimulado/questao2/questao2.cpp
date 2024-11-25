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
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int ARRAY_SIZE = 100000;
    int array[ARRAY_SIZE];

    int chunk_size = ARRAY_SIZE / size;
    int recv_array[chunk_size];

    if (rank == 0){
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            array[i] = i;  
        }

    }
    MPI_Scatter(array, chunk_size, MPI_INT, recv_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    int soma_local;

    #pragma omp parallel for reduction(+:soma_local)
    for (int i = 0; i < chunk_size; ++i) {
        soma_local += recv_array[i] * recv_array[i];
    }

    int soma_global = 0;

    MPI_Reduce(&soma_local, &soma_global, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Soma total dos quadrados: " << soma_global << std::endl;
    }


    MPI_Finalize();
    return 0;
}
