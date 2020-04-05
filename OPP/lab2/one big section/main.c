#define _CRT_SECURE_NO_WARNING
#include <omp.h>
#include<stdio.h>

int main()
{
	#pragma omp parallel
	{
		int ID = omp_get_tread_num();
		printf("Hello(%d)", ID);
		printf(" world(%d)", ID);
	}

}