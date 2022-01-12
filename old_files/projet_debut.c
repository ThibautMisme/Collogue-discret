#include<stdio.h>
#include<math.h>



void Euclide_Etendu(unsigned int a, unsigned int b, int *x, int *y, int *pgcd) 

{

/*on suppose que a et b ne sont pas simultanement nuls.*/

unsigned int tempx1 , tempx2 , tempy1 , tempy2 , tempor ; 
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


unsigned int square_and_multiply_rec(unsigned int a, unsigned int d, unsigned int n)
{
	unsigned int temp;
	if (d==0)
		return 1;
	temp=square_and_multiply_rec((a*a)%n,d>>1,n);
	return ((d&0x1)==0)? temp: (temp*a)%n;
}



unsigned int est_generateur (unsigned int tableau[3], unsigned int candidat, unsigned int p)

/*On suppose que le tableau contient les facteurs premiers de p-1 et qu'il se termine par au moins un zero.*/
{
	unsigned int i;
	unsigned int d;
	unsigned int ordre_du_groupe; 
	unsigned int inter;

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

unsigned int generateur (unsigned int tableau[3], unsigned int p)
{
	unsigned int candidat;
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


unsigned int premier_coeffb0 (unsigned int beta,unsigned int p,unsigned int rho,unsigned int tableau[3])
 /*a ce stade, cette fonction calcule bo pour un certain facteur rho lorsque sa puissance dans la factorisation de p-1 est 1
 rho est un élémént de tableau car un diviseur de p-1*/
{
	unsigned int gamma;
	unsigned int alpha;
	unsigned int betapuissance;
	unsigned int b0;
	unsigned int i;
	unsigned int imax;
	imax=100;
	i=1;
	alpha= generateur(tableau,p);
	betapuissance=beta^((p-1)/rho);
	gamma=alpha^((p-1)/rho);
	while ((gamma!=betapuissance)&&(i<imax));
	{
		gamma=gamma*gamma;
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
	

	unsigned int tableau[3];
	unsigned int candidat;
	unsigned int p;
	unsigned int beta;

	tableau[0]=2;
	tableau[1]=5;
	tableau[2]=0;
	p=11;
	beta=5;
	
	/*candidat=2;
	printf("%d\n",est_generateur(tableau,candidat,p));

	printf("%d\n",generateur(tableau,p)); */

	int b00;
	int b01;

	b00=premier_coeffb0(beta,p,tableau[0],tableau);
	b01=premier_coeffb0(beta,p,tableau[1],tableau);

	printf("%d\n",premier_coeffb0(beta,p,tableau[0],tableau));
	printf("%d\n",premier_coeffb0(beta,p,tableau[1],tableau));

	/*int u;
	int v;
	int pgcd;

	Euclide_Etendu(tableau[0],tableau[1],&u,&v,&pgcd);
	printf(" les valeurs de u et v sont respectivement %d et %d\n", u,v);

	int x;
	x=tableau[0]*b01*u+b00*tableau[1]*v;
	printf("la valeur de x est %d", x);*/








	
}

