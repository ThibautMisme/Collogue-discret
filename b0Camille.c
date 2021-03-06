#include<stdio.h>
#include<math.h>
#include"theoreme_chinois.h"



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



int est_generateur (int tableau[3], int candidat, int p)

/*On suppose que le tableau contient les facteurs premiers de p-1 et qu'il se termine par au moins un zero.*/
{
	int i;
	int d;
	int ordre_du_groupe; 
	int inter;

	ordre_du_groupe=p-1;
	for(i=0;i<10;i++)
/* On considère pour l'instant que p-1 n'a pas plus de 10 facteurs premiers dans sa decomposition*/
	{
		if (tableau[i]==0)
			return 1;
		d=ordre_du_groupe/tableau[i];
		if (square_and_multiply_rec(candidat,d,p)==1)
			break;
	}
	return 0;

}

int generateur (int tableau[3], int p)
{
	int candidat;
	candidat=1;
	int imax;
	int i;
	i=0;
	imax=100;
	while (((est_generateur(tableau,candidat,p))==0)&&(i<imax))
	{
		candidat=candidat+1;
		i=i+1;
	}
	return candidat;
}


int premier_coeffb0 (int beta,int p,int rho_puissance,int rho, int gamma)
 /*a ce stade, cette fonction calcule bo pour un certain facteur rho lorsque sa puissance dans la factorisation de p-1 est 1
 rho est un élémént de tableau car un diviseur de p-1*/
{
	//int gamma;
	int gammapuissance;
	int betapuissance;
	int b0;
	int i;
	int imax;

	imax=p+1;
	i=1;

	// /!\ ATTENTION : ici on appelle rho_puissance ce qui vaut en réalité dans la fonction //
	// coef_bj rho*rho_puissance !!!!!! //
	betapuissance=square_and_multiply_rec(beta,((p-1)/rho_puissance),p);



	//gamma=square_and_multiply_rec(generateur,((p-1)/rho),p);
	gammapuissance=gamma;
	while (((gammapuissance%p)!=betapuissance)&&(i<imax))
	{
		gammapuissance=(gammapuissance*gamma)%p;
		/*printf("%d\n",betapuissance);
		/*printf("%d\n",gamma);*/
		i=i+1;
	}


	b0=i%rho;
	return b0;
}

int coef_bj(int rho, int beta, int generateur, int p)
{
	int new_bj;
	int x=0;
	int rho_puissance = 1;

	int gamma;
	gamma = square_and_multiply_rec(alpha,((p-1)/rho),p);

	int inv_generateur = inv_mod(generateur,p);


	while((p-1) % (rho*rho_puissance) == 0)
	{	 

		// /!\ ATTENTION: premier_coeffb0 prend en argument le nombre rho*rho_puissance //
		// qui sera stocké dans une variable locale dénommée rho_puissance //
		new_bj = premier_coeffb0(beta, p, rho*rho_puissance,rho,gamma);

		x = x + new_bj * rho_puissance;

		beta = beta * square_and_multiply_rec(inv_generateur,new_bj*rho_puissance,p);

		rho_puissance = rho_puissance * rho;

	}

	return x;
} 
