#include<stdio.h>
#include<stdlib.h>
#include"factorisation_v3.h"
#include"theoreme_chinois.h"

#define m 3


void afficher_tableau_classique(int a,int b,int tb[a][b],const char * nom)
{
	int i;
	int j;
	for(i=0;i<a;i++)
	{
		printf(nom);
		printf("[%d]= ",i);
		for(j=0;j<b;j++)
		{
			printf("%d, ",tb[i][j]);
		}
		printf("\n");
	}
}

void afficher_liste_classique(int a,int *pt,const char * nom)
{
	int i;
	printf(nom);
	printf("=");
	for(i=0;i<a;i++)
	{
		printf("%d ,",pt[i]);
	}
	printf("\n");
}




// BASE ET RELATIONS BASE

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







//PARTIE PIVOT DE GAUSS

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
		tab[i+decalage][k+decalage]=( ((lambda%p)*tab[i+decalage][k+decalage])%p + p)%p;
	}
}

void operation_elementaire(int tab[(2*m)+1][m+1],int decalage,int j,int i, int lambda,int p)
// Lj <- Lj - lambda*Li
{
	int k;
	for(k=0;k<=(m-decalage);k++)
	{
		tab[decalage + j][decalage + k]=(( tab[decalage + j][decalage + k] - (((lambda%p)*tab[decalage + i][decalage + k])%p) )+p)%p;
	}
}

int test_nul_colonne(int tab[(2*m)+1][m+1],int decalage,int j)
//renvoie 0 si la colonne j, en prenant en compte le decalage, est nulle, 1 sinon
{
	int drapeau=0;
	int k;
	for(k=0;k<=((2*m)-decalage);k++)
	{
		if (tab[k+decalage][j+decalage]!=0)
			drapeau=1;
	}
	return(drapeau);
}

//void echanger_colonne(int tab[(2*m)+1][m+1],int a,int b,int decalage)
// echange la colonne a avec la colonne b de la matrice tab ou les decalage premières lignes et colonnes ne comptent pas
//{
//	int transition;
//	int k;
//	for(k=0;k<=((2*m)-decalage);k++)
//	{
//		transition=tab[decalage + k][decalage + a];
//		tab[decalage + k][decalage + a]=tab[decalage + k][decalage + b];
//		tab[decalage + k][decalage + b]=transition;
//	}
//}


void premiere_colonne_non_nulle(int tab[(2*m)+1][m+1],int decalage)
//echange des colonnes jusqu'à ce que la première colonne soit non nulle
//n'est pas utilie pour premier tour de pivot_Gauss_rec, mais l'est pour les suivants
{
	if (test_nul_colonne(tab,decalage,0)==0)
		printf("Pivot de Gauss est tombée sur une colonne vide");
}

//	int colonne_pour_echange=1;
//	while(test_nul_colonne(tab,decalage,0)==0)
//	{
//		echanger_colonne(tab,0,colonne_pour_echange,decalage);
//		colonne_pour_echange++;
//	}


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
		operation_elementaire(tab,decalage,j,0, tab[j+decalage][decalage] ,p);
	}
}

//void procedure_decalage_fin(int tab[(2*m)+1][m+1],int decalage)
//{
//	if(test_nul_colonne(tab,decalage,decalage)==1)
//	{
//		tab[decalage][decalage]=1;
//		int k;
//		for(k=1;k<=((2*m)-decalage);k++)
//		{
//			tab[decalage +k][decalage]=0;
//		}
//	}
//}

int test_nul_matrice(int tab[(2*m)+1][m+1],int decalage)
//renvoie 0 si la matrice décalée est la matrice nulle, 1 sinon
{
	if (decalage<(m-1))
	{
		int drapeau=0;
		int i;
		int j;
		for(i=1;i<=( (2*m) -decalage);i++)
		{
			for(j=1;j<=(m-decalage);j++)
			{
				if (tab[decalage +i][decalage +j]!=0)
					drapeau=1;
			}
		}
		return(drapeau);
	}
	else
	{
		//procedure_decalage_fin(tab,decalage);//cas pathogène lorsque decalage=m
		return(0);
	}
}


void Pivot_Gauss_recurrence(int tab[(2*m)+1][m+1],int decalage,int p)
{
	premiere_colonne_non_nulle(tab,decalage);
	premier_coeff_non_nul(tab,decalage);
	multiplication_ligne(tab,decalage,0, (inv_mod(tab[decalage][decalage],p)+p)%p ,p);
	etape3(tab,decalage,p);
	//printf("decalage = %d et test_nul_matrice = %d\n",decalage,test_nul_matrice(tab,decalage));
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
	for(i=(2*m);i>0;i--)
	{
		if (test_nul_ligne(tab,i)==1)
		{
			j=place_pivot(tab,i);
			for(k=(i-1);k>=0;k--)
			{
				operation_elementaire(tab,0,k,i, tab[k][j] ,p);
			}
		}
	}
}

void Pivot_Gauss(int tab[(2*m)+1][m+1],int p)
//modifie tab et renvoie l'unique matrice échelonnée réduite équivalente.

// ATTENTION: RQ: DANS TOUTE CETTE FONCTION ON COMMENCERA PAR COMPTER LES LIGNES PAR 0
// ATTENTION: si on tombe sur colonne vide: sort segmentation fault
{
	Pivot_Gauss_recurrence(tab,0,p);
	derniere_etape(tab,p);
}







// LOG BASE

void reduction_mod(int tab1[(2*m)+1][m+1],int tab2[(2*m)+1][m+1],int q)
//met dans tab2 les coefs de tab1 mod q
{
	int i;
	int j;
	for(i=0;i<=(2*m);i++)
	{
		for(j=0;j<=m;j++)
		{
			tab2[i][j]=tab1[i][j]%q;
		}
	}
}

int* transformation_facteur(int *facteur_q,int taille)
//va donner pointeur liste utile pour le theoreme chinois
{
	int *output;
	output=(int*)calloc(taille+1,sizeof(int));// il faut que la dernière case soit un zero pour theoreme_chinois
	int k;
	for(k=0;k<taille;k++)
	{
		output[k]=facteur_q[2*k];
	}
	return(output);
}

int* preparation_log(int *taille,int q)
{
	int *facteur_q;
	facteur_q=factorisation(q);
	// ATTENTION il faut que les facteurs de q soient simples
	*taille=calcul_taille(facteur_q)/2;
	return(transformation_facteur(facteur_q,*taille));
	free(facteur_q);
}

void remplissage_chinois(int taille_facteur,int chinois[m][taille_facteur],int *facteur,int tab[(2*m)+1][m+1])
{
	int reduit_modulo[(2*m)+1][m+1];
	int rho;
	int i;
	int k;
	for(k=0;k<taille_facteur;k++)
	{
		rho=facteur[k];
		reduction_mod(tab,reduit_modulo,rho);
		Pivot_Gauss(reduit_modulo,rho);
		for(i=0;i<m;i++)
		{
			chinois[i][k]=reduit_modulo[i][m];
		}
	}
}

int* theoreme_chinois_log(int taille_facteur,int chinois[m][taille_facteur],int *facteur,int p)
{
	int *log;
	log=(int*)calloc(m,sizeof(int));
	int i;
	//afficher_tableau_classique(m,taille_facteur,chinois,"chinois");
	//afficher_liste_classique(taille_facteur,facteur,"facteur");

	for(i=0;i<m;i++)
	{
		log[i]=theoreme_chinois(chinois[i],facteur,p);
	}
	//afficher_liste_classique(m,log,"log");
	return(log);
}

int* log_base(int tab[(2*m)+1][m+1],int p)
//crée un pointeur liste des logarithmes des éléments de la base
// chinois[k]=[log_alpha(p1) (mod r), ... , log_alpha(pm) (mod r)]
{
	int q=(p-1);
	int *log;
	int *facteur;
	int taille_facteur;
	facteur=preparation_log(&taille_facteur,q);
	int chinois[m][taille_facteur];

	remplissage_chinois(taille_facteur,chinois,facteur,tab);
	return(theoreme_chinois_log(taille_facteur,chinois,facteur,p));
	free(facteur);
}

void preprocessing(int p, int alpha, int base[m][2])
// Cette étape peut se faire avant de connaitre beta, en amont.
// renvoie le tableau base complété. tab[i]=[pi,log_alpha(pi)]
{
	int tab[(2*m)+1][m+1];
	int *prebase;
	prebase=creation_base();
	creation_relation_base(prebase,tab,p,alpha);
	int *log;
	log=log_base(tab,p);

	int k;
	for(k=0;k<m;k++)
	{
		base[k][0]=prebase[k];
		base[k][1]=log[k];
	}

	free(log);
	free(prebase);
}




void afficher_tableau(int tab[(2*m)+1][m+1],int i, int j,int decalage)
//affiche le tableau de la ligne i à la ligne j en prenant compte du décalage
//attention, les lignes sont comptées à partir de 0.
// j>=i
{
	int ligne;
	int k;
	for(ligne=0;ligne<=(j-i);ligne++)
	{
		printf("tab=[%d] = ",i +ligne);
		for(k=0;k<=(m-decalage);k++)
		{
			printf("%d, ",tab[i+ligne+decalage][decalage+k]);
		}
		printf("\n");
	}
	printf("\n");	
}







// SEGMENTATION FAULT: pourrait etre que Pivot de Gauss est tombé sur une colonne vide.

void main(int argc, char** argv)
{
	int p;
	p=atoi(argv[1]);
	int alpha;
	alpha=atoi(argv[2]);
	
	int base[m][2];
	preprocessing(p,alpha,base);
	afficher_tableau_classique(m,2,base,"base");
	printf("jusqu'ici tout va bien");
}