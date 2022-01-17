#include <stdio.h>
#include <stdlib.h>

int inv_mod(int x,int y)
{
	// On s'inspire de la remontée des restes pour trouver les coefs de Bézout //
	// r0 = q*r1 + r2 -> r2 = r0- q*q1 //
	// rk+2 = rk - q*rk+1 = a_k+2*r0 + b_k+2*r1 //
	// avec rk et rk+1 s'exprimant en fonction de r0 et r1 //
	// on ne fait donc pas la remontée des restes, on calcule les coefficients //
	// au fur et à mesure //

	// création des variables: ci dessus a_k = tempx1, b_k = tempy1 //
	// a_k+1 = tempx2, b_k+1 = tempy2 //
	 int tempx1, tempx2, tempy1, tempy2, tempor;
	div_t temp;

	int r0 = y;
	int r1 = x;

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


	// printf("%d = %d * %d + %d * %d \n",r0, tempx1, y, tempy1, x ); bout de code de vérification//

	return tempy1;


}

int calcul_taille(int *pointeur)
//calcul la taille du tableau pointeur, cad la position du premier 0
{
	int i=0;
	int drapeau=1;
	while (drapeau==1)
	{
		if (*(pointeur+i) == 0)
			drapeau=0;
		i++;
	}
	return(i-1);
}

int theoreme_chinois(int *tab1,int *tab2,int p)
//tab1= valeurs de x mod(p_i^e_i)
//tab2= facteurs premiers entre (ie: les p_i ^ e_i) 
{
	int M=p-1;
	int nb_facteurs_premiers = calcul_taille(tab2);
	int s=0;
	for(int i=0 ;i<nb_facteurs_premiers ;i++)
	{
		s+=(tab1[i]*inv_mod(M/tab2[i],tab2[i])*(M/tab2[i]))%M;
	}
	s= (s+M)%M;
	return(s);
}

//int main(int *argc,char **argv)
//{
	// tableau contenant les valeurs de x mod(p_i^e_i) //
//	int tab1[] = {2,1,16};
	// tableau conteant les facteurs premiers entre (ie: les p_i ^ e_i) //
//	int tab2[] = {4,9,25,0};
//	int s;
//	int p;
//	p=(tab2[0]*tab2[1]*tab2[2])+1;
//	s= theoreme_chinois(tab1,tab2,p);
//	int nb_facteurs_premiers;
//	nb_facteurs_premiers=calcul_taille(tab2);
//	printf("On a bien que %d verifie les equations modulaires suivantes:\n", s);
//	for(int l = 0; l < nb_facteurs_premiers ;l++)
//	{
//		printf("%d = %d * %d + %d = %d mod(%d)\n",s,s/tab2[l],tab2[l],s%tab2[l],tab1[l],tab2[l]);
//	}
//}