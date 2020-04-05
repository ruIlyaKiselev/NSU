#pragma once
#include <math.h>

void conjugateGradientMethod(double** A, double* B, double* X, double* R, double* Z, double* currentVector, int N);
double calcVectorNorm(double* vector, int N);
void generateDefaultData(double** A, double* B, double* x, int N);
void copyVectors(double* src, double* dst, int N);
void fillMatrix(double** matrix, int N);
void fillVector(double value, double* vector, int N);