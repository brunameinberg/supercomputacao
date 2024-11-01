#include <iostream>
#include <cstring>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv){
    char message[20];
    int i, rank, size, type = 99;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //cout << size << endl;

    if (rank == 0){
        strcpy(message, "oiii");
       
        MPI_Send(message, strlen(message), MPI_CHAR, 1, type, MPI_COMM_WORLD);
    
        MPI_Recv(message, strlen(message), MPI_CHAR, size-1, type, MPI_COMM_WORLD, &status);
        cout << "Message from node" << size-1 << ":" << message << endl;


    }
    else{
        
        
            MPI_Recv(message, strlen(message), MPI_CHAR, rank - 1, type, MPI_COMM_WORLD, &status);
            cout << "Message from node" << rank - 1 << ":" << message << endl;

            if (rank != size - 1){
                MPI_Send(message, strlen(message), MPI_CHAR, rank + 1, type, MPI_COMM_WORLD);
            }

            if (rank == size - 1){
                MPI_Send(message, strlen(message), MPI_CHAR, 0, type, MPI_COMM_WORLD);
            }

    
    }

    MPI_Finalize();
    return 0;

}