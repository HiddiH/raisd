#include "RAiSD.h"

double snpv_cmp_range (uint64_t * A, uint64_t * B, int size)
{
	assert(A!=NULL);
	assert(B!=NULL);
	assert(size>=1);
	
	int i;
	for(i=0;i!=size;++i)
		if(A[i]!=B[i])
		{	
			return 0.0;
		}
	
	return 1.0;
}

double isnpv_cmp_range (uint64_t * A, uint64_t * B, int size, int numberOfSamples)
{
	assert(A!=NULL);
	assert(B!=NULL);
	assert(size>=1);
	
	int i;
	for(i=0;i!=size-1;++i)
	{
		if((~A[i])!=B[i])
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