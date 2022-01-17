#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include"theoreme_chinois.h"
#include"bj.h"
#include"factorisation_v3.h"

int square_and_multiply_classique(int m, int a)
// renvoie a^m
// c'est juste pour creation_tableau_facteurs_premiers_entre_eux
// il y a des galères avec 0
{
	if (m==1)
		return(a);
	else
		if (m&1 == 0)
			return( square_and_multiply_classique(m>>1, a*a )  );
		else
			return( a*square_and_multiply_classique(m-1,a) );
}

int* creation_tableau_facteurs_premiers_entre_eux(int* pointeur,int nb_facteurs)
// va créer le tab2 dont Charles à besoin pour theoreme_chinois: c'est tableau qui contient les pi^ej
//à partir du tableau factorisation(q)
{
	int *tableau;
	tableau=(int*)calloc(nb_facteurs,sizeof(int));
	int i;
	for(i=0;i<=(nb_facteurs-1);i++)
	{
		if ( *(pointeur + (2*i)) == 0 )
			*(tableau + i) = 0;
		else
			*(tableau + i)=square_and_multiply_classique( *(pointeur + (2*i) + 1) , *(pointeur + (2*i)) ); 
	}
	return(tableau);
}


int log_discret(int p,int alpha,int beta)
{
	//initialisation des valeurs récurrentes du code et des tableaux néccéssaires:
	int q=p-1;
	int nb_facteurs;
	nb_facteurs=(log_base2(p)+1);//on rajoute une case à 0 pour savoir que c'est fin du tableau

	int *facteur_q;
	facteur_q=factorisation(q);
	//tableau qui contient les facteurs de q dans les cases paires et leur multiplicités dans les cases impaires.

	int *pour_charles;
	pour_charles=(int*)calloc(nb_facteurs,sizeof(int));
	//va acceuillir les beta^alpha [pi^ei]

	int *facteurs_premiers_entre_eux;
	facteurs_premiers_entre_eux=creation_tableau_facteurs_premiers_entre_eux(facteur_q,nb_facteurs);
	//va acceuillir les pj^ej


	//debut de la boucle principale:

	//initialisation des variables de la boucle
	int bj;
	int rho;
	int i;
	//boucle principale
	for(i=0;i<=nb_facteurs-1;i++)
	{
		if ( *(facteur_q + (2*i))!=0 )
		{
			rho=*(facteurs_premiers_entre_eux + i);
			bj=coef_bj(rho,beta%rho,alpha%p,q);
			printf("bj= %d\n",(bj+rho)%rho);
			*(pour_charles + i)=(bj+rho)%rho; 
		}
	}

	//fin du code:
	return(theoreme_chinois(pour_charles,facteurs_premiers_entre_eux,p));
	free(pour_charles);
	free(facteur_q);
	free(facteurs_premiers_entre_eux);
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



