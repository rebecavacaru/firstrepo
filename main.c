#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
	int world_size, world_rank;
	int partialSum = 0;
	long long finalSum = 0;
	int nrElements, nElementsPerProcess;
	double totalTime, startCalculTime, endCalculTime;
	int* elements = NULL;
	int* receivedElements;


	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if (world_rank == 0) {
		FILE* f = fopen("C:\\Users\\rebeca.vacaru\\Documents\\suma_op_col\\elem.txt", "r");
		fscanf(f, "%d", &nrElements);

		elements = (int*)malloc(nrElements * sizeof(int));
        for (int i = 0; i < nrElements; i++) {
			fscanf(f, "%d", elements + i);
		}
		fclose(f);
		nElementsPerProcess= nrElements/world_size;
		}

    MPI_Bcast(&nElementsPerProcess, 1, MPI_INT, 0, MPI_COMM_WORLD);
    receivedElements = (int*)malloc(nElementsPerProcess * sizeof(int));
    totalTime = MPI_Wtime();
	MPI_Scatter(elements, nElementsPerProcess, MPI_INT, receivedElements, nElementsPerProcess,  MPI_INT, 0, MPI_COMM_WORLD);

	startCalculTime = MPI_Wtime();
	for (int i = 0; i < nElementsPerProcess; i++) {
		partialSum += receivedElements[i];
	}
	endCalculTime = MPI_Wtime();

	MPI_Reduce(&partialSum, &finalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    totalTime = MPI_Wtime() - totalTime;
	if (world_rank == 0)
	{
		for (int i = nrElements - (nrElements % world_size); i < nrElements; i++) {
			finalSum += elements[i];
		}

        printf("Final sum is: ");
        printf("%d",finalSum);
        printf("\nComputed in: ");
        printf("%.16f", endCalculTime-startCalculTime);
        printf("\nComunication time: ");
        printf("%.16f", totalTime- ( endCalculTime-startCalculTime ));
        printf("\nTotal time: ");
        printf("%.16f", totalTime);

	}
	MPI_Finalize();
}
