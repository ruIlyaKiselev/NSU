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

bool checkResults(double* rightM, double* M, int N, double epsilon)
{
    int i, j;
    for (i = 0; i != N; ++i)
    {
        for (j = 0; j != N; ++j)
        {
            if (fabs(fabs(rightM[i * N + j]) - fabs(M[i * N + j]) > epsilon))
            {
                std::cout << "Too big difference between calculated and correct value: rightX[" << i << "] = " << rightM[i * N + j] << " and X[" << i << "] = " << M[i * N + j] << std::endl;
                std::cout << "The modulus of difference X[i] and rightX[i] should be less than epsilon = " << epsilon << ". Your difference = " << fabs(M[i * N + j] - rightM[i * N + j]);
                return false;
            }
        }
    }

    std::cout << "checkResult: OK" << std::endl;
    return true;
}