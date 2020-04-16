#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <sstream>
#include <math.h>
#include <mpi.h>
#include "IOTools.h"

int main(int argc, char** argv) 
{
    int N;
    double epsilon = 0.001;
    std::istringstream issN(argv[4], std::istringstream::in);
    issN >> N;
    
    MPI_Init(&argc, &argv);

    int size, rank;
    int i, j;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double startTime, endTime; // time moments for timer
    int firstMatrixStringPosition = 0;
    
    int* offsets = new int[size];

    double modBPart = 0;
    double tmp = 0;

    int* stringsForOneProcess = new int[size];
    int limit = size - (N % size);
    int offsetPosition = 0;

    for (i = 0; i < size; i++) // separation positions from 0 to N for all processes
    {
        offsets[i] = offsetPosition;
        if (i >= limit) 
        {
            stringsForOneProcess[i] = N / size + 1;
        }
        else 
        {
            stringsForOneProcess[i] = N / size;
        }

        if (i < rank) 
        {
            firstMatrixStringPosition += stringsForOneProcess[i];
        }
        stringsForOneProcess[i] *= N;
        offsetPosition += stringsForOneProcess[i];
    }
    int currLinesNum = stringsForOneProcess[rank] / N;

    double* A = new double[N * N]; // allocating input matrix, input vectors and vectors for algorithm
    double* pieceA = new double[currLinesNum * N];

    double* B = new double[N];
    double* pieceB = new double[currLinesNum];

    double* X = new double[N];
    double* pieceX = new double[currLinesNum];
    
    double* Z = new double[N];
    double* pieceZ = new double[currLinesNum];
    
    double* vecRPart = new double[currLinesNum];
    
    double* currentVector = new double[N];

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) // reading input matrix A and vector B
    {
        loadMatrixFromBinaryFile(argv[1], A, N);
        loadVectorFromBinaryFile(argv[2], B, N);
    }

    MPI_Bcast(B, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // calculating ranges of matrix and vectors for all process
    int a = currLinesNum * N; 
    MPI_Scatterv(A, stringsForOneProcess, offsets, MPI_DOUBLE, pieceA, a, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    for (i = 0; i < size; i++) 
    {
        stringsForOneProcess[i] = stringsForOneProcess[i] / N;
        offsets[i] = offsets[i] / N;
    }

    // calculating normaB 
    MPI_Barrier(MPI_COMM_WORLD);
    for (i = 0; i < currLinesNum; i++) 
    {
        pieceB[i] = B[firstMatrixStringPosition + i];
        pieceX[i] = 0.01;
        modBPart += pow(pieceB[i], 2);
        vecRPart[i] = pieceB[i];
    }
    double normaB;
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Allreduce(&modBPart, &normaB, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    delete[] B;
    delete[] pieceB;
    MPI_Barrier(MPI_COMM_WORLD);
    normaB = sqrt(normaB);

    // calculation start data for algorithm
    double step = 0.01;
    for (i = 0; i < currLinesNum; i++) 
    {
        tmp = 0;
        for (j = 0; j < N; j++) 
        {
            tmp += pieceA[i * N + j] * step;
            X[j] = step;

        }

        vecRPart[i] -= tmp;
        pieceZ[i] = vecRPart[i];
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Allgatherv(pieceZ, currLinesNum, MPI_DOUBLE, Z, stringsForOneProcess, offsets, MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) 
        startTime = MPI_Wtime();

    //information for algorithm
    double normaR;

    double alpha;
    double pieceAlpha;
    double beta;
    double pieceBeta;
    double RN;
    double pieceRN;
    
    do //main cycle: conjugateGradientMethod
    {
        alpha = 0;
        pieceAlpha = 0;
        beta = 0;
        pieceBeta = 0;
        RN = 0;
        pieceRN = 0;

        for (i = 0; i < currLinesNum; i++) 
        {
            tmp = 0;
            for (j = 0; j < N; j++) 
            {
                tmp += pieceA[(i * N) + j] * Z[j];
            }
            currentVector[i] = tmp;
            pieceRN += pow(vecRPart[i], 2);
            pieceAlpha += pieceZ[i] * tmp;
        }

        MPI_Allreduce(&pieceAlpha, &alpha, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(&pieceRN, &RN, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);

        alpha = RN / alpha;

        MPI_Barrier(MPI_COMM_WORLD);
        for (i = 0; i < currLinesNum; i++) 
        {
            pieceX[i] += pieceZ[i] * alpha;
            vecRPart[i] -= currentVector[i] * alpha;

            pieceBeta += pow(vecRPart[i], 2) / RN;
        }

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allreduce(&pieceBeta, &beta, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);

        normaR = sqrt(beta * RN);

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allgatherv(pieceX, currLinesNum, MPI_DOUBLE, X, stringsForOneProcess, offsets, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);

        for (i = 0; i < currLinesNum; i++) 
        {
            pieceZ[i] *= beta;
            pieceZ[i] += vecRPart[i];
        }

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allgatherv(pieceZ, currLinesNum, MPI_DOUBLE, Z, stringsForOneProcess, offsets, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);

    } while (normaR / normaB > epsilon);

    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) // compare our result (vector X) and correct result (correctX)
    {
        endTime = MPI_Wtime();

        double* correctX = new double[N];
        loadVectorFromBinaryFile(argv[3], correctX, N);
        std::cout << "Time: " << endTime - startTime << std::endl;
        checkResults(correctX, X, N, 0.01);
        delete[] correctX;
    }

    //printVector(X, 2500); // our result

    delete[] pieceA;
    delete[] pieceX;
    delete[] pieceZ;
    delete[] vecRPart;
    delete[] currentVector;
    delete[] X;
    delete[] Z;
    delete[] offsets;
    delete[] stringsForOneProcess;
    delete[] A;

    MPI_Finalize();
    return 0;
}