#include<stdio.h>
#include<stdlib.h>

int calcul_r(int q)
{
	int i=0;
	while (((q>>i)&1) == (0&1))
	{
		i++;
	}
	return(i);
}

int square_and_multiply(int m,int a,int p)
{
	if (m==1)
		return(a%p);
	else
		if (m&1 == 0)
			return( (square_and_multiply(m>>1, ( (a%p)*(a%p) )%p ,p ))%p  );
		else
			return( ((a%p)*square_and_multiply(m-1,a%p,p))%p );
}

int etape_finale(int y,int r,int p)
// etape finale de Miller_Rabine
{
	int x=y;
	x=x%p;
	int drapeau=0;
	if ( (((x%p) -1)%p)==0 )
		drapeau=1;
	int j;
	for(j=1;j<=r+1;j++)
	{
		if ( ( ((x%p) +1)%p )==0 )
			drapeau=1;
		x=((x%p)*(x%p))%p;
	}
	return(drapeau);
}

int Miller_Rabine(int p,int a)
//retourne 1 si p semble premier, 0 sinon
{
	if  ((a%p)==1)
		return(1);
	else if ((a%p)==0)
		return(1);
	else
	{
		int q=p-1;
		int r;
		int d;
		int x;
		r=calcul_r(q);
		d=(q>>r);
		x=square_and_multiply(d,a,p);
		int drapeau;
		drapeau=etape_finale(x,r,p);
		return(drapeau);	
	}
	
}

int est_premier(int p, int k)
// retourne 1 si p est premier, 0 si p est composé
{
	int drapeau=1;
	int j;
	for(j=2;j<=k+1;j++)
	{
		if (Miller_Rabine(p,j)==0)
			drapeau=0;
	}
	return(drapeau);
}

int facto_recurrence(int q)
//ressort le plus petit facteur premier de q, 
//supérieur ou égal à borne, si la borne est plus petite que le premier facteur(c'est pour gagner temps de calcul)
//pour l'instant k=20 et c'est le seul endroit ou k apparait
{
	int k;
	k=20;
	int j;
	j=2;
	int drapeau=1;
	if (est_premier(q,k)==1)
	{
		drapeau=0;
		j=q;
	}
	int l;
	while (drapeau==1)
	{
		if (est_premier(j,k)==1)
		{
			l=1; 
			//comme on sait qu'on a testé tous les nombres premiers avant j, si l*j=q, la décomposition de l n'a que des nombres premiers plus grand que j
			//on peut donc initialiser l à j
			while ( (l*j)<q )
			{
				l++;
			}
			if ( (l*j)==q )
			{
				drapeau=0;
				j=j-1; //car il va prendre un +1	
			}			
		}
		if ( j>q ) //c'est pour éviter boucle infini si il y a erreur
			drapeau=0;
		j++;
	}
	return(j);
}

void inclure_tableau(int q,int *pt)
//inclus q dans le tableau, en respectant multiplicité
{
	int i=0;
	int drapeau=1;
	while (drapeau==1)
	{
		if ( (*(pt+ (2*i))) ==q) //une case sur 2 il y a multiplicité
		{
			*(pt+ (2*i) + 1 )=*(pt+ (2*i) + 1 ) + 1;
			drapeau=0;
		}
		if ( (*(pt+ (2*i))) ==0)
		{
			*(pt+ (2*i))=q;
			*(pt+ (2*i) + 1 )= 1;
			drapeau=0;
		}
		i++;
	}
}


void pre_factorisation(int p,int *pointeur)
//il faut déclarer le pointeur avant la fonction
{
	int q;
	int facteur;
	//int borne;
	//borne=2; //tres important d'initialiser à 2
	q=p;
	int i;
	while (q>1)
	{
		facteur=facto_recurrence(q);
		//borne=facteur;
		inclure_tableau(facteur,pointeur);
		q=q/facteur;
	}
}

int* factorisation(int p)
//retourne un pointeur de 40 cases, qui contient dans les premières cases paires les facteurs premiers de p et dans les cases impaires leur multiplicités.
{
	int *pointeur;
	int nb_facteurs=20;
	// nb est le nombre de facteurs maximum
	int taille;
	taille=2*nb_facteurs;
	//attention: toute variable créée dans une fonction disparait hors de la fonction. Il faut donc allouer la mémoire à pointeur AVANT factorisation.
	pointeur=(int*)calloc(taille,sizeof(int));
	//calloc permet de faciliter la fonction inclure_tableau (on sait que tous est à 0)
	//les cases paires sont pour les facteurs, et les cases impaires pour leur multiplicités (on commence à 0)
	pre_factorisation(p,pointeur);
	return(pointeur);
}

//Amélioration: rendre le nombre de facteurs variable. (Pour l'instant, doit etre inférieur à 20)

//ne pas oublier de libérer la mémoire allouée par la fonction factorisation lorsque plus nécessaire. 


//pour tester:
//void main(int argc, char** argv)
//{
//	int p;
//	p=atoi(argv[1]);
//	int *pointeur;
//	pointeur=factorisation(p);
//	int i=0;
//	while ( *(pointeur + (2*i)) != 0 )
//		{
//			printf("facteur %d = %d    ",i+1,*(pointeur + (2*i)) );
//			printf("de multiplicité %d\n",*(pointeur + (2*i) +1) );
//			i++;
//		}
//	free(pointeur);
//}