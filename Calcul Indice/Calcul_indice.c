#include<stdio.h>
#include<stdlib.h>
#include"factorisation_v3.h"
#include"theoreme_chinois.h"

#define m 7

int* creation_base()
//retourne pointeur vers liste des m premiers entiers premiers = {p1, ... ,p_m}, rangé dans l'ordre croissant.
{
	int *base;
	base=(int*)calloc(m,sizeof(int));
	int j=2;
	int compteur=0;
	while (compteur<m)
	{
		if (est_premier(j,20)==1)
		{
			*(base + compteur)=j;
			compteur++;
		}
		j++;
	}
	return(base);
}

int generateur_nombre(int q)
//renvoie un nombre modulo q, aléatoire si possible, entre 1 et q-1
{
	int a;
	a=(rand()%(q-1))+1;
	//printf("generateur_nombre(%d) = %d\n",q,a);
	return(a);
}

int est_dans(int y,int *liste)
//retourne 1 si y est dans la liste de taille m, 0 sinon
{
	int flag=0;
	int j;
	for(j=0;j<m;j++)
	{
		if (y==liste[j])
			flag=1;
	}
	return(flag);
}

void est_dans_factorisation(int y,int *liste,int *estdans_pt,int *position_pt)
//comme est_dans mais dans le cas ou liste est une liste produite par la fonction factorisation
//en plus: met 1 dans estdans si y est dans liste. Si oui, met dans position la position de y dans liste (cad y est le position-eme facteur plus grand).
//demande des pointeurs car c'est passage par pointeur.
{
	int flag=0;
	int j;
	for(j=0;j<m;j++)
	{
		if (y==liste[2*j])
		{
			flag=1;
			*position_pt=j;
		}
	}
	*estdans_pt=flag;
}

int base_friable(int *decomposition,int *base)
//renvoie 1 si le nombre que decomposition représente est un produit des elements de la base
{
	int drapeau=1;
	int i=0;
	while( *(decomposition + (2*i)) != 0 )
	{
		if (est_dans( *(decomposition + (2*i)) , base ) == 0)
			drapeau=0;
		i++;
	}
	return(drapeau);
}


void creation_relation_base(int *base,int tab[(2*m)+1][m+1],int p,int alpha)
//tab[2m+1][m+1] doit etre créer avant
//remplit tab
//tab[i]=[a1,...,am,nb] tq alpha^nb = p1^a1 * ... * pm^am

//attention ne marche pas pour p=0,1,2

{
	int nombre;
	int x;
	int *facteur_x;
	int j;
	int estdans;
	int position;
	int compteur=0;
	
	while(compteur<( (2*m)+1 ))
	{
		nombre=generateur_nombre(p-1);
		x=square_and_multiply(nombre,alpha,p);
		while (x==(1%p)) //x ne doit pas valoir 0 ou 1 pour factorisation. 0 est pas possible parce x est une puissance.
		{
			nombre=generateur_nombre(p-1);
			x=square_and_multiply(nombre,alpha,p);
		}
		//printf("x=%d\n",x);
		facteur_x=factorisation(x);
		if (base_friable(facteur_x,base)==1)
		{
			for(j=0;j<=(m-1);j++)
			{
				est_dans_factorisation(base[j],facteur_x,&estdans,&position); //cf passage par pointeur.
				if (estdans==1)
					tab[compteur][j]= *(facteur_x + (2*position) +1);
				else
					tab[compteur][j]=0; //attention le square and multiply ne prend pas la puissance 0
			}
			tab[compteur][m]=nombre;
			compteur++;
		}
		free(facteur_x);
	}
}





void echanger_lignes(int tab[(2*m)+1][m+1],int a,int b,int decalage)
// echange la ligne a avec la ligne b de la matrice tab ou les decalege premières lignes et colonnes ne comptent pas
{
	int transition;
	int k;
	for(k=0;k<=(m-decalage);k++)
	{
		transition=tab[decalage + a][decalage + k];
		tab[decalage + a][decalage + k]=tab[decalage + b][decalage + k];
		tab[decalage + b][decalage + k]=transition;
	}
}

void multiplication_ligne(int tab[(2*m)+1][m+1],int decalage,int i ,int lambda, int p)
//multiplie toute la ligne par lambda, en ne comptant pas les decalage premières colonnes.
{
	int k;
	for(k=0;k<=(m-decalage);k++)
	{
		tab[i][k+decalage]=(lambda*tab[i][k+decalage])%p;
	}
}

void operation_elementaire(int tab[(2*m)+1][m+1],int decalage,int j,int i, int lambda,int p)
// Lj <- Lj - lambda*Li
{
	int k;
	for(k=0;k<=(m-decalage);k++)
	{
		tab[decalage + j][decalage + k]=tab[decalage + j][decalage + k] - ((lambda*tab[decalage + i][decalage + k])%p);
	}
}


void premiere_colonne_non_nulle(int tab[(2*m)+1][m+1],int decalage)
//echange des lignes jusqu'à ce que la première colonne soit non nulle
{
}

void premier_coeff_non_nul(int tab[(2*m)+1][m+1],int decalage)
//modifie tab en échangeant lignes entre elles pour que le coef a_1_1 soit non nul.
{
	int ligne_pour_echange=1;
	while(tab[decalage+0][decalage+0]==0)
	{
		echanger_lignes(tab,0,ligne_pour_echange,decalage);
		ligne_pour_echange++;
	}
}

void etape3(int tab[(2*m)+1][m+1],int decalage,int p)
{
	int j;
	for(j=1;j<=( (2*m) -decalage);j++)
	{
		operation_elementaire(tab,decalage,j,0, tab[decalage][decalage + j] ,p);
	}
}

int test_nul_matrice(int tab[(2*m)+1][m+1],int decalage)
//renvoie 0 si la matrice décalée est la matrice nulle, 1 sinon
{
	int drapeau=0;
	int i;
	int j;
	for(i=0;i<=( (2*m) -decalage);i++)
	{
		for(j=0;j<=(m-decalage);j++)
		{
			if (tab[decalage +i][decalage +j]!=0)
				drapeau=1;
		}
	}
	return(drapeau);
}


void Pivot_Gauss_recurrence(int tab[(2*m)+1][m+1],int decalage,int p)
{
	premiere_colonne_non_nulle(tab,decalage);
	premier_coeff_non_nul(tab,decalage);
	multiplication_ligne(tab,decalage,0, (inv_mod(tab[decalage][decalage],p)+p)%p ,p);
	etape3(tab,decalage,p);
	if (test_nul_matrice(tab,decalage)==1)
		Pivot_Gauss_recurrence(tab,decalage+1,p);
}

int test_nul_ligne(int tab[(2*m)+1][m+1],int i)
//renvoie 0 si la ligne i est nulle, 1 sinon
{
	int drapeau=0;
	int k;
	for(k=0;k<=m;k++)
	{
		if (tab[i][k]!=0)
			drapeau=1;
	}
	return(drapeau);
}

int place_pivot(int tab[(2*m)+1][m+1],int i)
//retourne la position du pivot,cad la première case non nulle de la ligne i
{
	int position=0;
	while(tab[i][position]==0)
	{
		position++;
	}
	return(position);
}

void derniere_etape(int tab[(2*m)+1][m+1],int p)
{
	int i;
	int k;
	int j;
	for(i=(2*m);i>=0;i--)
	{
		if (test_nul_ligne(tab,i)==1)
		{
			j=place_pivot(tab,i);
			for(k=0;k<i;k++)
			{
				operation_elementaire(tab,0,k,i, tab[k][j] ,p);
			}
		}
	}
}

void Pivot_Gauss(int tab[(2*m)+1][m+1],int p)
//modifie tab et renvoie l'unique matrice échelonnée réduite équivalente.

// ATTENTION: RQ: DANS TOUTE CETTE FONCTION ON COMMENCERA PAR COMPTER LES LIGNES ET LES COLLONES PAR 0
{
	Pivot_Gauss_recurrence(tab,0,p);
	derniere_etape(tab,p);
}







void main(int argc, char** argv)
{
	int p;
	p=atoi(argv[1]);
	int alpha;
	alpha=atoi(argv[2]);
	int tab[(2*m)+1][m+1];
	int *base;
	base=creation_base();
	creation_relation_base(base,tab,p,alpha);
	//Pivot_Gauss(tab,p);
	free(base);
	printf("jusqu'ici tout va bien");
}