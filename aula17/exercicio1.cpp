#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    int media = 0;
    int i;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int ARRAY_SIZE = 100;
    int array[ARRAY_SIZE];

    if (rank == 0) {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = i;
        }
    }

    int chunk_size = ARRAY_SIZE / size;
    int recv_array[chunk_size];

    MPI_Scatter(array, chunk_size, MPI_INT, recv_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0 ){
        MPI_Gather(recv_array, chunk_size, MPI_INT, recv_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    // Imprime o vetor recebido
        
        std::cout << "Rank " << rank << " recebeu: ";
        for (i = 0; i < chunk_size; i++) {
            std::cout << recv_array[i] << " ";
            media = media + recv_array[i];
        }
        std::cout << std::endl;
        std::cout << "i:" << i << std::endl;
        int media_final = media / i;
        std::cout << "media: " << media_final << std::endl;
        
    }
    

    MPI_Finalize();
    return 0;
}
