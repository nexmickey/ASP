#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 1000000
#define Mbbs 253
#define MBITNIZ 1000

void ispisMenija();

/*---------------------------------------------------------------! Inicijalizacija cvora stabla !----------------------------------------------------------------------------------*/

typedef struct CvorG
{
	int brojPoteza;
	int id;
	int prethodni;
	char res;
	char bocica1;
	char bocica2;
	char** mat;
	struct CvorG* brat;
	struct CvorG* sin;
	struct CvorG* prethodniCvor;
} Cvor;

void noviCvor(Cvor* cvor, char** ep, int p, int idd, int n, int prethodni1, char res1, char bocica11, char bocica22)
{
	cvor->id = idd;
	cvor->brojPoteza = p;
	cvor->brat = NULL;
	cvor->sin = NULL;
	cvor->prethodniCvor = NULL;
	cvor->bocica1 = bocica11;
	cvor->bocica2 = bocica22;
	cvor->res = res1;
	cvor->prethodni = prethodni1;
	cvor->mat = (char**)malloc(n * sizeof(char*));
	for (int i = 0; i < n; i++)
	{
		cvor->mat[i] = (char*)malloc(4 * sizeof(char));
		if (cvor->mat[i] == NULL)
			return 0;
		memcpy(cvor->mat[i], ep[i], 4 * sizeof(char));
	}
}

void ispisMatrice(Cvor* cvor, int n)
{
	if (cvor->res) printf("****************************\n");
	//printf("%d %d\n", cvor->prethodni, cvor->id);

	for (int j = 3; j >= 0; j--, putchar('\n'))
		for (int i = 0; i < n; i++)
			printf("%d ", cvor->mat[i][j]);

	if (cvor->res) printf("****************************\n");
	else putchar('\n');
}

void napraviMatricu(char*** ep, int n, char brojBoja)
{
	int broj = 0, broj1 = 0, index = 0, indexNiza = 0, x0, x, slobodne_lokacije = (int)brojBoja * 4;
	char niz[MBITNIZ] = { 0 };
	char flokacije[MBITNIZ] = { 0 };

	srand(time(NULL));
	x0 = rand() % 50 + 2;
	for (char i = 0; i < brojBoja; i++)
		for (char j = 0; j < 4; j++)
			niz[i * 4 + j] = i + 1;
	for (char i = brojBoja; i < (char)n; i++)
		for (char j = 0; j < 4; j++)
			ep[0][i][j] = (char)0;
	while (slobodne_lokacije)
	{
		broj = 0;
		for (int i = 0; i < 14; i++)
		{
			x = (x0 * x0) % Mbbs;
			broj = broj | ((x & 3) << ((13 - i) * 2));
			x0 = x;
		}
		broj = broj % slobodne_lokacije + 1;
		broj1 = 0;
		index = 0;
		while (broj1 < broj)
		{
			if (flokacije[index] == 0)
			{
				broj1++;
				if (broj1 == broj)
					break;
			}
			index++;
		}
		flokacije[index] = 1;
		ep[0][index / 4][index % 4] = niz[indexNiza++];
		slobodne_lokacije--;
	}
}


int proveraTacnogResenja(char** ep, int n)
{
	int provera = 1;
	for (int i = 0; i < n && provera; i++)
		for (int j = 0; j < 3 && provera; j++)
			if (ep[i][j] != ep[i][j + 1])
			{
				provera = 0;
				break;
			}
	return provera;
}

/*--------------------------------------------------------------! Iterativni obilazak stabla !-----------------------------------------------------------------------------------*/

typedef struct stackG
{
	struct CvorG* cvor;
	struct stack* sledeci;
} stack;

void push(stack** top, Cvor* n);
struct node* pop(stack** top);
int isEmpty(stack* top);

int obilazakStabla1(Cvor* root, int n, int brojPoteza)
{
	Cvor* temp = root, *temp1;
	stack* s_temp = NULL;
	int flag = 1;
	while (flag)
	{
		if (temp)
		{
			printf("Cvor %d, nivo: %d , deca cvorovi: ", temp->id, brojPoteza - temp->brojPoteza);
			push(&s_temp, temp);
			temp = temp->sin;
			temp1 = temp;
			while (temp1 != NULL)
			{
				printf("%d ", temp1->id);
				temp1 = temp1->brat;
			}
			putchar('\n');
		}
		else
		{
			if (!isEmpty(s_temp))
			{
				temp = pop(&s_temp);
				temp = temp->brat;
			}
			else
				flag = 0;
		}
	}
}

int obilazakStabla2(Cvor* root, int n, int nivo)
{
	Cvor* temp = root, * temp1;
	stack* s_temp = NULL;
	int flag = 1;
	while (flag)
	{
		if (temp)
		{
			if(temp->brojPoteza == nivo)
				printf("(%d)%d ", temp->prethodni, temp->id);
			push(&s_temp, temp);
			temp = temp->sin;
		}
		else
		{
			if (!isEmpty(s_temp))
			{
				temp = pop(&s_temp);
				temp = temp->brat;
			}
			else
				flag = 0;
		}
	}
}

void push(stack** top, Cvor* cvor1)
{
	stack* new_n = (stack*)malloc(sizeof(stack));
	if (new_n == NULL)
		return 0;
	new_n->cvor = cvor1;
	new_n->sledeci = (*top);
	(*top) = new_n;
}
int isEmpty(stack* top)
{
	if (top == NULL)
		return 1;
	else
		return 0;
}
struct node* pop(stack** top_n)
{
	Cvor* item;
	stack* top;
	top = *top_n;
	item = top->cvor;
	*top_n = top->sledeci;
	free(top);
	return item;
}

int hint(Cvor* root, int n, char* bocica1, char* bocica2)
{
	Cvor* temp = root->sin;
	stack* s_temp = NULL;
	int flag = 1, pronadjen = 0;
	char niz[MAXN] = {0};
	while (flag)
	{
		if (temp)
		{
			if (niz[temp->id] == 0)
			{
				niz[temp->id] = 1;
				if (temp->res)
				{
					pronadjen = 1;
					break;
				}
				push(&s_temp, temp);
				if (temp->brat != NULL)
					push(&s_temp, temp->brat);
				if (temp->sin != NULL)
					temp = temp->sin;
			}
			else
			{
				temp = NULL;
				continue;
			}
		}
		else
		{
			if (!isEmpty(s_temp))
				temp = pop(&s_temp);
			else
				flag = 0;
		}
	}
	if (pronadjen)
	{
		while (temp->prethodniCvor != root)
		{
			while (!isEmpty(s_temp) && temp->prethodniCvor != s_temp->cvor)
				pop(&s_temp);
			temp = s_temp->cvor;
		}
		*bocica1 = temp->bocica1;
		*bocica2 = temp->bocica2;
		return 1;
	}
	else
		return 0;
}


void nadji_resenje(Cvor* root, int n)
{
	Cvor* temp = root->sin;
	stack* s_temp = NULL;
	int flag = 1, pronadjen = 0, index = 0;
	char bocica1, bocica2;
	char niz[MAXN] = { 0 };
	Cvor rekonstrukcijaPuta[100];
	while (flag)
	{
		if (temp)
		{
			if (niz[temp->id] == 0)
			{
				niz[temp->id] = 1;
				if (temp->res)
				{
					pronadjen = 1;
					break;
				}
				push(&s_temp, temp);
				if (temp->brat != NULL)
					push(&s_temp, temp->brat);
				if (temp->sin != NULL)
					temp = temp->sin;
			}
			else
			{
				temp = NULL;
				continue;
			}
		}
		else
		{
			if (!isEmpty(s_temp))
				temp = pop(&s_temp);
			else
				flag = 0;
		}
	}
	if (pronadjen)
	{
		rekonstrukcijaPuta[index++] = *temp;
		while (temp->prethodniCvor != root)
		{
			while (!isEmpty(s_temp) && temp->prethodniCvor != s_temp->cvor)
				pop(&s_temp);
			temp = s_temp->cvor;
			rekonstrukcijaPuta[index++] = *temp;
		}
		printf("Resenje:\n");
		ispisMatrice(root, n);
		for (int i = index - 1; i >= 0; i--)
			ispisMatrice(&rekonstrukcijaPuta[i], n);
	}
	else
		printf("Nema resenja!\n");
}

void isprazniMemoriju(Cvor* root, int n)
{
	Cvor* temp = root, *temp1;
	stack* s_temp = NULL;
	int flag = 1;
	while (flag)
	{
		if (temp)
		{
			push(&s_temp, temp);
			for (int i = 0; i < n; i++)
				free(temp->mat[i]);
			free(temp->mat);
			temp = temp->sin;
		}
		else
		{
			if (!isEmpty(s_temp))
			{
				temp = pop(&s_temp);
				temp1 = temp;
				temp = temp->brat;
				free(temp1);
			}
			else
				flag = 0;
		}
	}
}

/*---------------------------------------------------------------! Pravljenje stabla !----------------------------------------------------------------------------------*/

Cvor** f(Cvor* osnovniCvor, int n, int brojBoja)
{
	int provera, uradjeniPrviCvor, br = 1, mx=0;
	char i, j, i1, ep1, ep2, trazenaBoja;
	Cvor* cvor, * cvor1, * prethodni;
	Cvor** pokCvor, ** pokCvor1, ** pokCvor2, ** pokCvor3=NULL;
	pokCvor3 = (Cvor**)malloc(1 * sizeof(Cvor*));
	if (pokCvor3 == NULL)
		return 0;
	*pokCvor3 = osnovniCvor;

	pokCvor = (Cvor**)malloc(8000000 * sizeof(Cvor*));
	if (pokCvor == NULL)
		return 0;
	pokCvor1 = pokCvor;
	pokCvor2 = pokCvor + 1;
	*pokCvor1 = osnovniCvor;

	while (*pokCvor1 != NULL)
	{
		if (*pokCvor1 == NULL)
			break;
		cvor = *pokCvor1; pokCvor1++;
		prethodni = cvor;
		uradjeniPrviCvor = 0;
		if (cvor->brojPoteza == 0)
			break;
		if (cvor->res)
			continue;
		for (i = 0; i < n; i++) //prva bocica
		{
			for (j = 3; j >= 0 && cvor->mat[i][j] == 0; j--); //stanje u prvoj bocici
			if (j == -1)
				continue;
			else
			{
				trazenaBoja = cvor->mat[i][j];
				ep1 = j;
				for (i1 = 0; i1 < n; i1++) //sve bocice svem sa koje se skida(i)
					if (i != i1)
					{
						provera = 1; //da li moze da se sipa u datu epruvetu i1
						ep2 = 3;
						while (ep2 >= 0)
						{
							if (cvor->mat[i1][ep2] == 0)
							{
								ep2--;
								continue;
							}
							else if (cvor->mat[i1][ep2] == trazenaBoja)
								break;
							else if (cvor->mat[i1][ep2] != trazenaBoja)
							{
								provera = 0;
								break;
							}
						}
						if (provera == 0)
							continue;
						else //pravljenje novog cvora!
						{
							ep2++; ep1 = j;
							if (ep2 >= 4)
								continue;
							cvor1 = (Cvor*)malloc(1 * sizeof(Cvor));
							if (cvor1 == NULL)
								return 0;
							noviCvor(cvor1, cvor->mat, cvor->brojPoteza - 1, br, n, cvor->id, 0, i, i1);
							while (ep1 >= 0 && cvor->mat[i][ep1] == trazenaBoja && ep2 <= 3)
							{
								cvor1->mat[i1][ep2] = cvor1->mat[i][ep1];
								cvor1->mat[i][ep1] = 0;
								ep2++;  ep1--;
							}
							cvor1->res = proveraTacnogResenja(cvor1->mat, n);
							cvor1->prethodniCvor = cvor;
							if (uradjeniPrviCvor == 0)
							{
								cvor->sin = cvor1;
								uradjeniPrviCvor = 1;
							}
							else if (uradjeniPrviCvor == 1)
								prethodni->brat = cvor1;
							prethodni = cvor1;
							//ubacivanje novih cvorova u niz za obradu
							*pokCvor2 = cvor1;
							pokCvor2++;
							br++;
							mx = br;
						}
					}
			}

		}
	}
	//printf("%d\n", mx);
	free(pokCvor);
	return pokCvor3;
}



int main()
{
	int n, k, p, opcija, izlaz = 1;
	char bocica1, bocica2, brojBoja;
	char* bocica11, * bocica22;
	char*** ep;
	ispisMenija();
	printf("Ukupan broj bocica: "); scanf("%d", &n);
	printf("Broj praznih bocica: "); scanf("%d", &k);
	printf("Maksimalan broj poteza: "); scanf("%d", &p);
	putchar('\n');

	brojBoja = (char)n - (char)k;
	ep = (char***)malloc(1 * sizeof(char**));
	if (ep == NULL)
		return 0;
	ep[0] = malloc(n * sizeof(char*));
	if (ep[0] == NULL)
		return 0;
	for (char i = 0; i < n; i++)
	{
		ep[0][i] = malloc(4 * sizeof(char));
		if (ep[0][i] == NULL)
			return 0;
	}
	napraviMatricu(ep, n, brojBoja);

	Cvor* pCvor = malloc(1 * sizeof(Cvor));
	noviCvor(pCvor, ep[0], p, 0, n, -1, 0, -1, -1);
	Cvor** rootStabla = f(pCvor, n, brojBoja);
	Cvor** trenutniCvor = rootStabla, **pretragaCvor = NULL;
	bocica11 = (char*)malloc(1 * sizeof(char));
	if (bocica11 == NULL)
		return 0;
	bocica22 = (char*)malloc(1 * sizeof(char));
	if (bocica22 == NULL)
		return 0;

	printf("Pocetno stanje:\n");
	while (izlaz)
	{
		if (trenutniCvor[0]->res)
		{
			printf("USPESNO RESENA IGRA!\n");
			printf("Mozete ispisati celo stablo /opcija 4 ili 5/ ili ispisati resenje /opcija 3/, a za novu igru ponovo pokrenuti program.\n");
			putchar('\n');
		}
		else if (trenutniCvor[0]->brojPoteza == 0)
		{
			printf("Nemate vise poteza!\n");
			printf("Mozete ispisati celo stablo /opcija 4 ili 5/ ili ispisati resenje /opcija 3/, a za novu igru ponovo pokrenuti program.\n");
			putchar('\n');
		}
		ispisMatrice(*trenutniCvor, n);
		printf("Broj preostalih poteza %d\n", trenutniCvor[0]->brojPoteza);
		putchar('\n');
		printf("Izaberite opciju: "); scanf("%d", &opcija);
		switch (opcija)
		{
		case 1:
			printf("Prebaci tecnost iz bocice: "); 
			scanf("%hhd", &bocica1);
			printf("U bocicu: "); 
			scanf("%hhd", &bocica2);
			pretragaCvor = &(trenutniCvor[0]->sin);
			while (*pretragaCvor != NULL)
			{
				if (pretragaCvor[0]->bocica1 == bocica1-1 && pretragaCvor[0]->bocica2 == bocica2-1)
					break;
				pretragaCvor = &pretragaCvor[0]->brat;
			}
			if (*pretragaCvor == NULL)
				printf("Potez nije moguc!\n");
			else
			{
				*bocica11 = bocica1;
				*bocica22 = bocica2;
				if (pretragaCvor[0]->res == 0 && !hint(pretragaCvor[0], n, bocica11, bocica22))
				{
					printf("Odigrani potez ne vodi ka resenju! Pokusajte ponovo!\n");
					continue;
				}
				trenutniCvor = pretragaCvor;
			}
			break;
		case 2:
			if (hint(trenutniCvor[0], n, bocica11, bocica22))
			{
				pretragaCvor = &(trenutniCvor[0]->sin);
				while (*pretragaCvor != NULL)
				{
					if (pretragaCvor[0]->bocica1 == *bocica11 && pretragaCvor[0]->bocica2 == *bocica22)
						break;
					pretragaCvor = &pretragaCvor[0]->brat;
				}
				trenutniCvor = pretragaCvor;
			}
			else
				printf("Nema poteza koji vodi ka resenju u preostalom broju poteza!\n");
			break;
		case 3:
			nadji_resenje(rootStabla[0], n);
			break;

		case 4:
			for (int i = 0; i <= p; i++)
			{
				printf("Nivo: %d\n", i);
				obilazakStabla2(rootStabla[0], n, p-i);
				putchar('\n');
			}
			break;
		case 5:
			obilazakStabla1(rootStabla[0], n, p);
			break;
		case 0:
			izlaz = 0;
			break;
		default:
			printf("Izabrali ste nepostojecu opciju!\n");
			break;
		}
	}
	free(bocica11); free(bocica22);
	isprazniMemoriju(rootStabla[0], n);

	for (char i = 0; i < n; i++)
		free(ep[0][i]);
	free(ep[0]);
	free(ep);

	system("CLS");
}



void ispisMenija()
{
	char naslov[] = "S O R T I R A NJ E   T E C N O S T I";
	char prvi[] = "1. UNESI SLEDECI POTEZ";
	char drugi[] = "2. POMOC ZA SLEDECI POTEZ";
	char treci[] = "3. ISPIS JEDNOG RESENJA";
	char cetvrti[] = "4. ISPIS KOMPLETNOG STABLA v1";
	char peti[] = "5. ISPIS KOMPLETNOG STABLA v2";
	char nula[] = "0. IZLAZ IZ PROGRAMA";

	for (int i = 0; i <= 80; i++) printf("#");
	putchar('\n'); putchar('#');
	for (int i = 0; i < 79; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 20; i++) putchar(' ');
	printf(naslov);
	for (int i = 0; i < 23; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 79; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 2; i++) putchar(' ');
	printf(prvi);
	for (int i = 0; i < 23; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	for (int i = 0; i < 7; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 2; i++) putchar(' ');
	printf(drugi);
	for (int i = 0; i < 20; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	for (int i = 0; i < 7; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 2; i++) putchar(' ');
	printf(treci);
	for (int i = 0; i < 22; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	for (int i = 0; i < 7; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 2; i++) putchar(' ');
	printf(cetvrti);
	for (int i = 0; i < 16; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar(' '); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	for (int i = 0; i < 7; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 2; i++) putchar(' ');
	printf(peti);
	for (int i = 0; i < 16; i++) putchar(' ');
	putchar('|'); putchar('_'); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar('_'); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar('_'); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar('_'); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	putchar('|'); putchar('_'); putchar('|'); for (int i = 0; i < 2; i++) putchar(' ');
	for (int i = 0; i < 7; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 2; i++) putchar(' ');
	printf(nula);
	for (int i = 0; i < 57; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 79; i++) putchar(' ');
	putchar('#'); putchar('\n');
	for (int i = 0; i <= 80; i++) putchar('#');
	putchar('\n'); putchar('\n');
}



/*
4
2
9

5
2
9

6
2
8

*/

