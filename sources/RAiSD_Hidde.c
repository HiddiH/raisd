#include "RAiSD.h"

double snpv_cmp_range (uint64_t * A, uint64_t * B, int size, int numberOfSamples)
{
	assert(A!=NULL);
	assert(B!=NULL);
	assert(size>=1);
	

	int i, numA1 = 0, numB1 = 0, numMatch = 0;
	for (i = 0; i != size; ++i) // Counting the amount of SNP i, j and ij
	{
		numA1 += rsd_popcnt_u64(A[i]) ;
		numB1 += rsd_popcnt_u64(B[i]);
		numMatch += rsd_popcnt_u64(A[i] & B[i]);
	}

	// Calculating the probabilities
	double Pij = (double) numMatch / (double) numberOfSamples;
	double Pi = (double) numA1 / (double) numberOfSamples;
	double Pj = (double) numB1 / (double) numberOfSamples;

	double D = Pij - Pi * Pj;
	double r2 = (D * D) / (Pi * Pj * (1.0 - Pi) * (1.0 - Pj));

	assert(r2>=-0.000001 && r2 <= 1.000001); 
	return r2;

}
