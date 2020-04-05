#include "IOTools.h"

int loadMatrixFromBinaryFile(string fileName, double** matrix)
{
    int matrixSize = 0;
    ifstream fin(fileName, ios::in | ios::binary | ios::ate);

    if (fin.is_open())
    {
        int size = fin.tellg(); // фактический размер данных в байтах
        matrixSize = sqrt(size / sizeof(float)); // размер матрицы N при условии, что она записана как float (из задания)

        char* memblock = new char[size]; // здесь будут лежать данные из бинарного файла, разбитые по байтам (char)
        fin.seekg(0, ios::beg);
        fin.read(memblock, size);
        fin.close();

        float* float_values = (float*)memblock; // разбиваем наш считанный кусок уже по блокам float
        
        int i, j;
        for (i = 0; i != matrixSize; ++i)
        {
            for (j = 0; j != matrixSize; ++j)
            {
                matrix[i][j] = float_values[i * matrixSize + j];
            }
        }
    }

    fin.close();
    return matrixSize;
}

int loadVectorFromBinaryFile(string fileName, double* vector)
{
    int vectorSize = 0;
    ifstream fin(fileName, ios::in | ios::binary | ios::ate);

    if (fin.is_open())
    {
        int size = fin.tellg(); // фактический размер данных в байтах
        vectorSize = size / sizeof(float); // размер вектора N при условии, что он записан как float (из задания)
        
        char* memblock = new char[size]; // здесь будут лежать данные из бинарного файла, разбитые по байтам (char)
        fin.seekg(0, ios::beg);
        fin.read(memblock, size);
        fin.close();

        float* float_values = (float*)memblock; // разбиваем наш считанный кусок уже по блокам float
        
        int i;
        for (i = 0; i != vectorSize; ++i)
        {
            vector[i] = float_values[i];
        }
    }

    fin.close();
    return vectorSize;
}

void loadMatrixFromFile(string fileName, double** matrix, int matrixSize)
{
    ifstream fin(fileName);

    if (fin.is_open())
    {
        for (int i = 0; i != matrixSize; ++i)
        {
            for (int j = 0; j != matrixSize; ++j)
            {
                fin >> matrix[i][j];
            }
        }
    }

    fin.close();
}

void loadVectorFromFile(string fileName, double* vector, int vectorSize)
{
    ifstream fin(fileName);

    if (fin.is_open())
    {
        for (int i = 0; i != vectorSize; ++i)
        {
            fin >> vector[i];
        }
    }

    fin.close();
}


void printMatrix(double** matrix, int matrixSize)
{
    for (int i = 0; i != matrixSize; ++i)
    {
        for (int j = 0; j != matrixSize; ++j)
        {
            cout << matrix[i][j] << " ";
        }

        cout << endl;
    }
}

void printVector(double* vector, int vectorSize)
{
    for (int i = 0; i != vectorSize; ++i)
    {
        cout << vector[i] << " ";
    }
    cout << endl;
}

void saveMatrixToFile(double** matrix, string fileName, int matrixSize)
{
    ofstream fout(fileName);

    if (fout.is_open())
    {
        for (int i = 0; i != matrixSize; ++i)
        {
            for (int j = 0; j != matrixSize; ++j)
            {
                fout << matrix[i][j] << " ";
            }
            fout << endl;
        }
    }

    fout.close();
}

void saveVectorToFile(double* vector, string fileName, int vectorSize)
{
    ofstream fout(fileName);

    if (fout.is_open())
    {
        for (int i = 0; i != vectorSize; ++i)
        {
            fout << vector[i] << " ";
        }
    }

    fout.close();
}

bool checkResults(double* rightX, double* X, int N, double epsilon)
{
    int i;
    for (i = 0; i != N; ++i)
    {
        if (fabs(fabs(rightX[i]) - fabs(X[i])) > epsilon)
        {
            cout << "Too big difference between calculated and correct value: rightX[" << i << "] = " << rightX[i] << " and X[" << i << "] = " << X[i] << endl;
            cout << "The modulus of difference X[i] and rightX[i] should be less than epsilon = " << epsilon << ". Your difference = " << fabs(X[i] - rightX[i]);
            return false;
        }
    }

    cout << "checkResult: OK" << endl;
    return true;
}