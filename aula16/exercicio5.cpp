#include <iostream>
#include <cstring>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv){
    
    int rank, size, type = 99;
    int valor, val_recebe, soma;
    valor = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    valor = rank + 1;

    if (rank == 0){
        
       soma = valor;
        MPI_Send(&soma, 1, MPI_INT, rank + 1, type, MPI_COMM_WORLD);

    }
    else{
        //cout << rank << endl;
        MPI_Recv(&val_recebe, 1, MPI_INT, rank - 1, type, MPI_COMM_WORLD, &status);
        
        soma = val_recebe + valor;

        if (rank < size - 1){
            MPI_Send(&soma, 1, MPI_INT, rank + 1, type, MPI_COMM_WORLD);
        }
        else{
            cout << "soma total: " << soma << endl;
        }
    }

    MPI_Finalize();
    return 0;

}