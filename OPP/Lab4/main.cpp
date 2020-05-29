#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include <mpi.h>
#include <assert.h>
#include<iostream>
#include<algorithm>

double x = -1, x2 = 1;
double Dx = x2 - x;
double y = -1, y2 = 1;
double Dy = y2 - y;
double z = -1, z2 = 1;
double Dz = z2 - z;

const int Nx = 500;
const int Ny = 100;
const int Nz = 300;

const double a = 10000;
const double eps = 0.0001;

double hx = Dx / (Nx - 1);
double hy = Dy / (Ny - 1);
double hz = Dz / (Nz - 1);

double f(double x, double y, double z) 
{
    return (double)(x * x + y * y + z * z);
}

double ro(double x, double y, double z, double a) 
{
    return (double)(6 - a * f(x, y, z));
}

double* init() 
{
    int count = 0;
    double* GridPlad = new double[Nx * Ny * Nz];
    
    for (int z = 0; z < Nz; z++)
    {
        for (int y = 0; y < Ny; y++)
        {
            for (int x = 0; x < Nx; x++)
            {
                if ((x != 0) && (y != 0) && (z != 0) && (z != Nz - 1) && (y != Ny - 1) && (x != Nx - 1))
                {
                    GridPlad[z * Nx * Ny + y * Nx + x] = 0;
                }
                else
                {
                    GridPlad[z * Nx * Ny + y * Nx + x] = f(x + x * hx, y + y * hy, z + z * hz);
                }
                
                count++;
            }
        }
    }

    return GridPlad;
}

void calcDisplsPs(int* displs, int* Ps, int size)  
{
    int count = Nz / size;
    int sq = Nz % size;
    int curOffset = 0;
    
    for (int i = 0; i < size; i++) 
    {
        Ps[i] = count;
        
        if (sq > 0) 
        {
            Ps[i]++;
            sq--;
        }
        
        Ps[i] *= Nx * Ny;
        displs[i] = curOffset;
        curOffset += Ps[i];
    }
}

double CalcFi(double VX0, double VX1, double VY0, double VY1, double VZ0, double VZ1, double resro) 
{
    double kef = 1.0 / ((2.0 / (hx * hx)) + (2.0 / (hy * hy)) + (2.0 / (hz * hz)) + a);
    double tX = (VX1 + VX0) / (hx * hx);
    double tZ = (VZ1 + VZ0) / (hz * hz);
    double tY = (VY1 + VY0) / (hy * hy);
    
    return (tX + tY + tZ - resro) * kef;
}

void ChangeBounds(double* Data, int* counts, double* topLay, double* botLay, MPI_Request* topR, MPI_Request* botR, int rank, int size) 
{
    if (rank != 0)
    {
        MPI_Request w1;
        MPI_Isend(Data, Nx * Ny, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &w1); // send message without lock
        MPI_Irecv(botLay, Nx * Ny, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD, botR); // get message without lock
    }
    
    if (rank < size - 1)
    {
        MPI_Request w2;
        MPI_Isend(Data + counts[rank] - Nx * Ny, Nx * Ny, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD, &w2); // send message without lock
        MPI_Irecv(topLay, Nx * Ny, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, topR); // get message without lock
    }
}

double* CalcCenter(double* UPData, double* Data, int* count, int* displs, int rank, int size) 
{
    for (int z = 1; z < (count[rank] / (Nx * Ny)) - 1; z++) 
    {
        for (int y = 1; y < Ny - 1; y++) 
        {
            for (int x = 1; x < Nx - 1; x++) 
            {
                UPData[z * Nx * Ny + y * Nx + x] = CalcFi
                (
                    Data[z * Nx * Ny + y * Nx + x - 1], Data[z * Nx * Ny + y * Nx + x + 1], Data[z * Nx * Ny + (y - 1) * Nx + x], Data[z * Nx * Ny + (y + 1) * Nx + x],
                    Data[(z - 1) * Nx * Ny + y * Nx + x], Data[(z + 1) * Nx * Ny + y * Nx + x], ro(x + hx * x, y + hy * y, z + hz * (z + (displs[rank] / (Nx * Ny))), a)
                );
            }
        }
    }
    return UPData;
}

void CalcBottomBorder(double* UPData, double* Data, int Bound, double* Layer, int displs) 
{
    for (int y = 1; y < Ny - 1; y++)
    {
        for (int x = 1; x < Nx - 1; x++)
        {
            UPData[Bound * Nx * Ny + y * Nx + x] = CalcFi
            (
                Data[Bound * Nx * Ny + y * Nx + x - 1], Data[Bound * Nx * Ny + y * Nx + x + 1], Data[Bound * Nx * Ny + (y - 1) * Nx + x], Data[Bound * Nx * Ny + (y + 1) * Nx + x],
                Layer[x + Nx * y], Data[(Bound + 1) * Nx * Ny + y * Nx + x], ro(x + hx * x, y + hy * y, z + hz * (Bound + (displs / (Nx * Ny))), a)
            );
        }
    }
}

void CalcTopBorder(double* UPData, double* Data, int Bound, double* Layer, int displs) 
{
    for (int y = 1; y < Ny - 1; y++)
    {
        for (int x = 1; x < Nx - 1; x++)
        {
            UPData[Bound * Nx * Ny + y * Nx + x] = CalcFi
            (
                Data[Bound * Nx * Ny + y * Nx + x - 1], Data[Bound * Nx * Ny + y * Nx + x + 1], Data[Bound * Nx * Ny + (y - 1) * Nx + x], Data[Bound * Nx * Ny + (y + 1) * Nx + x],
                Data[(Bound - 1) * Nx * Ny + y * Nx + x], Layer[x + Nx * y], ro(x + hx * x, y + hy * y, z + hz * (Bound + (displs / (Nx * Ny))), a)
            );
        }
    }
}

void GetAndCalcBounds(double* UPData, double* Data, int* counts, int* displs, double* topLay, double* botLay, int rank, int size, MPI_Request* topR, MPI_Request* botR) 
{
    if (rank != 0) 
    {
        MPI_Wait(botR, MPI_STATUS_IGNORE);
        CalcBottomBorder(UPData, Data, 0, botLay, displs[rank]); 
    }

    if (rank < size - 1) 
    {
        MPI_Wait(topR, MPI_STATUS_IGNORE);
        CalcTopBorder(UPData, Data, (counts[rank] / (Nx * Ny)) - 1, topLay, displs[rank]);
    }
}

bool CheckOutDiff(double* Data1, double* Data2, int* count, int rank) // main cycle condition
{
    double diffOnRank = -100, diff = -100;
    
    for (int it = 0; it < count[rank]; it++) 
    {
        if (rank == 0)
        {
            if (diffOnRank < fabs(Data2[it] - Data1[it]))
            {
                diffOnRank = fabs(Data2[it] - Data1[it]);
            }
        }
    }

    MPI_Allreduce(&diffOnRank, &diff, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    return diff < eps ? true : false;
}

int main(int argc, char* argv[]) 
{
    int size, rank;
    double* FirstGridPlad = 0;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int* displs = new int[size];
    int* partSize = new int[size];
    
    if (rank == 0) 
    {
        FirstGridPlad = init();
    }
    
    calcDisplsPs(displs, partSize, size);
    
    double* PartPlad = new double[partSize[rank]];
    double* PartPladUP = new double[partSize[rank]];
    
    MPI_Scatterv(FirstGridPlad, partSize, displs, MPI_DOUBLE, PartPlad, partSize[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    std::copy(PartPlad, PartPlad + partSize[rank], PartPladUP);

    bool endCondition = false;

    double* topLayer = new double[Nx * Ny];
    double* botLayer = new double[Nx * Ny];
    MPI_Request topRequest, botRequest;

    double* tmp;
    double start, finish;
    
    if (rank == 0)
    {
        start = MPI_Wtime();
    }
    
    while (endCondition != true) 
    {
        ChangeBounds(PartPlad, partSize, topLayer, botLayer, &topRequest, &botRequest, rank, size);

        PartPladUP = CalcCenter(PartPladUP, PartPlad, partSize, displs, rank, size);

        GetAndCalcBounds(PartPladUP, PartPlad, partSize, displs, topLayer, botLayer, rank, size, &topRequest, &botRequest);
        endCondition = CheckOutDiff(PartPlad, PartPladUP, partSize, rank);

        tmp = PartPlad;
        PartPlad = PartPladUP;
        PartPladUP = tmp;
    }

    MPI_Gatherv(PartPlad, partSize[rank], MPI_DOUBLE, FirstGridPlad, partSize, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    if (rank == 0) 
    {
        finish = MPI_Wtime();
        std::cout << "Time: " << finish - start << std::endl;
    }

    MPI_Finalize();
    return 0;
}