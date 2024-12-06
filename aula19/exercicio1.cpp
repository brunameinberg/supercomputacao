#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <chrono>

int main(int argc, char *argv[]) {

    auto start = std::chrono::high_resolution_clock::now();

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

   
    const int N = 1000;  // Dimensões do array bidimensional
    int data[N][N];

    //std::cout<< "N: "<< N << std::endl;

    // Inicialização do array pelo processo 0
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                data[i][j] = i + j;
            }
        }
    }

    // Dividir o array entre os processos
    int chunk_size = N / size;
    int local_data[chunk_size][N];
    MPI_Scatter(data, chunk_size * N, MPI_INT, local_data, chunk_size * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Paralelização com OpenMP

    //std::cout<< "Com paralelização" << std::endl;
    //#pragma omp parallel for collapse(2)
    for (int i = 0; i < chunk_size; i++) {
        for (int j = 0; j < N; j++) {
            local_data[i][j] *= local_data[i][j];  // Calcula o quadrado do elemento
        }
    }

    // Reunir os resultados no processo 0
    MPI_Gather(local_data, chunk_size * N, MPI_INT, data, chunk_size * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Processo 0 imprime os resultados
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                std::cout << data[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    auto end =  std::chrono::high_resolution_clock::now(); 

    std::chrono::duration<double> duration = end - start;
    std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;


    MPI_Finalize();
    return 0;
}