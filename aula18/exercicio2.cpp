#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    int num_interacoes = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        num_interacoes = 1000;  // Por exemplo, 1000 iterações
        std::cout << "Processo raiz configurou num_interacoes para: " << num_interacoes << std::endl;
    
    }


    MPI_Bcast(&num_interacoes, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    int resultado = 0;
    for (int i = 0; i < num_interacoes; i++) {
        resultado += rank + i; 
    }

    std::cout << "Processo " << rank << " obteve resultado: " << resultado << std::endl;


    MPI_Finalize();
    return 0;
}
