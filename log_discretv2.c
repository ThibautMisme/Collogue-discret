#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include"theoreme_chinois.h"
#include"b0Camille.h"
#include"factorisationv2.h"

int* modification_tableau(int* pointeur)
//cette fonction crée un tableau contenant les facteurs premiers de p-1 à partir du pointeur renvoyé par Thibaut.
{
	int *tableau;
	tableau=(int*)calloc(11,sizeof(int)); //les fonctions de Camille ne marchent que pour des nombres qui ont moins de 10 facteurs premiers
	int i;
	for(i=0;i<=9;i++)
	// la 11eme case sera un 0, comme le veut Camille, grace au calloc
	{
		*(tableau + i)= *(pointeur + (2*i));
	}
	return(tableau);
}

int log_discret(int p,int alpha,int beta)
{
	int q=p-1;
	int *facteur_q;
	facteur_q=modification_tableau(factorisation(q));
	int *pour_charles;
	pour_charles=(int*)calloc(10,sizeof(int));
	//va acceuillir les beta^alpha [pi^ei]

	int b0;
	int rho;
	int i;
	for(i=0;i<=9;i++)
	//on rappelle que facteur_q est un tableau de taille 11 qui réunit les facteurs de p-1=q et leur multiplicités, dont la dernière case est un 0
	{
		if ( *(facteur_q + i)!=0 )
		{
			rho=*(facteur_q + i); //attention la multiplicité de rho doit etre 1
			b0=coef_bj (alpha,beta,p,rho);
			*(pour_charles + i)=b0%rho; 
			printf("b0 = %d\n",b0%rho);
		}
	}
	int a=1;
	//c'est la puissance telle que alpha^a=beta
	a=theoreme_chinois(pour_charles,facteur_q,p); //c'est le nom de la fonction que Charles a codé (elle était en main, j'ai juste changé le nom)
	//rq pour l'instant les facteurs premiers entre eux, sont égaux aux facteurs premiers
	printf("a = %d\n",a);
	return(a);
	free(pour_charles);
	free(facteur_q);
}

int main(int argc, char** argv)
{
	int p;
	p=atoi(argv[1]);
	int alpha;
	alpha=atoi(argv[2]);
	int beta;
	beta=atoi(argv[3]);
	int log;
	log=log_discret(p,alpha,beta);
	printf("log discret = %d\n", log );
	printf("on a bien %d = %d \n",beta%p, square_and_multiply( log , alpha , p ) );
}
