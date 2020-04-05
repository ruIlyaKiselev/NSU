#include "IOTools.h"
#include "MatrixOperation.h"
#include <ctime>
#include <sstream>
#include <omp.h>

int main(int argc, char* argv[])
{
    string matrixA;
    string vectorB;
    string vectorX;
    int epsilon = 10e-6;
    int numberOfThreads = 0;
    int N = 0;
    
    if (argc == 6)
    {
        matrixA = argv[1];
        vectorB = argv[2];
        vectorX = argv[3];
        istringstream issNumberOfThreads(argv[4], istringstream::in);
        issNumberOfThreads >> numberOfThreads;
        istringstream issN(argv[5], istringstream::in);
        issN >> N;
    }

    if (numberOfThreads <= 0)
    {
        cout << "Bad input" << endl;
        return 1;
    }

    omp_set_num_threads(numberOfThreads);

    int i, j;
    
    /* Подготовка матрицы, векторов значений и векторов для алгоритма*/
    double** A = new double* [N];
    for (i = 0; i != N; ++i)
    {
        A[i] = new double[N];
    }
    double* B = new double[N];
    double* X = new double[N];
    double* R = new double[N];
    double* Z = new double[N];
    double* currentVector = new double[N];

    //generateDefaultData(A, B, X, N);

    int result1 = loadMatrixFromBinaryFile("matA.bin", A);
    int result2 = loadVectorFromBinaryFile("vecB.bin", B);

    if (result1 != result2 || result1 != N || result2 != N)
    {
        cout << "Bad matrix or vector file" << endl;
        return 1;
    }
    
    fillVector(1, X, N);
    
    /* Начало подготовки данных для алгоритма*/
    fillVector(0, currentVector, N);
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            currentVector[i] += A[i][j] * X[j];
        }
    }
    
    for (i = 0; i < N; i++)
    {
        R[i] = B[i] - currentVector[i];
    }

    fillVector(0, currentVector, N);
    copyVectors(Z, R, N);

    double currentNorm = 0;
    /* Конец подготовки данных для алгоритма*/
    
    /* Засекаем время старта алгоритма*/
    double start = omp_get_wtime();
    
    while ((currentNorm = calcVectorNorm(R, N) / calcVectorNorm(B, N)) > epsilon) // Основной цикл с условием выполнения пока соотношение норм R и B меньше epsilon
    {
        conjugateGradientMethod(A, B, X, R, Z, currentVector, N); // Функция метода сопряженных градиентов
    }
    
    /* Засекаем время конца алгоритма*/
    double finish = omp_get_wtime();

    cout << "Threads: " << numberOfThreads << ", time: " << (finish - start) << std::endl; // Выводится число потоков и время, за которое были произведены вычисления.

    double* rightX = new double[N]; // Вектор, в котором хранятся правильные из ответы (не результат работы программы, он нужен для проверки)
    loadVectorFromBinaryFile("vecX.bin", rightX);

    fillVector(0, currentVector, N);   // Сравнение результатов с данными из ответа. Если ответы правильные, но функция говорит, 
    checkResults(rightX, X, N, 10e-3); //что это не так, то нужно снизить точность (последний аргумент функции), то есть передать число большее по модулю
                 
    //printVector(X, N); // Если нужно посмотреть содержимое вектора значений 

    for (i = 0; i < N; i++)
    {
        delete[] A[i];
    }
    delete[] A;
    delete[] B;
    delete[] X;
    delete[] R;
    delete[] Z;
    delete[] currentVector;
    delete[] rightX;

    return 0;
}