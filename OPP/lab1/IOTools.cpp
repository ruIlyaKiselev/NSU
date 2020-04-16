#include "IOTools.h"

int loadMatrixFromBinaryFile(const char* fileName, double* matrix, const int matrixSize)
{
    int result = 0;
    float* tmp = new float[matrixSize * matrixSize];
    FILE* fileMatrix = fopen(fileName, "rb");
    
    if (fileMatrix != NULL)
        result = fread(tmp, sizeof(float), matrixSize * matrixSize, fileMatrix);
    else
        return -1;
    
    int i;
    for (i = 0; i != matrixSize * matrixSize; ++i)
    {
        matrix[i] = (double)tmp[i];
    }
    
    delete[] tmp;
    fclose(fileMatrix);
    return result;
}

int loadVectorFromBinaryFile(const char* fileName, double* vector, const int vectorSize)
{
    int result = 0;
    float* tmp = new float [vectorSize];
    FILE* fileVector = fopen(fileName, "rb");

    if (fileVector != NULL)
        result = fread(tmp, sizeof(float), vectorSize, fileVector);
    else
        return -1;

    int i;
    for (i = 0; i != vectorSize; ++i)
    {
        vector[i] = (double)tmp[i];
    }

    delete[] tmp;
    fclose(fileVector);
    return result;
}

void printMatrix(double* matrix, int matrixSize)
{
    for (int i = 0; i != matrixSize; ++i)
    {
        for (int j = 0; j != matrixSize; ++j)
        {
            std::cout << matrix[i * matrixSize + j] << " ";
        }

        std::cout << std::endl;
    }
}

void printVector(double* vector, int vectorSize)
{
    for (int i = 0; i != vectorSize; ++i)
    {
        std::cout << vector[i] << " ";
    }
    std::cout << std::endl;
}

bool checkResults(double* rightX, double* X, int N, double epsilon)
{
    int i;
    for (i = 0; i != N; ++i)
    {
        if (fabs(fabs(rightX[i]) - fabs(X[i])) > epsilon)
        {
            std::cout << "Too big difference between calculated and correct value: rightX[" << i << "] = " << rightX[i] << " and X[" << i << "] = " << X[i] << std::endl;
            std::cout << "The modulus of difference X[i] and rightX[i] should be less than epsilon = " << epsilon << ". Your difference = " << fabs(X[i] - rightX[i]);
            return false;
        }
    }

    std::cout << "checkResult: OK" << std::endl;
    return true;
}