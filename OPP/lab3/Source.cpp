#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <malloc.h>
#include "IOTools.h"

#define TOPOLOGY_SIZE 2
#define N 2500

int main(int argc, char* argv[]) 
{
    int size, rank;
    int i, j, k;

    double start_time, finish_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double* A = NULL;
    double* B = NULL;
    double* C = NULL;
    double* correctC = NULL;

    double* APiece = NULL;
    double* BPiece = NULL;
    double* CPiece = NULL;

    int cPieceSize[2];
    int coords[2];
    int currentGridRank;

    int P0 = sqrt(size);
    int P1 = size / P0;

    /* for MPI_Cart_create */
    int dims[TOPOLOGY_SIZE] = { P0, P1 };
    int periods[TOPOLOGY_SIZE] = { 0, 0 };
    int reorder = 0;
    MPI_Comm comm_cart;

    MPI_Cart_create(MPI_COMM_WORLD, TOPOLOGY_SIZE, dims, periods, reorder, &comm_cart);

    if (rank == 0) 
    {
        A = new double[N * N];
        B = new double[N * N];
        C = new double[N * N];
        correctC = new double[N * N];

        loadMatrixFromBinaryFile(argv[1], A, N);
        loadMatrixFromBinaryFile(argv[2], B, N);
        loadMatrixFromBinaryFile(argv[3], correctC, N);

        for (i = 0; i != N; ++i) 
        {
            for (j = 0; j != N; ++j)
            {
                C[N * i + j] = 0;
                double t = B[N * i + j];
                B[N * i + j] = B[N * j + i];
                B[N * j + i] = t;
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    cPieceSize[0] = N / dims[0];
    cPieceSize[1] = N / dims[1];

    APiece = new double[cPieceSize[0] * N];
    BPiece = new double[cPieceSize[1] * N];
    CPiece = new double[cPieceSize[0] * cPieceSize[1]];

    if (rank == 0)
    {
        start_time = MPI_Wtime();
    }

    MPI_Comm_rank(comm_cart, &currentGridRank);
    MPI_Cart_coords(comm_cart, currentGridRank, TOPOLOGY_SIZE, coords);

    MPI_Comm rowComm, colComm;

    MPI_Comm_split(comm_cart, coords[0], coords[1], &rowComm);
    MPI_Comm_split(comm_cart, coords[1], coords[0], &colComm);

    if (coords[1] == 0)
    {
        MPI_Scatter(A, cPieceSize[0] * N, MPI_DOUBLE, APiece, cPieceSize[0] * N, MPI_DOUBLE, 0, colComm);
    }
        
    MPI_Bcast(APiece, cPieceSize[0] * N, MPI_DOUBLE, 0, rowComm);

    if (coords[0] == 0)
    {
        MPI_Scatter(B, cPieceSize[1] * N, MPI_DOUBLE, BPiece, cPieceSize[1] * N, MPI_DOUBLE, 0, rowComm);
    }

    MPI_Bcast(BPiece, cPieceSize[1] * N, MPI_DOUBLE, 0, colComm);

    for (i = 0; i != cPieceSize[0]; i++)
    {
        for (j = 0; j != cPieceSize[1]; j++)
        {
            CPiece[cPieceSize[1] * i + j] = 0.0;

            for (k = 0; k != N; k++)
            {
                int pos1 = N * i + k;
                int pos2 = N * j + k;
                CPiece[cPieceSize[1] * i + j] += APiece[pos1] * BPiece[pos2];
            }
        }
    }

    /* for MPI_Gatherv */
    int* recvcounts = new int[size];
    int* displs = new int[size];

    MPI_Datatype vector, newVector;

    MPI_Type_vector(cPieceSize[0], cPieceSize[1], N, MPI_DOUBLE, &vector);
    MPI_Type_commit(&vector);

    MPI_Type_create_resized(vector, 0L, (long)cPieceSize[1] * sizeof(double), &newVector);
    MPI_Type_commit(&newVector);

    for (i = 0; i != dims[0]; i++) 
    {
        for (j = 0; j != dims[1]; j++)
        {
            recvcounts[i * dims[1] + j] = 1;
            displs[i * dims[1] + j] = i * dims[1] * cPieceSize[0] + j;
        }
    }

    MPI_Gatherv(CPiece, cPieceSize[0] * cPieceSize[1], MPI_DOUBLE, C, recvcounts, displs, newVector, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    delete[] APiece;
    delete[] BPiece;
    delete[] CPiece;

    if (rank == 0) 
    {
        finish_time = MPI_Wtime();

        printf("Time: %lf\n", finish_time - start_time);
        checkResults(correctC, C, N, 0.001);

        delete[] A;
        delete[] B;
        delete[] C;
        delete[] correctC;
    }

    MPI_Type_free(&vector);
    MPI_Type_free(&newVector);

    MPI_Comm_free(&colComm);
    MPI_Comm_free(&rowComm);
    MPI_Comm_free(&comm_cart);
    MPI_Finalize();

    delete[] recvcounts;
    delete[] displs;

    return 0;
}