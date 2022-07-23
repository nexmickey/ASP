#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MaxN 1000

int main()
{
	_Bool izlaz = 1, kraj = 1, napravljenaPrvaMatrica = 0, napravljenaDrugaMatrica = 0;
	int n, m, el, x, n1, m1, el1, elz, index = 0, x1, y1, vrednost, r1, c1, c2, br1, br2, brNe0;
	int* R = NULL, * C = NULL, * V = NULL, * R1 = NULL, * C1 = NULL, * V1 = NULL, * RZ = NULL, * CZ = NULL, * VZ = NULL;

	while (izlaz)
	{
		printf("CSR ALGORITAM ZA RAD SA RETKIM MATRICAMA\n");
		printf("\n");
		printf("1. UNOS MATRICE\n");
		printf("2. ISPIS ELEMENTA MATRICE\n");
		printf("3. ISPIS CELE MATRICE\n");
		printf("4. ISPIS CSR VEKTORA\n");
		printf("5. UNOS DRUGE MATRICE\n");
		printf("6. ISPIS ZBIRA DVE MATRICE\n");
		printf("7. ISPIS ZBIRNOG CSR VEKTORA\n");
		printf("0. IZLAZ IZ PROGRAMA\n");
		printf("\n");
		printf("Izaberite opciju: ");
		int opcija;
		scanf("%d", &opcija);

		switch (opcija)
		{
		case 1:
			napravljenaPrvaMatrica = 1;
			printf("Broj vrsta matrice: "); scanf("%d", &m);
			printf("Broj kolona matrice: "); scanf("%d", &n);
			printf("Broj nepodrazumevanih elemenata matrice: "); scanf("%d", &el);
			if (m <= 1 || n <= 1 || el<1)
			{
				printf("Pogresan unos!\n");
				break;
			}
			R = (int*)realloc(R, (m + 1) * sizeof(int));
			C = (int*)realloc(C, el * sizeof(int));
			V = (int*)realloc(V, el * sizeof(int));
			index = 0;
			for (int i = 0; i < m; i++)
			{
				R[i] = index;
				for (int j = 0; j < n; j++)
				{
					scanf("%d", &x);
					if (x != 0)
					{
						C[index] = j;
						V[index] = x;
						index++;
					}
				}
			}
			R[m] = el;
			break;
		case 2:
			if (napravljenaPrvaMatrica == 0)
			{
				printf("Nije uneta prva matrica!\n");
				break;
			}
			printf("Vrednost vrste matrice(1 do x): "); scanf("%d", &x1);
			printf("Vrednost kolone matrice(1 do y): "); scanf("%d", &y1);
			x1--; y1--;
			vrednost = 0;
			for (int j = R[x1]; j < R[x1 + 1]; j++)
				if (C[j] == y1)
				{
					vrednost = V[j];
					break;
				}
			printf("%d\n", vrednost);
			break;
		case 3:
			if (napravljenaPrvaMatrica == 0)
			{
				printf("Nije uneta prva matrica!\n");
				break;
			}
			c1 = 0;
			for (int i = 0; i < m; i++)
			{
				for (int j = 0, brNe0 = R[i + 1] - R[i]; j < n; j++)
					if (brNe0 > 0 && j == C[c1])
					{
						printf("%d ", V[c1++]);
						brNe0--;
					}
					else
						printf("%d ", 0);
				printf("\n");
			}
			break;
		case 4:
			if (napravljenaPrvaMatrica == 0)
			{
				printf("Nije uneta prva matrica!\n");
				break;
			}
			printf("R: ");
			for (int i = 0; i <= m; i++)
				printf("%d ", R[i]+1);
			printf("\n");

			printf("C: ");
			for (int i = 0; i < el; i++)
				printf("%d ", C[i]+1);
			printf("\n");

			printf("V: ");
			for (int i = 0; i < el; i++)
				printf("%d ", V[i]);
			printf("\n");
			break;
		case 5:
			if (napravljenaPrvaMatrica == 0)
			{
				printf("Nije uneta prva matrica!\n");
				break;
			}
			printf("Broj vrsta druge matrice: "); scanf("%d", &m1);
			printf("Broj kolona druge matrice: "); scanf("%d", &n1);
			if (m1 != m && n1 != n)
			{
				printf("Matrice se ne mogu sabrati!\n");
				break;
			}
			printf("Broj nepodrazumevanih elemenata matrice: "); scanf("%d", &el1);
			R1 = (int*)realloc(R1, (m1 + 1) * sizeof(int));
			C1 = (int*)realloc(C1, el1 * sizeof(int));
			V1 = (int*)realloc(V1, el1 * sizeof(int));
			napravljenaDrugaMatrica = 1;
			index = 0;
			for (int i = 0; i < m1; i++)
			{
				R1[i] = index;
				for (int j = 0; j < n1; j++)
				{
					scanf("%d", &x);
					if (x != 0)
					{
						C1[index] = j;
						V1[index] = x;
						index++;
					}
				}
			}
			R1[m] = el1;
			printf("\n");

			RZ = (int*)realloc(RZ, (m1 + 1) * sizeof(int));
			CZ = (int*)realloc(CZ, el + el1 * sizeof(int));
			VZ = (int*)realloc(VZ, el + el1 * sizeof(int));

			c1 = 0; c2 = 0; br1 = 0; br2 = 0;
			index = 0;
			for (int i = 0; i < m1; i++)
			{
				br1 = R[i + 1] - R[i];
				br2 = R1[i + 1] - R1[i];
				RZ[i] = index;
				for (int j = 0; j < n1; j++)
				{
					x = 0;
					if (br1 > 0 && j == C[c1])
					{
						x += V[c1++];
						br1--;
					}
					if (br2 > 0 && j == C1[c2])
					{
						x += V1[c2++];
						br2--;
					}
					if (x != 0)
					{
						CZ[index] = j;
						VZ[index] = x;
						index++;
					}
				}
			}
			RZ[m] = index;
			elz = index;
			break;
		case 6:
			if (!napravljenaPrvaMatrica || !napravljenaDrugaMatrica)
			{
				printf("Nisu unete dve matrice!\n");
				break;
			}
			c1 = 0;
			for (int i = 0; i < m; i++)
			{
				for (int j = 0, brNe0 = RZ[i + 1] - RZ[i]; j < n; j++)
					if (brNe0 > 0 && j == CZ[c1])
					{
						printf("%d ", VZ[c1++]);
						brNe0--;
					}
					else
						printf("%d ", 0);
				printf("\n");
			}
			break;
		case 7:
			if (!napravljenaPrvaMatrica || !napravljenaDrugaMatrica)
			{
				printf("Nisu unete dve matrice!\n");
				break;
			}
			printf("R: ");
			for (int i = 0; i <= m; i++)
				printf("%d ", RZ[i]+1);
			printf("\n");

			printf("C: ");
			for (int i = 0; i < elz; i++)
				printf("%d ", CZ[i]+1);
			printf("\n");

			printf("V: ");
			for (int i = 0; i < elz; i++)
				printf("%d ", VZ[i]);
			printf("\n");
			break;
		case 0:
			izlaz = 0;
			kraj = 0;
			break;
		default:
			printf("Izabrali ste nepostojecu opciju!\n");
			break;
		}

		if (kraj)
		{
			printf("Pritisni bilo koji taster za povratak u osnovni meni... ");
			getch();
			system("CLS");
		}	
		
	}
	system("CLS");
	/*
	free(R); free(C); free(V);
	free(R1); free(C1); free(V1);
	free(RZ); free(CZ); free(VZ);*/
}
/*
matrica sa vezbi
1
6 7 7
0 0 0 0 0 0 0
0 0 4 0 0 0 0
0 0 0 5 0 11 0
0 0 0 0 0 0 0
9 0 0 8 10 0 0
0 0 15 0 0 0 0

druga matrica
5
6 7 7
0 0 0 0 0 0 0
0 0 0 0 0 2 0
9 0 0 1 0 0 0
0 0 0 0 0 0 5
20 0 0 0 10 0 0
0 0 0 0 0 4 0
------------------
zbirna matrica

6 7 11
0 0 0 0 0 0 0
0 0 4 0 0 2 0
9 0 0 6 0 11 0
0 0 0 0 0 0 5
29 0 0 8 20 0 0
0 0 15 0 0 4 0


m1
R: 1 1 2 4 4 7 8
C: 3 4 6 1 3 5 3
V: 4 5 11 9 8 10 15

m2
R: 0 0 1 3 4 6 7
C: 5 0 3 6 0 4 5
V: 2 9 1 5 20 10 4

zb m
R: 1 1 3 6 7 10 12
C: 3 6 1 4 6 7 1 4 5 3 6
V: 4 2 9 6 11 5 29 8 20 15 4

*/