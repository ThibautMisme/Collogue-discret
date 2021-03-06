#include<stdio.h>
#include<math.h>



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


int premier_coeffb0 (int generateur, int beta,int p,int rho,int tableau[3])
 /*a ce stade, cette fonction calcule bo pour un certain facteur rho lorsque sa puissance dans la factorisation de p-1 est 1
 rho est un élémént de tableau car un diviseur de p-1*/
{
	int gamma;
	int betapuissance;
	int b0;
	int i;
	int imax;
	imax=p+1;
	i=1;
	betapuissance=square_and_multiply_rec(beta,((p-1)/rho),p);
	gamma=square_and_multiply_rec(generateur,((p-1)/rho),p);
	while (((gamma%p)!=betapuissance)&&(i<imax))
	{
		gamma=(gamma*gamma)%p;
		printf("%d\n",betapuissance);
		/*printf("%d\n",gamma);*/
		i=i+1;
	}
	b0=i;
	return b0;

}




int main()
{/*
	int a=5;
	int b=7;
	int u;
	int v;
	int pgcd;
	Euclide_Etendu(a,b,&u,&v,&pgcd);
	printf(" les valeurs de u et v sont respectivement %d et %d\n", u,v);
	printf(" le pgcd vaut %d\n", pgcd); 
	 
	printf("%u\n", square_and_multiply_rec(2,3,3)); */
	

	int tableau[3];
	int candidat;
	int p;
	int beta;
	int alpha;

	tableau[0]=2;
	tableau[1]=5;
	tableau[2]=0;
	p=11;
	beta=5;
	alpha=generateur(tableau,p);
	
	/*candidat=2;
	printf("%d\n",est_generateur(tableau,candidat,p));

	printf("%d\n",generateur(tableau,p)); */

	int b00;
	int b01;

	b00=premier_coeffb0(alpha,beta,p,tableau[0],tableau);
	b01=premier_coeffb0(alpha,beta,p,tableau[1],tableau);

	printf("la valeur de b0 pour le premier coeff est %d\n",b00);
	printf("la valeur de b0 pour le deuxieme coeff est %d\n",b01);
	/*printf("%d\n",alpha);*/

	int u;
	int v;
	int pgcd;

	Euclide_Etendu(tableau[0],tableau[1],&u,&v,&pgcd);
	printf("les valeurs de u et v sont respectivement %d et %d\n", u,v);

	int x;
	x=(tableau[0]*b01*u+b00*tableau[1]*v)%p;
	printf("la valeur de x est %d\n", x);

	/*printf("%d\n",square_and_multiply_rec(5,2,11));*/








	
}

