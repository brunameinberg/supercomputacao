#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>

using namespace std;


int main(int argc, char *argv[]) {


    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 1000;  
    int data[N][N];

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                data[i][j] = i + j;
            }
        }

        cout << "Matriz preenchida" << endl;
    }
    
    

    int chunk_size = N / size;
    int local_data[chunk_size][N];

    MPI_Scatter(data, chunk_size * N, MPI_INT, local_data, chunk_size * N, MPI_INT, 0, MPI_COMM_WORLD);

    double medias_parciais_locais[chunk_size];

    //cout << "Começando tasks" << endl;

    #pragma omp parallel for
    for (int i = 0; i < chunk_size; i++) {

        int soma_local = 0.0;
        double media = 0.0;

        for (int j = 0; j < N; j++) {
            soma_local += local_data[i][j];  
        }

        #pragma omp critical
        media = (double)soma_local/N;
        medias_parciais_locais[i] = media;
    }

    double medias_finais[chunk_size*N];

    //cout << "Reunindo tasks" << endl;

    MPI_Gather(medias_parciais_locais, chunk_size, MPI_DOUBLE, medias_finais, chunk_size*4, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double media_total = 0;
    if (rank == 0) {
        //cout << "entrei aqui" << endl;
        for (int i = 0; i < chunk_size; i++) {
            media_total += medias_finais[i];
        }

         cout << "Média Final: " << media_total << endl;
    }

   

    MPI_Finalize();
    return 0;
}

