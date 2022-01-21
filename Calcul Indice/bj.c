#include<stdio.h>
#include<math.h>
#include"theoreme_chinois.h"
#include"factorisation_v3.h"



void Euclide_Etendu(int a, int b, int *x, int *y, int *pgcd) 

{

/*on suppose que a et b ne sont pas simultanement nuls.*/

int tempx1 , tempx2 , tempy1 , tempy2 , tempor ; 
int temp1;
int temp2;

tempx1=1; 
tempx2=0; 
tempy1=0; 
tempy2=1;

while (b!=0)
{
	temp1=a/b;
	temp2=a%b;

	a=b;
	b=temp2;

	tempor=tempx2;
	tempx2=tempx1-(temp1)*tempx2;
	tempx1=tempor;

	tempor=tempy2;
	tempy2=tempy1-(temp1)*tempy2;
	tempy1=tempor;	
}

*x=tempx1;
*y=tempy1;
*pgcd=a;

}


int square_and_multiply_rec(int a, int d, int n)
{
	int temp;
	if (d==0)
		return 1;
	temp=square_and_multiply_rec((a*a)%n,d>>1,n);
	return ((d&0x1)==0)? temp: (temp*a)%n;
}



int est_generateur (int *tableau, int candidat, int p)

/*On suppose que le tableau contient les facteurs premiers de p-1 et qu'il se termine par au moins un zero.*/
{
	int drapeau=1;
	int i;
	int d;
	int ordre_du_groupe; 
	int inter;
	int taille;
	taille=calcul_taille(tableau)/2;

	ordre_du_groupe=p-1;
	for(i=0;i<taille;i++)
	{
		d=ordre_du_groupe/tableau[2*i];
		if (square_and_multiply(d,candidat,p)==(1%p))
			drapeau=0;
	}
	return drapeau;

}

int generateur (int *tableau, int p)
{
	int candidat;
	candidat=2;
	int imax;
	int i;
	i=0;
	imax=100;
	while ( (est_generateur(tableau,candidat,p)==0)&&(i<imax) )
	{
		candidat=candidat+1;
		i=i+1;
	}
	return (candidat%p);
}


int premier_coeffb0 (int generateur, int beta,int p,int rho)
 /*a ce stade, cette fonction calcule bo pour un certain facteur rho lorsque sa puissance dans la factorisation de p-1 est 1
 rho est un élémént de tableau car un diviseur de p-1*/
{
	int gamma;
	int gammapuissance;
	int betapuissance;
	int b0;
	int i;
	int imax;
	imax=p+1;
	i=1;
	betapuissance=square_and_multiply_rec(beta,((p-1)/rho),p);
	gamma=square_and_multiply_rec(generateur,((p-1)/rho),p);
	gammapuissance=gamma;
	while (((gammapuissance%p)!=betapuissance)&&(i<imax))
	{
		gammapuissance=(gammapuissance*gamma)%p;
		/*printf("%d\n",betapuissance);
		/*printf("%d\n",gamma);*/
		i=i+1;
	}
	b0=i;
	return b0;
}

int coef_bj(int rho, int beta, int generateur, int q)
{
	int new_b0;
	int new_bj;
	int cpt=0;
	int x=0;
	int p=q+1;
	int inv_generateur;
	inv_generateur=inv_mod(generateur,p);


	while(q % square_and_multiply_rec(rho,cpt,q) == 0)
	{	 

		new_bj = premier_coeffb0(generateur,beta, p, rho);

		x += new_bj * square_and_multiply_rec(rho,cpt,q);


		// /!\ attention ici le square and multiply //
		// se fait dans Z/pZ et non Z/(p-1)Z //
		beta = beta * square_and_multiply_rec(inv_generateur,new_b0,p);

		cpt++;

	}

	return x;
} 
