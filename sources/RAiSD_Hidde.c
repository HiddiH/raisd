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
	if (numB1<=0 || numB1>=numberOfSamples)
	{
		for (i = 0; i != size; ++i)
		{
			printf("%lu \t popcnt: %lu\n", B[i], __builtin_popcount(B[i]));
		}
	}
	fflush(stdout);
	assert(numA1>0 && numA1<numberOfSamples);
	assert(numB1>0 && numB1<numberOfSamples);

	// Probabilities are too low, when a value of 0.5 is expected the value is 0.32
	double Pij = (double) numMatch / (double) numberOfSamples;
	double Pi = (double) numA1 / (double) numberOfSamples;
	double Pj = (double) numB1 / (double) numberOfSamples;

	double D = Pij - Pi * Pj;
	double r2 = (D * D) / (Pi * Pj * (1.0 - Pi) * (1.0 - Pj));

	if (isnan(r2)) // Nan occurs when Pi or Pj (and thus Pij) is zero
	{
		FILE *nan;
		nan = fopen("nan.txt", "a");
		fprintf(nan, "Pij:%f\tPi:%f\tPj:%f\tD:%f\tr:%f\n", Pij, Pi, Pj, D, r2);
		fclose(nan);
	}
	if ((fabs(r2 - 0.221453)) < 0.00001) // 0.221453 is 1.0 when 0.32 is moved to 0.5 (opposide image)
	{
		FILE *weird;
		weird = fopen("weird.txt", "a");
		fprintf(weird, "Pij:%f\tPi:%f\tPj:%f\tD:%f\tr:%f\n", Pij, Pi, Pj, D, r2);
		fclose(weird);
	}
	if ((fabs(r2 - 1.0)) < 0.0001)
	{
		FILE *matchValue;
		matchValue = fopen("matchValue.txt", "a");
		fprintf(matchValue, "Pij:%f\tPi:%f\tPj:%f\tD:%f\tr:%f\n", Pij, Pi, Pj, D, r2);
		fclose(matchValue);
	}

	assert(r2>=0.0 && r2 <= 1.0); // TODO proper float comparison
	return r2;

}

double isnpv_cmp_range (uint64_t * A, uint64_t * B, int size, int numberOfSamples)
{
	assert(A!=NULL);
	assert(B!=NULL);
	assert(size>=1);
	
	int i;
	for (i=0;i!=size-1;++i)
	{
		if ((~A[i])!=B[i])
		{
			return 0.0;
		}
	}
	uint64_t temp = ~B[size-1];
	int shiftLast = 64-(numberOfSamples-(size-1)*64);
	temp = temp << shiftLast;
	temp = temp >> shiftLast;
	if(temp!=A[size-1])
		return 0.0;

	return 1.0;
}