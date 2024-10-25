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
       
        MPI_Send(message, 13, MPI_CHAR, 1, type, MPI_COMM_WORLD);
    
        MPI_Recv(message, 13, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status);
        cout << "Message from node" << rank << ":" << message << endl;


    }
    else{
        if (rank == 1){
            MPI_Recv(message, 13, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status);
            cout << "Message from node" << rank << ":" << message << endl;

            MPI_Send(message, 13, MPI_CHAR, 2, type, MPI_COMM_WORLD);
           
        }
        else{
            cout << rank << endl;
            for (int i = 2; i < size; i++){
            
                MPI_Recv(message, 13, MPI_CHAR, i - 1, type, MPI_COMM_WORLD, &status);
                cout << "Message from node" << rank << ":" << message << endl;

                MPI_Send(message, 13, MPI_CHAR, i + 1, type, MPI_COMM_WORLD);

                if (i == size - 1){
                    MPI_Send(message, 13, MPI_CHAR, 0, type, MPI_COMM_WORLD);
                }
            }
        }
    }

    MPI_Finalize();
    return 0;

}