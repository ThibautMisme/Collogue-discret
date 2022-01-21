#include<stdio.h>
#include<stdlib.h>
#include"factorisation_v3.h"
#include"theoreme_chinois.h"
#include"bj.h"

#define m 3



//FONCTIONS PRATQIUES

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






// BASE ET RELATIONS BASE

int* creation_base(int p)
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
			*(base + compteur)=j%p;
			compteur++;
		}
		j++;
	}
	return(base);
}

int* creation_base_2(int p,int *fact)
//retourne pointeur vers liste des m premiers entiers premiers = {p1, ... ,p_m}, rangé dans l'ordre croissant.
{
	int *base;
	base=(int*)calloc(m,sizeof(int));
	int j=2;
	int compteur=0;
	int estdans=0;
	int position=0;
	while (compteur<m)
	{
		if (est_premier(j,20)==1)
		{
			est_dans_factorisation(j,fact,&estdans,&position);
			if (estdans==0)
			{
				*(base + compteur)=j%p;
				compteur++;
			}
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

void echanger_lignes(int tab[(2*m)+1][m+1],int a,int b,int decalagei,int decalagej)
// echange la ligne a avec la ligne b de la matrice tab ou les decalege premières lignes et colonnes ne comptent pas
{
	int transition;
	int k;
	for(k=0;k<=(m-decalagej);k++)
	{
		transition=tab[decalagei + a][decalagej + k];
		tab[decalagei + a][decalagej + k]=tab[decalagei + b][decalagej + k];
		tab[decalagei + b][decalagej + k]=transition;
	}
}

void multiplication_ligne(int tab[(2*m)+1][m+1],int decalagei,int decalagej,int i ,int lambda, int p)
//multiplie toute la ligne par lambda, en ne comptant pas les decalage premières colonnes.
{
	int k;
	for(k=0;k<=(m-decalagej);k++)
	{
		tab[i+decalagei][k+decalagej]=( ((lambda%p)*tab[i+decalagei][k+decalagej])%p + p)%p;
	}
}

void operation_elementaire(int tab[(2*m)+1][m+1],int decalagei,int decalagej,int n,int i, int lambda,int p)
// Ln <- Ln - lambda*Li
{
	int k;
	for(k=0;k<=(m-decalagej);k++)
	{
		tab[decalagei + n][decalagej+ k]=(( tab[decalagei + n][decalagej + k] - (((lambda%p)*tab[decalagei + i][decalagej + k])%p) )+p)%p;
	}
}

int test_nul_colonne(int tab[(2*m)+1][m+1],int decalagei,int decalagej,int j)
//renvoie 0 si la colonne j, en prenant en compte le decalage, est nulle, 1 sinon
{
	int drapeau=0;
	int k;
	for(k=0;k<=((2*m)-decalagei);k++)
	{
		if (tab[k+decalagei][j+decalagej]!=0)
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


int premiere_colonne_non_nulle(int tab[(2*m)+1][m+1],int decalagei,int decalagej)
//echange des colonnes jusqu'à ce que la première colonne soit non nulle
//n'est pas utilie pour premier tour de pivot_Gauss_rec, mais l'est pour les suivants
//{
	//if (test_nul_colonne(tab,decalage,0)==0)
	//{
	//	printf("Pivot de Gauss est tombée sur une colonne vide\n");
	//	printf("colonne qui pose probleme = %d\n",decalage);
	//	afficher_tableau_classique((2*m)+1,m+1,tab,"tab");
	//}
//}
{
	int delt=0;
	while(test_nul_colonne(tab,decalagei,decalagej + delt,0)==0)
	{
		delt++;
	}
	return(delt);
}


void premier_coeff_non_nul(int tab[(2*m)+1][m+1],int decalagei,int decalagej)
//modifie tab en échangeant lignes entre elles pour que le coef a_1_1 soit non nul.
{
	int ligne_pour_echange=1;
	while ( (tab[decalagei+0][decalagej+0]==0) && (ligne_pour_echange<=(2*m)) )
	{
		echanger_lignes(tab,0,ligne_pour_echange,decalagei,decalagej);
		ligne_pour_echange++;
	}
	if (ligne_pour_echange>(2*m))
		printf("Pivot de Gauss a trouvé une colonne vide\n");
}

void etape3(int tab[(2*m)+1][m+1],int decalagei,int decalagej,int p)
{
	int n;
	for(n=1;n<=( (2*m) -decalagei);n++)
	{
		operation_elementaire(tab,decalagei,decalagej,n,0, tab[n+decalagei][decalagej] ,p);
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

int test_nul_matrice(int tab[(2*m)+1][m+1],int decalagei,int decalagej)
//renvoie 0 si la matrice décalée est la matrice nulle, 1 sinon
{
	if (decalagej<(m-1))
	{
		int drapeau=0;
		int i;
		int j;
		for(i=1;i<=( (2*m) -decalagei);i++)
		{
			for(j=1;j<=(m-decalagej);j++)
			{
				if (tab[decalagei +i][decalagej +j]!=0)
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


void Pivot_Gauss_recurrence(int tab[(2*m)+1][m+1],int decalagei,int decalagej,int p,int *delta)
{
	*delta=premiere_colonne_non_nulle(tab,decalagei,decalagej);
	premier_coeff_non_nul(tab,decalagei,decalagej + *delta);
	multiplication_ligne(tab,decalagei,decalagej+ *delta,0, (inv_mod(tab[decalagei][decalagej + *delta],p)+p)%p ,p);
	etape3(tab,decalagei,decalagej + *delta,p);
	//printf("decalage = %d et test_nul_matrice = %d\n",decalage,test_nul_matrice(tab,decalage));
	if (test_nul_matrice(tab,decalagei,decalagej + *delta)==1)
		Pivot_Gauss_recurrence(tab,decalagei+1,decalagej+1 + *delta,p,delta);
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
				operation_elementaire(tab,0,0,k,i, tab[k][j] ,p);
			}
		}
	}
}

void Pivot_Gauss(int tab[(2*m)+1][m+1],int p)
//modifie tab et renvoie l'unique matrice échelonnée réduite équivalente.

// ATTENTION: RQ: DANS TOUTE CETTE FONCTION ON COMMENCERA PAR COMPTER LES LIGNES PAR 0
// ATTENTION: si on tombe sur colonne vide: sort segmentation fault
{
	int delta=0;
	Pivot_Gauss_recurrence(tab,0,0,p,&delta);
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

int verification_facteur_simple(int *fct)
//vérifie que les facteurs premiers de q sont bien de multiplicités 1
{
	int drapeau=1;
	int k=0;
	while(fct[(2*k)]!=0)
	{
		if (fct[(2*k)+1]!=1)
			drapeau=0;
		k++;
	}
	if (drapeau==0)
		printf("ATTENTION: p-1 N'A PAS UNE DECOMPOSITION EN FACTEURS SIMPLES\n");
	return(drapeau);
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
	prebase=creation_base(p);
	//prebase=creation_base_2(p,factorisation(p-1));
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


int p_convient(int *facteur,int p)
//vérifie que p-1 n'a que des facteurs simples
{
	int drapeau=1;
	int k=0;
	while(facteur[2*k]!=0)
	{
		if (est_premier(p,20)==0)
			drapeau=0;
		if (facteur[(2*k)+1]!=1)
			drapeau=0;
		k++;
	}
	return(drapeau);
}

int bon_resultat(int p,int alpha,int base[m][2])
{
	int drapeau=1;
	int k;
	for(k=0;k<m;k++)
	{
		if( square_and_multiply(base[k][1],alpha,p) != (base[k][0]%p) )
			drapeau=0;
	}
	return(drapeau);
}

void programme_test_preprocessing(int borneinf,int bornesup)
//test que la fonction preprocessing marche entre tous les nombres adéquats entre borneinf et bornesup
{
	int compteur_nb_premier=0;
	int compteur_algo_bon=0;
	int base[m][2];
	int p=borneinf;
	int *facteur;
	facteur=factorisation(p-1);
	int alpha;
	alpha=generateur(facteur,p);
	while(p<bornesup)
	{
		if (p_convient(facteur,p)==1)
		{
			compteur_algo_bon++;
			preprocessing(p,alpha,base);
			if (bon_resultat(p,alpha,base)==1)
				compteur_nb_premier++;
		}
		p++;
		free(facteur);
		facteur=factorisation(p-1);
		alpha=generateur(facteur,p);
	}
	printf("compteur = %d = %d",compteur_nb_premier,compteur_algo_bon);
	free(facteur);
}

void voir_test_coherent(int base[m][2],int alpha, int p)
// test que les log retournés par preprocessing sont bien ceux de la base
{
	int k;
	for(k=0;k<m;k++)
	{
		printf("%d ^ %d = %d = %d\n",alpha,base[k][1], square_and_multiply(base[k][1],alpha,p) ,base[k][0]);
	}
}

void test_classique(int p)
// test pour une valeur de preprocessing
{
	int base[m][2];
	int *facteur_simple;
	facteur_simple=factorisation(p-1);
	int alpha;
	alpha=generateur(facteur_simple,p);
	printf("alpha = %d\n",alpha);
	if (verification_facteur_simple(facteur_simple)==1)
	{
		preprocessing(p,alpha,base);
		afficher_tableau_classique(m,2,base,"base");
		voir_test_coherent(base,alpha,p);
	}
	free(facteur_simple);
	printf("jusqu'ici tout va bien");
}




// SEGMENTATION FAULT: pourrait etre que Pivot de Gauss est tombé sur une colonne vide.
// ATTENTION: p-1 doit n'avoir que des facteurs premiers simples.
// ATTENTION: m doit etre petit par rapport à p.

void main(int argc, char** argv)
{
	int p;
	p=atoi(argv[1]);
	int q;
	q=atoi(argv[2]);
	programme_test_preprocessing(p,q);
}