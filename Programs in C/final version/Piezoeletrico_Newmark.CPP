#include <stdio.h>
#include <io.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define  nGmax  	3    // numero de n�s
#define  nELE		2    // n�mero de elementos
#define  GL			4	 // n�mero de graus de liberdade por elemento
#define  NGL		6    // n�mero de graus de liberdade m�ximo sem o grau de liberdade do potencial
#define  NGLT		9    // n�mero de graus de liberdade totais
#define  GLP    	2    // n�mero de potencial por elemento
#define  GLPT    	3    // n�mero de potencial total = n�mero de graus de liberdade m�ximo do potencial
#define  CCT    	7    // n�mero de grau de liberdade desprezando as condi��es de contorno

int nNs; 				 //n�mero de n�s
int nEl;                 //n�mero de elementos
int gl;                  //n�mero de graus de liberdade
int nm, np, nt_Transiente;
long double dens;
long double densp;
long double aux;
long double a, b, c;
long double d, ef, fe, g;
long double h, h1, nh, mn;				      //auxiliares para c�lculo das matrizes M e K completas
long double t, t0, dt;							//tempo
long double dist;						//mesma fun��o que dist1 -> passo
long double Wo, W;
long double Wi, Wf, dW;
long double lp, ls; 							//tamanho do elemento analisado
long double PI;
long double tf;
long double gama, alfa;
long double a0, a1, a2, a3, a4, a5, a6, a7;
long double hs, hp, bs, bp;
long double MoIs, MoIp;
long double aes, aep;
long double e31, k33, c11;
int np_F1, np_F2, np_F3, np_F4, np_F5, np_F6;
int nbp;

//matriz massa estrutura para um elemento
long double Mls[GL][GL];
//matriz massa estrutura para n elemento, onde NGL � o numero m�ximo de grau de liberdades sem considerar os graus e liberdade dos potenciais.
long double Ms[NGL][NGL];

//matriz rigidez estrutura para um elemento
long double KEls[GL][GL];
//matriz rigidez estrutura para n elemento, onde NGL � o numero m�ximo de grau de liberdades sem considerar os graus e liberdade dos potenciais.
long double Ks[NGL][NGL];

//matriz massa piezo para um elemento
long double Mlp[GL][GL];
//matriz massa estrutura para n elemento, onde NGL � o numero m�ximo de grau de liberdades sem considerar os graus e liberdade dos potenciais.
long double Mp[NGL][NGL];

//matriz rigidez estrutura para um elemento
long double KElp[GL][GL];
//matriz rigidez estrutura para n elemento, onde NGL � o numero m�ximo de grau de liberdades sem considerar os graus e liberdade dos potenciais.
long double Kp[NGL][NGL];

//Forma��o da Matriz acoplamento u v por elemento
long double K_u_vp[GL][GLP];
//Forma��o da Matriz acoplamento total u v
long double K_u_v[NGL][GLPT];

//Forma��o da Matriz acoplamento v u por elemento
long double K_v_up[GLP][GL];
//Forma��o da Matriz acoplamento total v u
long double K_v_u[GLPT][NGL];

//Forma��o da Matriz acoplamento v v por elemento
long double K_v_vp[GLP][GLP];
//Forma��o da Matriz acoplamento total v v
long double K_v_v[GLPT][GLPT];

//soma da  massa da estrutura com matriz massa do piezo, onde NGL � o numero m�ximo de grau de liberdades sem considerar os graus e liberdade dos potenciais.
long double M[NGL][NGL];

//soma da  rigidez da estrutura com matriz rigidez do piezo, onde NGL � o numero m�ximo de grau de liberdades sem considerar os graus e liberdade dos potenciais.
long double K[NGL][NGL];

//Forma��o da Matriz massa incluindo os graus de liberdade relacionados com os potenciais, onde NGL2 = NGL + NGL/2.
long double M_G[NGLT][NGLT];

//Forma��o da Matriz RIGIDEZ incluindo os graus de liberdade relacionados com os potenciais, onde NGL2 = NGL + NGL/2.
long double K_G[NGLT][NGLT];

//Forma��o da Matriz AMORTECIMENTO incluindo os graus de liberdade relacionados com os potenciais, onde NGL2 = NGL + NGL/2.
long double C[NGLT][NGLT];

//Forma��o da Matriz massa incluindo os graus de liberdade relacionados com os potenciais, e diminuindo o numero e condi��es de contorno.
long double M_cc[CCT][CCT];

//Forma��o da Matriz rigidez incluindo os graus de liberdade relacionados com os potenciais, e diminuindo o numero e condi��es de contorno.
long double K_cc[CCT][CCT];

//Forma��o da Matriz amortecimento incluindo os graus de liberdade relacionados com os potenciais,e diminuindo o numero e condi��es de contorno.
long double C_cc[CCT][CCT];

//vetor for�amento aplicado as condi��es de contorno
long double fa[CCT];

//[P1] = a0*[M] + a1*[C] + [K]
long double P1[CCT][CCT];

//[P2a] = a0*[M] + a1*[C]
long double P2a[CCT][CCT];

//[P2] = [P2a] x [y]
long double P2[CCT];

//[P3a] = a2*[M] + a4*[C]
long double P3a[CCT][CCT];

//[P3] = [P3a] x [vel]
long double P3[CCT];

//[P4a] = a3*[M] + a5*[C]
long double P4a[CCT][CCT];

//[P4] = [P4a] x [acel]
long double P4[CCT];

//[P5] = [fa]*sin(W*t) + [P2] + [P3] + [P4]
long double P5[CCT];

long double y_equivalente[CCT];        //vetor delta y para calcular o resultado
long double delta_fa[CCT];             //[delta_fa] = [fa]i - [fa]i-1

long double K_Y[CCT];      		      //vetor produto [K] x [y]
long double C_Y[CCT];    	      	  //vetor produto [C] x [vel]
long double K_C_Y[CCT];                //[K_C_Y] = [fa] - [C_Y] - [K_Y]

// Vetor for�amento do corpo todo usado diretamente no m�todo 
long double f[NGLT];

// Vetor for�amento com os valores m�ximos
long double f_max[NGLT];

long double cc[NGLT], cc_p[nELE];      //vetor condi��es de contorno
long double Aes[nELE], Aep[nELE];	  //vetor �rea transversal de todos os elementos
long double mEs[nELE], mEp[nELE];      //vetor m�dulo de elasticidade de todos os elementos
long double Mos[nELE], Mop[nELE];      //vetor momento de in�rcia de todos os elementos

// Vetor deslocamento
long double y[CCT];

// Vetor velocidade
long double vel[CCT];

// Vetor acelera��o
long double acel[CCT];

// Vetor deslocamento anterior
long double y_ant[CCT];

// Vetor velocidade anterior
long double vel_ant[CCT];

// Vetor acelera��o anterior
long double acel_ant[CCT];

//[delta_y] = [y]i - [y]i-1
long double delta_y[21];

FILE* input;
FILE* output;
FILE* frequencia;

void LeituraDados(void)
{
	char ch;
	char abobrinhas[100];

	gl = GL / 2;
	nNs = nGmax;
	nEl = nNs - 1;

	input = fopen("dados_piezo.inp", "r");

	if (input == NULL)
	{
		printf("Arquivo nao encontrado ou nao existente!");
		return;
	}

	while ((ch = fgetc(input)) != EOF)
	{
		if (ch == '\n') break;
	}

	//	Numero de barras piezoel�trica
	fscanf(input, "%d", &nbp);

	ch = fgetc(input);
	while ((ch = fgetc(input)) != EOF)
	{
		if (ch == '\n') break;
	}

	// Permissividade el�trica e constante diel�trica
	fscanf(input, "%Lf %Lf\n", &e31, &k33);

	ch = fgetc(input);
	while ((ch = fgetc(input)) != EOF)
	{
		if (ch == '\n') break;
	}

	fscanf(input, "%Lf\n", &c11);

	ch = fgetc(input);
	while ((ch = fgetc(input)) != EOF)
	{
		if (ch == '\n') break;
	}

	// Dimens�o transversal da barra e do piezo
	fscanf(input, "%Lf %Lf %Lf %Lf %Lf %Lf\n", &bs, &hs, &bp, &hp, &lp, &ls);

	ch = fgetc(input);
	while ((ch = fgetc(input)) != EOF)
	{
		if (ch == '\n') break;
	}

	// Leitura do vetor cc do piezoletrico
	for (int i = 0; i < nEl; i++)
	{
		fscanf(input, "%Lf", &cc_p[i]);
	}

	aes = bs * hs;
	for (int i = 0; i < nEl; i++)      /*  AREA DA ESTRUTURA*/
	{
		Aes[i] = aes;
	}

	aep = bp * hp;
	for (int i = 0; i < nEl; i++)      /* AREA DO PIEZO*/
	{
		if (cc_p[i] == 0)
		{
			Aep[i] = 0;
		}
		else
		{
			Aep[i] = nbp * aep;
		}
	}

	ch = fgetc(input);
	while ((ch = fgetc(input)) != EOF)
	{
		if (ch == '\n') break;
	}

	// Modulo de elasticidade ESTRUTURA
	for (int i = 0; i < nEl; i++)
	{
		fscanf(input, "%Lf", &mEs[i]);
	}

	ch = fgetc(input);
	while ((ch = fgetc(input)) != EOF)
	{
		if (ch == '\n') break;
	}

	// Modulo de elasticidade PIEZO
	for (int i = 0; i < nEl; i++)
	{
		fscanf(input, "%Lf", &mEp[i]);
	}

	// Momento de in�rcia ESTRUTURA
	MoIs = bs * pow(hs, 3) / 12.0;
	for (int i = 0; i < nEl; i++)
	{
		Mos[i] = MoIs;
	}

	// Momento de in�rcia PIEZO
	MoIp = nbp * (bp * pow(hp, 3) / 12.0 + bp * hp * pow((hp + hs) / 2, 2));
	for (int i = 0; i < nEl; i++)
	{
		if (cc_p[i] == 0)
		{
			Mop[i] = 0;
		}
		else
		{
			Mop[i] = MoIp;
		}
	}

	ch = fgetc(input);
	while ((ch = fgetc(input)) != EOF)
	{
		if (ch == '\n') break;
	}

	// For�amento
	for (int i = 0; i < NGLT; i++)
	{
		fscanf(input, "%Lf", &f_max[i]);
	}

	ch = fgetc(input);
	while ((ch = fgetc(input)) != EOF)
	{
		if (ch == '\n') break;
	}

	// Leitura do vetor cc
	for (int i = 0; i < NGLT; i++)
	{
		fscanf(input, "%Lf", &cc[i]);
	}

	ch = fgetc(input);
	while ((ch = fgetc(input)) != EOF)
	{
		if (ch == '\n') break;
	}

	// Par�metros de tempo
	fscanf(input, "%Lf %d %d %d\n", &t0, &nm, &np, &nt_Transiente);
	/* nm = numero de divisoes por periodo
	   np = numero de periodos
	*/

	ch = fgetc(input);
	while ((ch = fgetc(input)) != EOF)
	{
		if (ch == '\n') break;
	}

	// Par�metros da frequ�ncia angular
	fscanf(input, "%Lf %Lf %Lf\n", &Wi, &Wf, &dW);

	// Fechamento do Arquivo de leitura
	fclose(input);
}
/*  ==================== Rotinas de execucao =====================*/

/***********************************************************************/
void MatrizRigidez_estrutura(void)
{
	/* montagem da matriz global */
	for (int i = 0; i < NGL; i++)
	{
		for (int j = 0; j < NGL; j++)
		{
			Ks[i][j] = 0;
		}
	}

	for (int n = 0; n < nEl; n++)
	{
		long double constante = Mos[n] * mEs[n] / pow(ls, 3);

		KEls[0][0] = 12 * constante;
		KEls[0][1] = 6 * ls * constante;
		KEls[0][2] = -12 * constante;
		KEls[0][3] = 6 * ls * constante;
		KEls[1][0] = 6 * ls * constante;
		KEls[1][1] = 4 * pow(ls, 2) * constante;
		KEls[1][2] = -(6 * ls * constante);
		KEls[1][3] = 2 * pow(ls, 2) * constante;
		KEls[2][0] = -(12 * constante);
		KEls[2][1] = -(6 * ls * constante);
		KEls[2][2] = 12 * constante;
		KEls[2][3] = -(6 * ls * constante);
		KEls[3][0] = 6 * ls * constante;
		KEls[3][1] = 2 * pow(ls, 2) * constante;
		KEls[3][2] = -(6 * ls * constante);
		KEls[3][3] = 4 * pow(ls, 2) * constante;

		for (int i = 2 * n; i < 2 * n + GL; i++)
		{
			for (int j = 2 * n; j < 2 * n + GL; j++)
			{
				Ks[i][j] += KEls[i - 2 * n][j - 2 * n];
			}
		}
	}
}
/***********************************************************************/
void MatrizRigidez_piezo(void)
{
	for (int i = 0; i < NGL; i++)
	{
		for (int j = 0; j < NGL; j++)
		{
			Kp[i][j] = 0;
		}
	}

	for (int n = 0; n < nEl; n++)
	{
		long double constante = Mop[n] * c11 / pow(lp, 3);

		if (cc_p[n] == 0)
		{
			for (int i = 0; i < GL; i++)
			{
				for (int j = 0; j < GL; j++)
				{
					KElp[i][j] = 0;
				}
			}
		}
		else
		{
			KElp[0][0] = 12 * constante;
			KElp[0][1] = 6 * lp * constante;
			KElp[0][2] = -12 * constante;
			KElp[0][3] = 6 * lp * constante;
			KElp[1][0] = 6 * lp * constante;
			KElp[1][1] = 4 * pow(lp, 2) * constante;
			KElp[1][2] = -(6 * lp * constante);
			KElp[1][3] = 2 * pow(lp, 2) * constante;
			KElp[2][0] = -(12 * constante);
			KElp[2][1] = -(6 * lp * constante);
			KElp[2][2] = 12 * constante;
			KElp[2][3] = -(6 * lp * constante);
			KElp[3][0] = 6 * lp * constante;
			KElp[3][1] = 2 * pow(lp, 2) * constante;
			KElp[3][2] = -(6 * lp * constante);
			KElp[3][3] = 4 * pow(lp, 2) * constante;
		}

		for (int i = 2 * n; i < 2 * n + GL; i++)
		{
			for (int j = 2 * n; j < 2 * n + GL; j++)
			{
				Kp[i][j] += KElp[i - 2 * n][j - 2 * n];
			}
		}
	}
}
/***********************************************************************/
void MatrizMassa_estrutura(void)
{
	/* montagem da matriz massa*/
	for (int i = 0; i < NGL; i++)
	{
		for (int j = 0; j < NGL; j++)
		{
			Ms[i][j] = 0;
		}
	}

	for (int n = 0; n < nEl; n++)
	{
		long double constante = Aes[n] * dens * ls / 420;

		Mls[0][0] = 156 * constante;
		Mls[0][1] = 22 * ls * constante;
		Mls[0][2] = 54 * constante;
		Mls[0][3] = -13 * ls * constante;
		Mls[1][0] = 22 * ls * constante;
		Mls[1][1] = 4 * pow(ls, 2) * constante;
		Mls[1][2] = 13 * ls * constante;
		Mls[1][3] = -3 * pow(ls, 2) * constante;
		Mls[2][0] = 54 * constante;
		Mls[2][1] = 13 * ls * constante;
		Mls[2][2] = 156 * constante;
		Mls[2][3] = -22 * ls * constante;
		Mls[3][0] = -13 * ls * constante;
		Mls[3][1] = -3 * pow(ls, 2) * constante;
		Mls[3][2] = -22 * ls * constante;
		Mls[3][3] = 4 * pow(ls, 2) * constante;

		for (int i = 2 * n; i < 2 * n + GL; i++)
		{
			for (int j = 2 * n; j < 2 * n + GL; j++)
			{
				Ms[i][j] += Mls[i - 2 * n][j - 2 * n];
			}
		}
	}
}
/***********************************************************************/
void MatrizMassa_piezo(void)
{
	/* montagem da matriz massa*/
	for (int i = 0; i < NGL; i++)
	{
		for (int j = 0; j < NGL; j++)
		{
			Mp[i][j] = 0;
		}
	}

	for (int n = 0; n < nEl; n++)
	{
		long double constante = Aep[n] * densp * lp / 420;

		if (cc_p[n] == 0)
		{
			for (int i = 0; i < GL; i++)
			{
				for (int j = 0; j < GL; j++)
				{
					Mlp[i][j] = 0;
				}
			}
		}
		else
		{
			Mlp[0][0] = 156 * constante;
			Mlp[0][1] = 22 * lp * constante;
			Mlp[0][2] = 54 * constante;
			Mlp[0][3] = -13 * lp * constante;
			Mlp[1][0] = 22 * lp * constante;
			Mlp[1][1] = 4 * pow(lp, 2) * constante;
			Mlp[1][2] = 13 * lp * constante;
			Mlp[1][3] = -3 * pow(lp, 2) * constante;
			Mlp[2][0] = 54 * constante;
			Mlp[2][1] = 13 * lp * constante;
			Mlp[2][2] = 156 * constante;
			Mlp[2][3] = -22 * lp * constante;
			Mlp[3][0] = -13 * lp * constante;
			Mlp[3][1] = -3 * pow(lp, 2) * constante;
			Mlp[3][2] = -22 * lp * constante;
			Mlp[3][3] = 4 * pow(lp, 2) * constante;
		}

		for (int i = 2 * n; i < 2 * n + GL; i++)
		{
			for (int j = 2 * n; j < 2 * n + GL; j++)
			{
				Mp[i][j] += Mlp[i - 2 * n][j - 2 * n];
			}
		}
	}
}
/***********************************************************************/
void soma_rigidez_massa(void)
{
	for (int i = 0; i < NGL; i++)
	{
		for (int j = 0; j < NGL; j++)
		{
			K[i][j] = Ks[i][j] + Kp[i][j];
			M[i][j] = Ms[i][j] + Mp[i][j];
		}
	}
}
/***********************************************************************/
void Matriz_acoplamento_eletromecanico_U_V(void)
{
	for (int i = 0; i < NGL; i++)
	{
		for (int j = 0; j < GLPT; j++)
		{
			K_u_v[i][j] = 0;
		}
	}
	for (int n = 0; n < nEl; n++)
	{
		long double constante = -e31 * bp * lp * (2 * hs * hp + hp * hp) / 2.0;

		if (cc_p[n] == 0)
		{
			for (int i = 0; i < GL; i++)
			{
				for (int j = 0; j < GLP; j++)
				{
					K_u_vp[i][j] = 0;
				}
			}
		}
		else
		{
			K_u_vp[0][0] = 0;
			K_u_vp[0][1] = 0;
			K_u_vp[1][0] = -constante * lp;
			K_u_vp[1][1] = constante * lp;
			K_u_vp[2][0] = 0;
			K_u_vp[2][1] = constante * lp;
			K_u_vp[3][0] = constante * lp;
			K_u_vp[3][1] = -constante * lp;
		}

		for (int i = 2 * n; i < 2 * n + GL; i++)
		{
			for (int j = n; j < n + GLP; j++)
			{
				K_u_v[i][j] += K_u_vp[i - 2 * n][j - n];
			}
		}
	}
}
/***********************************************************************/
void Matriz_acoplamento_eletromecanico_V_U(void)
{
	for (int i = 0; i < NGL; i++)
	{
		for (int j = 0; j < GLP; j++)
		{
			K_v_u[j][i] = K_u_v[i][j];
		}
	}
}
/***********************************************************************/
void Matriz_acoplamento_eletromecanico_V_V(void)
{
	for (int i = 0; i < GLPT; i++)
	{
		for (int j = 0; j < GLPT; j++)
		{
			K_v_v[i][j] = 0.0;
		}
	}

	for (int n = 0; n < nEl; n++)
	{
		long double constante = -k33 * Aep[n] * lp / (hp * hp);

		if (cc_p[n] == 0)
		{
			for (int i = 0; i < GLPT; i++)
			{
				for (int j = 0; j < GLPT; j++)
				{
					K_v_vp[i][j] = 0;
				}
			}
		}
		else
		{
			K_v_vp[0][0] = constante;
			K_v_vp[0][1] = -constante;
			K_v_vp[1][0] = -constante;
			K_v_vp[1][1] = constante;
		}

		for (int i = n; i < n + GLP; i++)
		{
			for (int j = n; j < n + GLP; j++)
			{
				K_v_v[i][j] += K_v_vp[i - n][j - n];
			}
		}
	}
}
/***********************************************************************/
void Matriz_M_geral(void)
{
	for (int i = 0; i < NGLT; i++)
	{
		for (int j = 0; j < NGLT; j++)
		{
			if (i < NGL && j < NGL)
			{
				M_G[i][j] = M[i][j];
			}
			else
			{
				M_G[i][j] = 0;
			}
		}
	}
}
/***********************************************************************/
void Matriz_K_geral(void)
{
	for (int i = 0; i < NGLT; i++)
	{
		for (int j = 0; j < NGLT; j++)
		{
			if (i < NGL && j < NGL)
			{
				K_G[i][j] = K[i][j];
			}
			else if (i < NGL && j >= NGL)
			{
				K_G[i][j] = K_u_v[i][j - NGL];
			}
			else if (i >= NGL && j < NGL)
			{
				K_G[i][j] = K_v_u[i - NGL][j];
			}
			else if (i >= NGL && j >= NGL)
			{
				K_G[i][j] = K_v_v[i - NGL][j - NGL];
			}
		}
	}
}
/***********************************************************************/
void MatrizC(void)
{
	double alfa = 1e-6;
	double mi = 0;

	/* montagem da matriz massa*/
	for (int i = 0; i < NGLT; i++)
	{
		for (int j = 0; j < NGLT; j++)
		{
			C[i][j] = mi * M_G[i][j] + alfa * K_G[i][j];
		}
	}
}
/***********************************************************************/
void CondicoesContorno(void)
{
	for (int i = 0; i < NGLT; i++)
	{
		if (cc[i] == 0)
		{
			for (int j = 0; j < NGLT; j++)
			{
				K_G[i][j] = 0;
				M_G[i][j] = 0;
				C[i][j] = 0;
			}
			for (int k = 0; k < NGLT; k++)
			{
				K_G[k][i] = 0;
				M_G[k][i] = 0;
				C[k][i] = 0;
			}
		}
	}

	int count1 = 0, count2 = 0;

	for (int i = 0; i < NGLT; i++)
	{
		count2 = 0;

		if (cc[i] == 1)
		{
			for (int j = 0; j < NGLT; j++)
			{
				if (cc[j] == 1)
				{
					M_cc[count1][count2] = M_G[j][i];
					K_cc[count1][count2] = K_G[j][i];
					C_cc[count1][count2] = C[j][i];

					count2 = count2 + 1;
				}
			}

			count1 += 1;
		}
	}
}
/***********************************************************************/
void CondicoesContorno1(void)
{
	int count1 = 0;

	for (int i = 0; i < NGLT; i++)
	{
		if (cc[i] == true)
		{
			fa[count1] = f[i];
			count1 += 1;
		}
	}
}
/***********************************************************************/
void RigidezEquivalente(void)
{
	for (int i = 0; i < CCT; i++)
	{
		for (int j = 0; j < CCT; j++)
		{
			P1[i][j] = a0 * M_cc[i][j] + a1 * C_cc[i][j] + K_cc[i][j];
		}
	}
}
/***********************************************************************/
void MatrizP2a(void)
{
	for (int i = 0; i < CCT; i++)
	{
		for (int j = 0; j < CCT; j++)
		{
			P2a[i][j] = a0 * M_cc[i][j] + a1 * C_cc[i][j];
		}
	}
}
/***********************************************************************/
void MatrizP3a(void)
{
	long double soma, soma1;

	for (int i = 0; i < CCT; i++)
	{
		for (int j = 0; j < CCT; j++)
		{
			P3a[i][j] = a2 * M_cc[i][j] + a4 * C_cc[i][j];
		}
	}
}
/***********************************************************************/
void MatrizP4a(void)
{
	for (int i = 0; i < CCT; i++)
	{
		for (int j = 0; j < CCT; j++)
		{
			P4a[i][j] = a3 * M_cc[i][j] + a5 * C_cc[i][j];
		}
	}
}
/***********************************************************************/
void MatrizP2(void)
{
	for (int i = 0; i < CCT; i++)
	{
		long double soma = 0;

		for (int j = 0; j < CCT; j++)
		{
			soma += P2a[i][j] * y_ant[j];
		}

		P2[i] = soma;
	}
}
/***********************************************************************/
void MatrizP3(void)
{
	for (int i = 0; i < CCT; i++)
	{
		long double soma = 0;

		for (int j = 0; j < CCT; j++)
		{
			soma += P3a[i][j] * vel_ant[j];
		}

		P3[i] = soma;
	}
}
/***********************************************************************/
void MatrizP4(void)
{
	long double soma;

	for (int i = 0; i < CCT; i++)
	{
		soma = 0;

		for (int j = 0; j < CCT; j++)
		{
			soma += P4a[i][j] * acel_ant[j];
		}

		P4[i] = soma;
	}
}
/***********************************************************************/
void MatrizP5(void)
{
	for (int i = 0; i < CCT; i++)
	{
		P5[i] = fa[i] + P2[i] + P3[i] + P4[i];
	}
}
/***********************************************************************/
void Elim_Gauss(void)
{
	long double P1inv[CCT][CCT];
    long double matriz[CCT][CCT];
	long double pivot = 0, p = 0, aux = 0;
	int i, j, k, l;

	for (int i = 0; i < CCT; i++)
	{
		for (int j = 0; j < CCT; j++)
		{
         matriz[i][j] = P1[i][j];
			if (i == j)
			{
				P1inv[i][j] = 1;
			}
			else
			{
				P1inv[i][j] = 0;
			}
		}
	}

	// Triangulariza��o
	for (int i = 0; i < CCT; i++)
	{
		pivot = matriz[i][i];

		for (int l = 0; l < CCT; l++)
		{
			matriz[i][l] = matriz[i][l] / pivot;
			P1inv[i][l] = P1inv[i][l] / pivot;
		}

		for (int k = i + 1; k < CCT; k++)
		{
			p = matriz[k][i];

			for (int j = 0; j < CCT; j++)
			{
				matriz[k][j] = matriz[k][j] - (p * matriz[i][j]);
				P1inv[k][j] = P1inv[k][j] - (p * P1inv[i][j]);
			}
		}
	}

	// Retrosubstitui��o
	for (int i = CCT - 1; i >= 0; i--)
	{
		for (int k = i - 1; k >= 0; k--)
		{
			p = matriz[k][i];

			for (int j = CCT - 1; j >= 0; j--)
			{
				matriz[k][j] = matriz[k][j] - (p * matriz[i][j]);
				P1inv[k][j] = P1inv[k][j] - (p * P1inv[i][j]);
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		long double aux = 0;

		for (int j = 0; j < 4; j++)
		{
			aux += P1inv[i][j] * P5[j];
		}

		y_equivalente[i] = aux;
	}
}
/********************************************************************/
void ImprimirMatrizesEquivalentes(void)
{
	FILE* matrizes_resultantes;

	matrizes_resultantes = fopen("matrizes_resultantes.txt", "w");

	// Matriz rigidez equivalente
	fprintf(matrizes_resultantes, "P1: {");
	for (int i = 0; i < CCT; i++)
	{
		fprintf(matrizes_resultantes, "{");
		for (int j = 0; j < CCT; j++)
		{
			fprintf(matrizes_resultantes, "%Lg, ", P1[i][j]);
		}
		fprintf(matrizes_resultantes, "},");
	}
	fprintf(matrizes_resultantes, "}");

	// Matriz for�a equivalente
	fprintf(matrizes_resultantes, "\nP5: {");
	for (int i = 0; i < CCT; i++)
	{
		fprintf(matrizes_resultantes, "%Lg, ", P5[i]);
	}
	fprintf(matrizes_resultantes, "}");

	fclose(matrizes_resultantes);
}
/***********************************************************************/

void ImprimirResultados(void)
{
	FILE* resultados;

	resultados = fopen("matrizes_resultantes2.txt", "w");

	// Matriz descolamento
	fprintf(resultados, "\ny: {");
	for (int i = 0; i < CCT; i++)
	{
		fprintf(resultados, "%Lf, ", y[i]);
	}
	fprintf(resultados, "}");

	// Matriz velocidade
	fprintf(resultados, "\nvel: {");
	for (int i = 0; i < CCT; i++)
	{
		fprintf(resultados, "%Lf, ", vel[i]);
	}
	fprintf(resultados, "}");

	// Matriz acelera��o
	fprintf(resultados, "\nacel: {");
	for (int i = 0; i < CCT; i++)
	{
		fprintf(resultados, "%Lf, ", acel[i]);
	}
	fprintf(resultados, "}");

	fclose(resultados);
}

/***********************************************************************/
void solucao(void)
{
	long double z = 0;
	int cont1 = 0;

	for (int jp = 0; jp < np; jp++)
	{
		for (int jn = 0; jn < nm; jn++)
		{
			for (int i = 0; i < NGLT; i++)
			{
				f[i] = f_max[i] * cos(W * t);
			}

			CondicoesContorno1();

			if (jp > 0)
			{
				if (W == 0.5)
				{
					fprintf(output, "%Lf\t %Lf\t", W, t);

					for (int i = 0; i < CCT; i++)
					{
						fprintf(output, "%Lf\f", y[i]);
					}

					fprintf(output, "\n");
				}
			}

			MatrizP2();
			MatrizP3();
			MatrizP4();
			MatrizP5();

			ImprimirMatrizesEquivalentes();

			Elim_Gauss();

			for (int i = 0; i < CCT; i++)
			{
				y[i] = y_equivalente[i];
			}

			for (int i = 0; i < CCT; i++)
			{
				acel[i] = a0 * (y[i] - y_ant[i]) - a2 * vel_ant[i] - a3 * acel_ant[i];
			}

			for (int i = 0; i < CCT; i++)
			{
				vel[i] = vel_ant[i] + a6 * acel_ant[i] + a7 * acel[i];
			}

			ImprimirResultados();

			t = t + dt;

			for (int i = 0; i < CCT; i++)
			{
				y_ant[i] = y[i];
				vel_ant[i] = vel[i];
				acel_ant[i] = acel[i];
			}
		}
	}

	fprintf(frequencia, "%Lg\t %Lg\n", W, z);
}

/***************************************************************/

void ImprimirMatrizesResultantes(void)
{
	FILE* matrizes_resultantes;

	matrizes_resultantes = fopen("matrizes_resultantes.txt", "w");

	// Matriz rigidez equivalente
	fprintf(matrizes_resultantes, "P1: {");
	for (int i = 0; i < CCT; i++)
	{
		fprintf(matrizes_resultantes, "{");
		for (int j = 0; j < CCT; j++)
		{
			fprintf(matrizes_resultantes, "%Lf, ", P1[i][j]);
		}
		fprintf(matrizes_resultantes, "},");
	}
	fprintf(matrizes_resultantes, "}");

	// Matriz for�a equivalente
	fprintf(matrizes_resultantes, "P5: {");
	for (int i = 0; i < CCT; i++)
	{
		fprintf(matrizes_resultantes, "%Lf, ", P5[i]);
	}
	fprintf(matrizes_resultantes, "}");

	// Matriz descolamento
	fprintf(matrizes_resultantes, "\ny: {");
	for (int i = 0; i < CCT; i++)
	{
		fprintf(matrizes_resultantes, "%Lf, ", y[i]);
	}
	fprintf(matrizes_resultantes, "}");

	// Matriz velocidade
	fprintf(matrizes_resultantes, "\nvel: {");
	for (int i = 0; i < CCT; i++)
	{
		fprintf(matrizes_resultantes, "%Lf, ", vel[i]);
	}
	fprintf(matrizes_resultantes, "}");

	// Matriz acelera��o
	fprintf(matrizes_resultantes, "\nacel: {");
	for (int i = 0; i < CCT; i++)
	{
		fprintf(matrizes_resultantes, "%Lf, ", acel[i]);
	}
	fprintf(matrizes_resultantes, "}");

	fclose(matrizes_resultantes);
}

/***************************************************************/

void ImprimirMatrizes(void)
{
	FILE* matrizes;

	matrizes = fopen("matrizes.txt", "w");

	// Matriz rigidez de um elemento da estrutura
	fprintf(matrizes, "KEls: {");
	for (int i = 0; i < GL; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < GL; j++)
		{
			fprintf(matrizes, "%Lf, ", KEls[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz rigidez da estrutura
	fprintf(matrizes, "\nKs: {");
	for (int i = 0; i < NGL; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < NGL; j++)
		{
			fprintf(matrizes, "%Lf, ", Ks[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz massa de um elemento da estrutura
	fprintf(matrizes, "\nMls: {");
	for (int i = 0; i < GL; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < GL; j++)
		{
			fprintf(matrizes, "%Lf, ", Mls[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz massa da estrutura
	fprintf(matrizes, "\nMs: {");
	for (int i = 0; i < NGL; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < NGL; j++)
		{
			fprintf(matrizes, "%Lf, ", Ms[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz rigidez de um elemento do piezoel�trico
	fprintf(matrizes, "\nKElp: {");
	for (int i = 0; i < GL; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < GL; j++)
		{
			fprintf(matrizes, "%Lf, ", KElp[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz rigidez do piezoel�trico
	fprintf(matrizes, "\nKp: {");
	for (int i = 0; i < NGL; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < NGL; j++)
		{
			fprintf(matrizes, "%Lf, ", Kp[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz massa de um elemento do piezoel�trico
	fprintf(matrizes, "\nMlp: {");
	for (int i = 0; i < GL; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < GL; j++)
		{
			fprintf(matrizes, "%Lf, ", Mlp[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz massa do piezoel�trico
	fprintf(matrizes, "\nMp: {");
	for (int i = 0; i < NGL; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < NGL; j++)
		{
			fprintf(matrizes, "%Lf, ", Mp[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz massa
	fprintf(matrizes, "\nK: {");
	for (int i = 0; i < NGL; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < NGL; j++)
		{
			fprintf(matrizes, "%Lf, ", K[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz rigidez
	fprintf(matrizes, "\nM: {");
	for (int i = 0; i < NGL; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < NGL; j++)
		{
			fprintf(matrizes, "%Lf, ", M[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz acoplamento eletromec�nico de um elemento do piezoel�trico
	fprintf(matrizes, "\nK_u_vp: {");
	for (int i = 0; i < GL; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < GLP; j++)
		{
			fprintf(matrizes, "%Lg, ", K_u_vp[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz acoplamento eletromec�nico do piezoel�trico
	fprintf(matrizes, "\nK_u_v: {");
	for (int i = 0; i < NGL; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < GLPT; j++)
		{
			fprintf(matrizes, "%Lg, ", K_u_v[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz capacit�ncia de um elemento do piezoel�trico
	fprintf(matrizes, "\nK_v_vp: {");
	for (int i = 0; i < GLP; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < GLP; j++)
		{
			fprintf(matrizes, "%Lg, ", K_v_vp[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz capacit�ncia do piezoel�trico
	fprintf(matrizes, "\nK_v_v: {");
	for (int i = 0; i < GLPT; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < GLPT; j++)
		{
			fprintf(matrizes, "%Lg, ", K_v_v[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz massa geral
	fprintf(matrizes, "\nK_G: {");
	for (int i = 0; i < NGLT; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < NGLT; j++)
		{
			fprintf(matrizes, "%Lg, ", K_G[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz rigidez geral
	fprintf(matrizes, "\nM_G: {");
	for (int i = 0; i < NGLT; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < NGLT; j++)
		{
			fprintf(matrizes, "%Lg, ", M_G[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz amortecimento
	fprintf(matrizes, "\nC: {");
	for (int i = 0; i < NGLT; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < NGLT; j++)
		{
			fprintf(matrizes, "%Lg, ", C[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz amortecimento
	fprintf(matrizes, "\nM_cc: {");
	for (int i = 0; i < CCT; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < CCT; j++)
		{
			fprintf(matrizes, "%Lg, ", M_cc[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz amortecimento
	fprintf(matrizes, "\nK_cc: {");
	for (int i = 0; i < CCT; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < CCT; j++)
		{
			fprintf(matrizes, "%Lg, ", K_cc[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	// Matriz amortecimento
	fprintf(matrizes, "\nC_cc: {");
	for (int i = 0; i < CCT; i++)
	{
		fprintf(matrizes, "{");
		for (int j = 0; j < CCT; j++)
		{
			fprintf(matrizes, "%Lg, ", C_cc[i][j]);
		}
		fprintf(matrizes, "},");
	}
	fprintf(matrizes, "}");

	fclose(matrizes);
}
/*******************************************************************/
void ImprimirConstantesDeIntegracao(void)
{
	FILE* constantes;

	constantes = fopen("constantes-de-integracao.txt", "w");

	fprintf(constantes, "a0 = %Lg", a0);
	fprintf(constantes, "\na1 = %Lg", a1);
	fprintf(constantes, "\na2 = %Lg", a2);
	fprintf(constantes, "\na3 = %Lg", a3);
	fprintf(constantes, "\na4 = %Lg", a4);
	fprintf(constantes, "\na5 = %Lg", a5);
	fprintf(constantes, "\na6 = %Lg", a6);
	fprintf(constantes, "\na7 = %Lg", a7);

	fclose(constantes);
}

/***********************************************************************/
/* Programa Principal */
main()
{
	PI = 4 * atan(1);            /*numero PI*/
	alfa = 1.0 / 4.0;
	gama = 1.0 / 2.0;
	dens = 7850;
	densp = 7650;
	LeituraDados();

	output = fopen("LINEAR.dat", "w");			/* external filename */
	frequencia = fopen("freq.dat", "w");

	// Matrizes Massa, Rigidez e Amortecimento
	MatrizRigidez_estrutura();
	MatrizRigidez_piezo();
	MatrizMassa_estrutura();
	MatrizMassa_piezo();
	soma_rigidez_massa();
	Matriz_acoplamento_eletromecanico_U_V();
	Matriz_acoplamento_eletromecanico_V_U();
	Matriz_acoplamento_eletromecanico_V_V();
	Matriz_M_geral();
	Matriz_K_geral();
	MatrizC();

	CondicoesContorno();

	ImprimirMatrizes();

	int nbif = (int)((Wf - Wi) / dW) + 1;

	for (int jb = 0; jb < nbif; jb++)
	{
		W = Wi + (jb * dW);
		printf("W = %g\n", W);

		if (W != 0)
		{
			dt = ((PI * 2 / W) / nm);
		}
		else
		{
			dt = (PI * 2 / nm);
		}

		if (t == 0)
		{
			for (int i = 0; i < CCT; i++)
			{
				y[i] = y_ant[i] = 0;
				vel[i] = vel_ant[i] = 0;
				acel[i] = acel_ant[i] = 0;
			}
		}

		a0 = 1.0 / (alfa * dt * dt);
		a1 = gama / (alfa * dt);
		a2 = 1.0 / (alfa * dt);
		a3 = (1 / (2 * alfa)) - 1;
		a4 = (gama / alfa) - 1;
		a5 = (dt / 2) * ((gama / alfa) - 2);
		a6 = dt * (1 - gama);
		a7 = gama * dt;

		ImprimirConstantesDeIntegracao();

		RigidezEquivalente();
		MatrizP2a();
		MatrizP3a();
		MatrizP4a();

		solucao();
	}

	fclose(output);
	fclose(frequencia);
}
