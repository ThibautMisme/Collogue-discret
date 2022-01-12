#include <stdio.h>
#include <stdlib.h>

unsigned int inv_mod(unsigned int x, unsigned int y)
{
	// On s'inspire de la remontée des restes pour trouver les coefs de Bézout //
	// r0 = q*r1 + r2 -> r2 = r0- q*q1 //
	// rk+2 = rk - q*rk+1 = a_k+2*r0 + b_k+2*r1 //
	// avec rk et rk+1 s'exprimant en fonction de r0 et r1 //
	// on ne fait donc pas la remontée des restes, on calcule les coefficients //
	// au fur et à mesure //

	// création des variables: ci dessus a_k = tempx1, b_k = tempy1 //
	// a_k+1 = tempx2, b_k+1 = tempy2 //
	unsigned int tempx1, tempx2, tempy1, tempy2, tempor;
	div_t temp;

	unsigned int r0 = y;
	unsigned int r1 = x;

	int sens;

	// l'algorithme qui suit trouve toujours les coefs de bézout //
	// /!\ mais il faut se rappeler si x<y ou pas //
	if(x<y)
	{
		sens = 1;
	}
	
	else
	{
		sens = 0;
	}

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

if(sens==1)
{
	printf("%d = %d * %d + %d * %d \n",r0, tempx1, x, tempy1, y );
	return tempx1;
}

else
{
	printf("%d = %d * %d + %d * %d \n",r0, tempy1, x, tempx1, y );
	return tempy1;
}

}

int main(int *argc,char **argv)
{
	// tableau contenant les valeurs de x mod(p_i^e_i) //
	unsigned int tab1[] = {};
	// tableau conteant les facteurs premiers entre (ie: les p_i ^ e_i) //
	unsigned int tab2[] = {};
	// on va recalculer p-1 (l'ordre du groupe) //
	unsigned int M;
	// s est l'élément de Z/p-1Z tq s=tab1[i] mod(tab2[i]) //
	unsigned int s;

	// tout est dans le nom: nombre de facteurs premiers distincts //
	int nb_facteurs_premiers = (int) (sizeof(tab)/ sizeof(tab[0]));


	// calcul de "p-1" = M //
	for(int k = 0, k<nb_facteurs_premiers,k++)
	{
		M = M * tab2[k];
	}


	// si x congru à x_i mod (p_i)^(e_i), alors x = \sum x_i * y_i * M_i mod(M) //
	// avec y_i = (M_i)^-1 mod((p_i)^(e_i)) et M_i = M/((p_i)^(e_i)) //
	// Le TRC quoi ... //

	for(int i=0,i<nb_facteurs_premiers,i++)
	{
		s+=tab1[i]*inv_mod(M/tab2[i],tab2[i])*(M/tab2[i]);
	}


	return s
}