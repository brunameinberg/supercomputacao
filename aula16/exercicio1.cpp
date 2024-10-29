// Comunicação entre dois processos

#include <iostream>
#include <cstring>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv){
    char message[5];
    int rank, size, type = 99;
    char message2[3];

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0){
        strcpy(message, "Olá");
        MPI_Send(message, strlen(message), MPI_CHAR, 1, type, MPI_COMM_WORLD);

        MPI_Recv(message2, strlen(message2), MPI_CHAR, 1, type, MPI_COMM_WORLD, &status);
        cout << "Message from node " << rank << ": " << message2 << endl;
    }
    else if (rank == 1) {
        MPI_Recv(message, strlen(message), MPI_CHAR, 0, type, MPI_COMM_WORLD, &status);
        cout << "Message from node " << rank << ": " << message << endl;
        
        strcpy(message2, "Oi");
        MPI_Send(message2, strlen(message2), MPI_CHAR, 0, type, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;

}