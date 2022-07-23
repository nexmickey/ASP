#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void f_meni();

typedef struct cvor {
	int broj;
	int tezina;
	struct cvor* pok;
} Cvor;

Cvor* napraviCvor(int g, int tezina, int bb)
{
	Cvor* cvor = malloc(1 * sizeof(Cvor));
	cvor->broj = g;
	cvor->tezina = tezina;
	cvor->pok = NULL;
	return cvor;
}

void brisanje_cvora_4(Cvor** listaSusednosti, int x, int brCvorova)
{
	Cvor* cvor, * pret;
	for (int i = 0; i < brCvorova; i++)
	{
		if (i != x)
		{
			if (listaSusednosti[i] == NULL)
				continue;
			cvor = listaSusednosti[i]->pok;
			pret = NULL;
			while (cvor)
			{
				if (cvor->broj == x)
				{
					if (pret == NULL)
						listaSusednosti[i]->pok = cvor->pok;
					else
						pret->pok = cvor->pok;
					free(cvor);
					break;
				}
				pret = cvor;
				cvor = cvor->pok;
			}
		}
		else if (i == x)
		{
			cvor = listaSusednosti[i]->pok;
			while (cvor)
			{
				pret = cvor;
				cvor = cvor->pok;
				free(pret);
			}
			listaSusednosti[i]->pok = NULL;
			listaSusednosti[i] = NULL;
		}
	}
	printf("Cvor %d je obrisan!\n", x+1);
}

void dodavanje_grane_na_kraj_5(Cvor** listaSusednosti, int x, int y, int tezina)
{
	Cvor* noviCvor, * tek;
	noviCvor = napraviCvor(y, tezina, x + 1);
	if (!(listaSusednosti[x]->pok))
		listaSusednosti[x]->pok = noviCvor;
	else
	{
		for (tek = listaSusednosti[x]; tek->pok; tek = tek->pok);
		tek->pok = noviCvor;
	}
	printf("Grana %d -> %d je dodata!\n", x + 1, y + 1);
}

Cvor** ucitavanje_iz_fajla_1(char ime_fajla[100], int* brCvorova)
{
	Cvor** listaSusednosti = NULL;
	int g;
	FILE* file = fopen(ime_fajla, "r");
	fscanf(file, "%d\n", brCvorova);
	listaSusednosti = realloc(listaSusednosti, (*brCvorova + 1) * sizeof(Cvor*));
	for (int i = 0; i < *brCvorova; i++) listaSusednosti[i] = napraviCvor(NULL, NULL, i+1);
	listaSusednosti[*brCvorova] = NULL;
	for (int i = 0; i < *brCvorova; i++)
		for (int j = 0; j < *brCvorova; j++)
		{
			fscanf(file, "%d", &g);
			if (g != 0)
				dodavanje_grane_na_kraj_5(listaSusednosti, i, j, g);
		}
	fclose(file);
	return listaSusednosti;
}

void brisanje_grane_6(Cvor** listaSusednosti, int x, int y)
{
	Cvor* cvor = listaSusednosti[x]->pok, * pret = NULL;
	while (cvor && cvor->broj != y)
	{
		pret = cvor;
		cvor = cvor->pok;
	}
	if (!cvor)
	{
		printf("GRESKA"); return 0;
	}
	if (pret == NULL)
		listaSusednosti[x]->pok = cvor->pok;
	else
		pret->pok = cvor->pok;
	free(cvor);
	printf("Grana %d -> %d je obrisana!\n", x + 1, y + 1);
}

void ispis_7(Cvor** listaSusednosti, int brCvorova)
{
	Cvor* cvor;
	for (int i = 0; i < brCvorova; i++)
	{
		if (listaSusednosti[i]) {
			cvor = listaSusednosti[i]->pok;
			printf("%d", i + 1);
			while (cvor)
			{
				printf(" -> %d", cvor->broj + 1);
				if (cvor->tezina) printf("(%d)", cvor->tezina);
				cvor = cvor->pok;
			}
			putchar('\n');
		}
	}
}

void r_rotate_one(int* a, int n)
{
	int t = a[n - 1];
	for (int i = n - 1; i > 0; i--)
		a[i] = a[i - 1];
	a[0] = t;
}

int are_equal(int* a, int* b, int count)
{
	while (count-- > 0)
		if (*a++ != *b++) 
			return 0;
	return 1;
}

int is_rotation(int* a, int* b, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (are_equal(a, b, n)) return 1;
		r_rotate_one(b, n);
	}
	return 0;
}

int** detekcija_ciklusa_8(Cvor** listaSusednosti, int brCvorova, int** noviDuzineCiklusa, int *noviBrojCiklusa)
{
	Cvor* cvor;
	int posecen[100] = { 0 };
	int stek[1000] = { 0 };
	int ciklus[50][100] = { 0 };
	int duzineCiklusa[50] = { 0 };
	for (int i = 0; i < 10; i++) for (int j = 0; j < 100; j++) ciklus[i][j] = -1;
	int parent[100];
	int index = -1, brojCiklusa = 0, g, br = 0, br1 = 0, provera;
	for (int i = 0; i < brCvorova; i++)
	{
		for (int j = 0; j < brCvorova; j++) parent[j] = -1;
		for (int j = 0; j < brCvorova; j++) posecen[j] = 0;
		for (int j = 0; j < 300; j++) stek[j] = 0;
		index = -1; br = 0;
		stek[++index] = i;
		while (index > -1)
		{
			if (posecen[stek[index]])
				index--;
			else
			{
				g = stek[index];
				posecen[g] = 1;
				index--;
				if (!listaSusednosti[g])
					continue;
				cvor = listaSusednosti[g]->pok;
				while (cvor)
				{
					if (cvor->broj == i) //pronadjen ciklus u i
					{
						cvor = cvor->pok;
						brojCiklusa++;
						br = 0;
						ciklus[brojCiklusa - 1][br++] = g;
						g = parent[g];
						while (g != i)
						{
							ciklus[brojCiklusa - 1][br++] = g;
							g = parent[g];
						}
						ciklus[brojCiklusa - 1][br++] = i;
						duzineCiklusa[brojCiklusa - 1] = br;
						//provera
						provera = 1;
						for (int j = 0; j < brojCiklusa - 1; j++)
						{
							if (duzineCiklusa[j] == duzineCiklusa[brojCiklusa - 1] && is_rotation(ciklus[j], ciklus[brojCiklusa - 1], duzineCiklusa[j]))
								provera = 0;
						}
						if (provera == 0)
						{
							for (int j = 0; j < duzineCiklusa[brojCiklusa - 1]; j++)
								ciklus[brojCiklusa - 1][j] = 0;
							duzineCiklusa[brojCiklusa - 1] = 0;
							brojCiklusa--;
						}
					}
					else
					{
						stek[index + 1] = cvor->broj;
						if(parent[cvor->broj] == -1)
							parent[cvor->broj] = g;
						index++;
						cvor = cvor->pok;
					}
				}
			}
		}
	}
	int** noviCiklusi = malloc(brojCiklusa * sizeof(int*));
	*noviBrojCiklusa = brojCiklusa;
	noviDuzineCiklusa[0] = malloc(brojCiklusa * sizeof(int));
	for (int i = 0; i < brojCiklusa; i++)
		for (int l = 0, d = duzineCiklusa[i] - 1; l<d; l++, d--)
		{ g = ciklus[i][l]; ciklus[i][l] = ciklus[i][d]; ciklus[i][d] = g; }
	for (int i = 0; i < brojCiklusa; i++)
	{
		noviCiklusi[i] = malloc(duzineCiklusa[i] * sizeof(int));
		noviDuzineCiklusa[0][i] = duzineCiklusa[i];
		for (int j = 0; j < duzineCiklusa[i]; j++)
			noviCiklusi[i][j] = ciklus[i][j];
	}
	return noviCiklusi;
}

int odredjivanje_max_kompenzacije_9(Cvor** listaSusednosti, int brCvorova, int* ciklus, int duzinaCiklusa)
{
	int mx = INT_MAX, promena;
	Cvor* g;
	for (int i = 0; i < duzinaCiklusa; i++)
	{
		g = listaSusednosti[ciklus[i]]->pok;
		promena = 0;
		while (g)
		{
			if (g->broj == ciklus[(i + 1) % duzinaCiklusa])
			{
				promena = 1;
				if (g->tezina < mx)
					mx = g->tezina;
				break;
			}
			g = g->pok;
		}
		if (promena == 0)
			return 0;
	}
	return mx;
}

void odredjivanje_max_kompenzacija_9(Cvor** listaSusednosti, int brCvorova, int** ciklus, int* duzineCiklusa, int brojCiklusa)
{
	for (int i = 0; i < brojCiklusa; i++)
		printf("%d. ciklus: %d\n", i + 1, odredjivanje_max_kompenzacije_9(listaSusednosti, brCvorova, ciklus[i], duzineCiklusa[i]));
}

void sprovodjenje_kompenzacije_10(Cvor** listaSusednosti, int brCvorova, int** ciklus, int* duzineCiklusa, int brojCiklusa)
{
	int promena = 1, ciklusT, mx_kompenzacija;
	int sprovedena_kompenzacija[100] = { 0 };
	Cvor* g;
	while (promena)
	{
		promena = 0;
		ciklusT = -1;
		for (int i = 0; i < brojCiklusa; i++)
		{
			if (sprovedena_kompenzacija[i] == 0)
			{
				if (ciklusT == -1)
					ciklusT = i;
				else if(duzineCiklusa[i] > duzineCiklusa[ciklusT])
					ciklusT = i;
			}
		}
		if (ciklusT == -1)
			break;
		sprovedena_kompenzacija[ciklusT] = 1;
		promena = 1;
		mx_kompenzacija = odredjivanje_max_kompenzacije_9(listaSusednosti, brCvorova, ciklus[ciklusT], duzineCiklusa[ciklusT]);
		if (mx_kompenzacija == 0)
			continue;
		for (int i = 0; i < duzineCiklusa[ciklusT]; i++)
		{
			g = listaSusednosti[ciklus[ciklusT][i]]->pok;
			while (g)
			{
				if (g->broj == ciklus[ciklusT][(i + 1) % duzineCiklusa[ciklusT]])
				{
					g->tezina = g->tezina - mx_kompenzacija;
					if (g->tezina == 0)
						brisanje_grane_6(listaSusednosti, ciklus[ciklusT][i], ciklus[ciklusT][(i + 1) % duzineCiklusa[ciklusT]]);
					break;
				}
				g = g->pok;
			}
		}
	}
	printf("Kompenzacija je sprovedena!\n");
}

void isprazni_listu_susednosti_11(Cvor** listaSusednosti, int brCvorova, int **ciklus, int *duzineCiklusa, int brojCiklusa)
{
	Cvor* cvor, * pret;
	for (int i = 0; i < brojCiklusa; i++)
		free(ciklus[i]);
	free(ciklus);
	free(duzineCiklusa);
	for (int i = 0; i < brCvorova; i++)
	{
		if (listaSusednosti[i]) {
			cvor = listaSusednosti[i]->pok;
			while (cvor)
			{
				pret = cvor;
				cvor = cvor->pok;
				free(pret);
			}
		}
	}
	free(listaSusednosti);
	printf("Graf je izbrisan iz memorije!\n");
}

int main()
{
	char ime_fajla[100];
	int brCvorova, cvor1, cvor2, tezina;
	_Bool izlaz = 1;
	Cvor** listaSusednosti = NULL;
	int** ciklus = NULL, * duzineCiklusa = NULL, brojCiklusa;
	f_meni();
	while (izlaz)
	{
		printf("Izaberite opciju: ");
		int opcija;
		scanf("%d", &opcija);
		switch (opcija)
		{
			case 1:
				if (listaSusednosti) { printf("Graf vec postoji!\n"); continue; }
				printf("Unesite ime fajla: ");
				scanf("%s", ime_fajla);
				listaSusednosti = ucitavanje_iz_fajla_1(ime_fajla, &brCvorova);
				printf("Graf je ucitan!\n");
				break;
			case 2:
				if (listaSusednosti) { printf("Graf vec postoji!\n"); continue; }
				printf("Unesite broj pocetnih cvorova: ");
				scanf("%d", &brCvorova);
				listaSusednosti = realloc(listaSusednosti, (brCvorova + 1) * sizeof(Cvor*));
				for (int i = 0; i < brCvorova; i++)
					listaSusednosti[i] = napraviCvor(NULL, NULL, i+1);
				listaSusednosti[brCvorova] = NULL;
				break;
			case 3:
				if (!listaSusednosti) { printf("Graf ne postoji!\n"); continue; }
				listaSusednosti = realloc(listaSusednosti, (++brCvorova + 1) * sizeof(Cvor*));
				listaSusednosti[brCvorova - 1] = napraviCvor(NULL, NULL, brCvorova);
				listaSusednosti[brCvorova] = NULL;
				break;
			case 4:
				if (!listaSusednosti) { printf("Graf ne postoji!\n"); continue; }
				printf("Unesite cvor koji se brise: ");
				scanf("%d", &cvor1);
				brisanje_cvora_4(listaSusednosti, cvor1 - 1, brCvorova);
				break;
			case 5:
				if (!listaSusednosti) { printf("Graf ne postoji!\n"); continue; }
				printf("Unesite cvorove izmedju kojih se dodaje grana i tezinu grane: ");
				scanf("%d%d%d", &cvor1, &cvor2, &tezina);
				dodavanje_grane_na_kraj_5(listaSusednosti, cvor1 - 1, cvor2 - 1, tezina);
				break;
			case 6:
				if (!listaSusednosti) { printf("Graf ne postoji!\n"); continue; }
				printf("Unesite cvorove izmedju kojih se uklanja grana: ");
				scanf("%d%d", &cvor1, &cvor2);
				brisanje_grane_6(listaSusednosti, cvor1 - 1, cvor2 - 1);
				break;
			case 7:
				if (!listaSusednosti) { printf("Graf ne postoji!\n"); continue; }
				ispis_7(listaSusednosti, brCvorova);
				break;
			case 8:
				if (!listaSusednosti) { printf("Graf ne postoji!\n"); continue; }
				ciklus = detekcija_ciklusa_8(listaSusednosti, brCvorova, &duzineCiklusa, &brojCiklusa);
				for (int i = 0; i < brojCiklusa; i++, putchar('\n'))
				{
					printf("%d. ciklus: ", i + 1);
					for (int j = 0; j < duzineCiklusa[i]; j++)
						printf("%d -> ", ciklus[i][j] + 1);
					printf("%d", ciklus[i][0] + 1);
				}
				break;
			case 9:
				if (!listaSusednosti) { printf("Graf ne postoji!\n"); continue; }
				odredjivanje_max_kompenzacija_9(listaSusednosti, brCvorova, ciklus, duzineCiklusa, brojCiklusa);
				break;
			case 10:
				if (!listaSusednosti) { printf("Graf ne postoji!\n"); continue; }
				sprovodjenje_kompenzacije_10(listaSusednosti, brCvorova, ciklus, duzineCiklusa, brojCiklusa);
				break;
			case 11:
				if (!listaSusednosti) { printf("Graf ne postoji!\n"); continue; }
				isprazni_listu_susednosti_11(listaSusednosti, brCvorova, ciklus, duzineCiklusa, brojCiklusa);
				listaSusednosti = NULL;
				ciklus = NULL;
				duzineCiklusa = NULL;
				break;
			case 0:
				izlaz = 0;
				break;
			default:
				printf("Izabrali ste nepostojecu opciju!\n"); system("CLS");
		}
		putchar('\n'); putchar('\n');
	}
}

void f_meni()
{
	printf("IMPLEMENTACIJA OSNOVNIH ALGORITAMA ZA RAD SA GRAFOM I MULTILATERALNA KOMPENZACIJA\n");
	for (int i = 0; i <= 80; i++) printf("=");
	printf("\n"); printf("\n");
	printf(" 1. UCITAVANJE GRAFA KOJI PREDSTAVLJA DUGOVANJA KOMPANIJA\n");
	printf(" 2. KREIRANJE PRAZNE STRUKTURE PODATAKA ZA GRAF\n");
	printf(" 3. DODAVANJE CVORA U GRAF\n");
	printf(" 4. UKLANJANJE CVORA IZ GRAFA\n");
	printf(" 5. DODAVANJE GRANE IZMEDJU DVA CVORA U GRAFU\n");
	printf(" 6. UKLANJANJE GRANE IZMEDJU DVA CVORA U GRAFU\n");
	printf(" 7. ISPIS REPREZENTACIJE GRAFA\n");
	printf(" 8. PRONALAZENJE I ISPISIVANJE CIKLUSA U GRAFU\n");
	printf(" 9. ODREDIVANJE MAX IZNOSA KOMPENZACIJE ZA SVAKI CIKLUS\n");
	printf("10. SPROVODJENJE KOMPENZACIJE\n");
	printf("11. BRISANJE GRAFA IZ MEMORIJE\n");
	printf(" 0. IZLAZ IZ PROGRAMA\n");
	printf("\n");
}

/*
2 14
5 1 2 200
5 2 3 300
5 3 4 400
5 4 5 100
5 5 1 500
5 6 5 500
5 1 6 1000
5 6 7 200
5 7 8 200
5 8 9 300
5 8 10 400
5 9 6 500
5 10 14 300
5 11 12 250
5 13 5 300
5 13 10 300
5 13 11 400
5 13 12 600
5 14 8 300
*/