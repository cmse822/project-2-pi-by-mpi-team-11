#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char** argv) 
{
    cout << "Hi!" << endl;
    MPI_Init(&argc, &argv);
    cout << "Hello, World!" << endl;
    MPI_Finalize();
    cout << "Hey!" << endl;
    return 0;
}
