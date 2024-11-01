#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <vector>
#include <random>
#include <ctime>

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

    MPI_Bcast(&maximo, 1, MPI_INT, 0, MPI_COMM_WORLD);


    int chunk_size = ARRAY_SIZE / size;
    double recv_array[chunk_size];

    MPI_Scatter(array, chunk_size, MPI_INT, recv_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);


    for (int i = 0; i < chunk_size; i++) {
        recv_array[i] = static_cast<double>(recv_array[i]) / maximo;
    }

    std::vector<double> normalized_array;
    if (rank == 0) {
        normalized_array.resize(ARRAY_SIZE);
    }

    MPI_Gather(recv_array, chunk_size, MPI_DOUBLE, normalized_array.data(), chunk_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        std::cout << "Array normalizado: ";
        for (int i = 0; i < ARRAY_SIZE; i++) {
            std::cout << normalized_array[i] << " ";
        }
        std::cout << std::endl;
    }
        
    

    MPI_Finalize();
    return 0;
}
