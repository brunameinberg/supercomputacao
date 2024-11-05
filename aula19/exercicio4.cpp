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
    //double media;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 100;
    int array[N];
    int valor_buscado = 0;


    if (rank == 0) {

        for (int i = 0; i < N; i++) {
            array[i] = i;
            std::cout << " " << i;

        }
        std::cout << " "<< std:: endl;
        //std::cout << "qual valor, de 0 a 99, voce deseja buscar?" << std::endl;
        //std::cin >> valor_buscado;
        valor_buscado = 23;

    }

    MPI_Bcast(&valor_buscado, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int chunk_size = N / size;
    int recv_array[chunk_size];

    MPI_Scatter(array, chunk_size, MPI_INT, recv_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    
    std::vector<int> posicoes_locais;


    #pragma omp parallel for //distribui o loop for entre várias threads
    for (int i = 0; i < chunk_size; i++) {
       if (recv_array[i] == valor_buscado){
            #pragma omp critical // para proteger a linha abaixo
            posicoes_locais.push_back(i + rank * chunk_size);
       }
    }

    int local_count = posicoes_locais.size();
    int* all_counts = nullptr;
    int* displs = nullptr;

    if (rank == 0) {
        all_counts = new int[size];
        displs = new int[size];
    }

    
   
    MPI_Gather(&local_count, 1, MPI_INT, all_counts, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    
    int total_count = 0;
    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            displs[i] = total_count;
            total_count += all_counts[i];
        }
    }

    int* all_positions = nullptr;
    if (rank == 0) {
        all_positions = new int[total_count];
    }

    // O gather v nao espera que o dado tenha o mesmo tamanho
    MPI_Gatherv(posicoes_locais.data(), local_count, MPI_INT, all_positions, all_counts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "posicoes encontradas para o valor " << valor_buscado << ": ";
        for (int i = 0; i < total_count; i++) {
            std::cout << all_positions[i] << " ";
        }
        std::cout << std::endl;

        delete[] all_counts;
        delete[] displs;
        delete[] all_positions;
    }

        

    MPI_Finalize();
    return 0;
}
