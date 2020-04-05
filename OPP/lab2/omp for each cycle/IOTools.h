#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>

using namespace std;

int loadMatrixFromBinaryFile(string fileName, double** matrix);
int loadVectorFromBinaryFile(string fileName, double* vector);
void loadMatrixFromFile(string fileName, double** matrix, int matrixSize);
void loadVectorFromFile(string fileName, double* vector, int vectorSize);
void printMatrix(double** matrix, int matrixSize);
void printVector(double* vector, int vectorSize);
void saveMatrixToFile(double** matrix, string fileName, int matrixSize);
void saveVectorToFile(double* vector, string fileName, int vectorSize);
bool checkResults(double* rightX, double* X, int N, double epsilon);