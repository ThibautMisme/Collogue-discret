#include <time.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int main(int *argc, char **argv)
{
	unsigned int x = 16;
	unsigned int y = 113;

	unsigned int tempx1, tempx2, tempy1, tempy2, tempor;
	div_t temp;

	unsigned int r0 = y;
	unsigned int r1 = x;

	tempx1=1; tempx2=0;
	tempy1=0; tempy2=1;

	// descente des restes et création simultanée des constantes de bézout //
	//  pour la DE //
	
	while(r1 != 0)
	{
		temp=div(r0,r1);

		r0=r1;
		r1=temp.rem;

		tempor=tempx2;
		tempx2=tempx1-(temp.quot)*tempx2;
		tempx1=tempor;

		tempor=tempy2;
		tempy2=tempy1-(temp.quot)*tempy2;
		tempy1=tempor;
	}


	printf("%d = %d * %d + %d * %d \n",r0, tempx1, y, tempy1, x );
	return tempy1;

}