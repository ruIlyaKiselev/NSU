#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <mpi.h>

int loadMatrixFromBinaryFile(const char* fileName, double* matrix, const int matrixSize);
int loadVectorFromBinaryFile(const char* fileName, double* vector, const int vectorSize);
void printMatrix(double* matrix, int matrixSize);
void printVector(double* vector, int vectorSize);
bool checkResults(double* rightX, double* X, int N, double epsilon);