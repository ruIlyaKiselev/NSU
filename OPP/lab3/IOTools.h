#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <mpi.h>

int loadMatrixFromBinaryFile(const char* fileName, double* matrix, const int matrixSize);
void printMatrix(double* matrix, int matrixSize);
bool checkResults(double* rightM, double* M, int N, double epsilon);