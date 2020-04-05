#include "MatrixOperation.h"

void conjugateGradientMethod(double** A, double* B, double* X, double* R, double* Z, double* currentVector, int N)
{
    double dev = 0;
    int i, j;

#pragma omp parallel for reduction(+:dev)
    for (i = 0; i < N; ++i)
    {
        dev += R[i] * R[i];
    }

#pragma omp parallel for
    for (i = 0; i < N; ++i)
    {
#pragma omp parallel for
        for (j = 0; j < N; ++j)
        {
            currentVector[i] += A[i][j] * Z[j];
        }
    }
    double divider = 0;
#pragma omp parallel for reduction(+:divider)
    for (i = 0; i < N; ++i)
    {
        divider += currentVector[i] * Z[i];
    }
    double alphaN1 = dev / divider;
    fillVector(0, currentVector, N);


#pragma omp parallel for
    for (i = 0; i < N; ++i)
    {
        X[i] += (Z[i] * alphaN1);
    }

    double divForBeta = 0;
#pragma omp parallel for reduction(+:divForBeta)

    for (i = 0; i < N; ++i)
    {
        divForBeta += R[i] * R[i];
    }

#pragma omp parallel for
    for (i = 0; i < N; ++i)
    {
#pragma omp parallel for
        for (j = 0; j < N; ++j)
        {
            currentVector[i] += A[i][j] * Z[j];
        }
    }
#pragma omp parallel for
    for (i = 0; i < N; ++i)
    {
        R[i] -= (alphaN1 * currentVector[i]);
    }

    fillVector(0, currentVector, N);

    double bettaN1 = 0;
#pragma omp parallel for reduction(+:bettaN1)

    for (i = 0; i < N; ++i)
    {
        bettaN1 += R[i] * R[i];
    }

    bettaN1 /= divForBeta;

#pragma omp parallel for
    for (i = 0; i < N; ++i)
    {
        Z[i] = bettaN1 * Z[i] + R[i];
    }
}

double calcVectorNorm(double* vector, int N) 
{
    double res = 0;
    int i;
#pragma omp parallel for reduction(+:res)
    for (i = 0; i < N; ++i)
    {
        res += (vector[i] * vector[i]);
    }
    return sqrt(res);
}

void generateDefaultData(double** A, double* B, double* x, int N)
{
    int i, j;
    for (i = 0; i != N; ++i)
    {
        for (j = 0; j != N; ++j)
        {
            A[i][j] = (i == j) ? 2 : 1;
        }
    }

    for (i = 0; i != N; ++i)
    {
        B[i] = N + 1;
    }

    for (i = 0; i != N; ++i)
    {
        x[i] = 1;
    }
}

void copyVectors(double* src, double* dst, int N)
{
    int i;
#pragma omp parallel for
    for (i = 0; i < N; ++i)
    {
        src[i] = dst[i];
    }
}

void fillMatrix(double** matrix, int N)
{
    int i, j;
    for (i = 0; i != N; ++i)
    {
        for (j = 0; j != N; ++j)
        {
            matrix[i][j] = 0;
        }
    }
}

void fillVector(double value, double* vector, int N)
{
    int i;
    for (i = 0; i != N; ++i)
    {
        vector[i] = value;
    }
}