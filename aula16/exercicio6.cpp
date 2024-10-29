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
        strcpy(message, "oiii do 0");

        for (int i = 1; i < size; i++){
       
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, i, type, MPI_COMM_WORLD);
            MPI_Recv(message, 20, MPI_CHAR, i, type, MPI_COMM_WORLD, &status);
            cout << "Message from node" << i << ":" << message << endl;

        }

    }
    else{
        
        MPI_Recv(message, 20, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status);
        cout << "Message from node" << 0 << ":" << message << endl;
        strcpy(message, "oiii");
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, type, MPI_COMM_WORLD);
    
    }

    MPI_Finalize();
    return 0;

}