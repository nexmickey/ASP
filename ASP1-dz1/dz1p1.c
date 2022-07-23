#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MaxN 15

typedef struct ElListe
{
	int ocena;
	char sifraPredmeta[15];
	struct ElListe* sledeci;
	struct ElListe* prethodni;
} ElementListe;

typedef struct lista
{
	ElementListe* prvi, * poslednji;
	int br_elem;
} Lista;

typedef struct student {
	char ime[15];
	char prezime[20];
	int index;
	Lista* listaPredmeta;
} Student;

Lista* napraviListu();
ElementListe* napravi_predmet(int ocena, char sifraPredmeta[]);
ElementListe* pronadji_mesto_umetanja(Lista* l, ElementListe* cvor, int ocena, char sifraPredmeta[]);
ElementListe* pretrazi_listu(ElementListe* cvor, char sifraPredmeta[]);

int dodaj_na_pocetak_liste(Lista* l, int ocena, char sifraPredmeta[]);
int dodaj_na_kraj_liste(Lista* l, int ocena, char sifraPredmeta[]);
int dodaj_iza(ElementListe* tekuci, int ocena, char sifraPredmeta[]);
int dodaj_sortirano(Lista* l, ElementListe* cvor, int ocena, char sifraPredmeta[]);
int poredjenje_indexa(char str1[], char str2[]);

void obrisi_tekuci_predmet(Lista* l, ElementListe* cvor);
void obrisi_predmet_sortirane_liste(Lista* l, char sifraPredmeta[]);

void ispis_predmeta_odredjene_ocene_3(Lista* lista, int ocena);
void obrisi_sve_predmete_5(Lista* l);
void ispis_svih_predmeta_i_ocena_7(Lista* lista);
void ispis_preseka_polozenih_predmeta_8(Lista* l1, Lista* l2);

int pronadji_studenta(Student listaStudenata[], int brojUradjenihStudenata, int index);
int pronadji_predmet(Lista* l, char sifraPredmeta[]);

void ispisMenija();

Student noviStudent(char ime[15], char prezime[20], int index)
{
	Student s;
	strcpy(s.ime, ime);
	strcpy(s.prezime, prezime);
	s.index = index;
	Lista* lista = napraviListu();
	s.listaPredmeta = lista;
	return s;
}


int main() 
{
	Student listaStudenata[MaxN];
	ElementListe* cvor=NULL;
	int n, brojUradjenihStudenata = 0, index, ocena, index1, index2, sifra1,sifra2;
	char ime[15], prezime[20], sifraPredmeta[20];
	_Bool provera = 1, izlaz = 1;
	while (izlaz)
	{
		ispisMenija();
		scanf("%d", &n);
		switch (n)
		{
			case 1:
				printf("Ime: "); scanf("%s", &ime);
				printf("Prezime: "); scanf("%s", &prezime);
				printf("Broj indeksa: "); scanf("%d", &index);
				provera = 1;
				for (int i = 0; i < brojUradjenihStudenata; i++)
					if (listaStudenata[i].index == index)
					{
						printf("Postoji student sa istim brojem indeksa!\n");
						provera = 0;
						break;
					}
				if (provera)
				{
					listaStudenata[brojUradjenihStudenata++] = noviStudent(ime, prezime, index);
					printf("Dodat student %s %s, indeks broj %d. \n", ime, prezime, index);
				}
				break;

			case 2:
				printf("Broj indeksa: "); scanf("%d", &index);
				index1 = pronadji_studenta(listaStudenata, brojUradjenihStudenata, index);
				if (index1 == -1) { printf("Ne postoji student sa indeksom broj %d\n", index); break; }

				printf("Sifra predmeta: "); scanf("%s", &sifraPredmeta);
				sifra1 = pronadji_predmet(listaStudenata[index1].listaPredmeta, sifraPredmeta);
				if (sifra1 != -1){ printf("Vec postoji predmet sa sifrom %s!\n", sifraPredmeta); break; }

				printf("Ocena: "); scanf("%d", &ocena);
				if (ocena >= 6 && ocena <= 10)
				{
					printf("%s %s - dodat predmet %s sa ocenom %d. \n", listaStudenata[index1].ime, listaStudenata[index1].prezime, sifraPredmeta, ocena);
					dodaj_sortirano(listaStudenata[index1].listaPredmeta, cvor, ocena, sifraPredmeta);
				}
				else
					printf("Pogresno uneta ocena!\n");
				break;

			case 3:
				printf("Broj indeksa: "); scanf("%d", &index);
				index1 = pronadji_studenta(listaStudenata, brojUradjenihStudenata, index);
				if (index1 == -1){ printf("Ne postoji student sa indeksom broj %d!\n", index); break; }

				printf("Ocena: "); scanf("%d", &ocena);

				if (ocena >= 6 && ocena <= 10)
				{
					printf("%s %s - polozeni predmeti sa ocenom %d: \n", listaStudenata[index1].ime, listaStudenata[index1].prezime, ocena);
					ispis_predmeta_odredjene_ocene_3(listaStudenata[index1].listaPredmeta, ocena);
				}
				else
					printf("Pogresno uneta ocena!\n");
				break;

			case 4:
				printf("Broj indeksa: "); scanf("%d", &index);
				index1 = pronadji_studenta(listaStudenata, brojUradjenihStudenata, index);
				if (index1 == -1){ printf("Ne postoji student sa indeksom broj %d!\n", index); break; }

				printf("Sifra predmeta: "); scanf("%s", &sifraPredmeta);
				sifra2 = pronadji_predmet(listaStudenata[index1].listaPredmeta, sifraPredmeta);
				if (sifra2 != -1) 
				{
					obrisi_predmet_sortirane_liste(listaStudenata[index1].listaPredmeta, sifraPredmeta);
					printf("Obrisan predmet sa sifrom %s!\n", sifraPredmeta);
				}
				else
				{
					printf("Ne postoji predmet sa sifrom %s!\n", sifraPredmeta); break;
				}
				break;

			case 5:
				printf("Broj indeksa: "); scanf("%d", &index);
				index1 = pronadji_studenta(listaStudenata, brojUradjenihStudenata, index);
				if (index1 == -1) { printf("Ne postoji student sa indeksom broj %d!\n", index); break; }

				obrisi_sve_predmete_5(listaStudenata[index1].listaPredmeta);
				printf("%s %s - obrisani svi polozeni ispiti! \n", listaStudenata[index1].ime, listaStudenata[index1].prezime);
				break;

			case 6:
				printf("Broj indeksa: "); scanf("%d", &index);
				index1 = pronadji_studenta(listaStudenata, brojUradjenihStudenata, index);
				if (index1 == -1) { printf("Ne postoji student sa indeksom broj %d!\n", index); break; }

				printf("%s %s je polozio %d ispita. \n", listaStudenata[index1].ime, listaStudenata[index1].prezime, listaStudenata[index1].listaPredmeta->br_elem);
				break;

			case 7:
				printf("Broj indeksa: "); scanf("%d", &index);
				index1 = pronadji_studenta(listaStudenata, brojUradjenihStudenata, index);
				if (index1 == -1) { printf("Ne postoji student sa indeksom broj %d!\n", index); break; }

				printf("%s %s - polozeni predmeti: \n", listaStudenata[index1].ime, listaStudenata[index1].prezime);
				ispis_svih_predmeta_i_ocena_7(listaStudenata[index1].listaPredmeta);
				break;

			case 8:
				printf("Broj indeksa prvog studenta: "); scanf("%d", &index);
				index1 = pronadji_studenta(listaStudenata, brojUradjenihStudenata, index);
				if (index1 == -1) { printf("Ne postoji student sa indeksom broj %d!\n", index); break; }

				printf("Broj indeksa drugog studenta: "); scanf("%d", &index);
				index2 = pronadji_studenta(listaStudenata, brojUradjenihStudenata, index);
				if (index2 == -1) { printf("Ne postoji student sa indeksom broj %d!\n", index); break; }

				ispis_preseka_polozenih_predmeta_8(listaStudenata[index1].listaPredmeta, listaStudenata[index2].listaPredmeta);
				break;

			case 0:
				izlaz = 0;
				system("CLS");
				break;
			default:
				break;
		}
		if (izlaz)
		{
			printf("Pritisni bilo koji taster za povratak u osnovni meni... ");
			getch();
			system("CLS");
		}
	}
}

Lista* napraviListu()
{
	Lista* l = calloc(1, sizeof(Lista));
	if (!l)
		return 0;
	return l;
}

ElementListe* napravi_predmet(int ocena, char sifraPredmeta[])
{
	ElementListe* noviEl = (ElementListe*)malloc(sizeof(ElementListe));
	if (noviEl == NULL)
		return NULL;
	strcpy(noviEl->sifraPredmeta, sifraPredmeta);
	noviEl->ocena = ocena;
	noviEl->sledeci = NULL;
	noviEl->prethodni = NULL;
	return noviEl;
}

int dodaj_na_pocetak_liste(Lista* l, int ocena, char sifraPredmeta[])
{
	ElementListe* noviEl = napravi_predmet(ocena, sifraPredmeta);
	if (!noviEl)
		return 0;
	noviEl->sledeci = l->prvi;
	if (l->prvi != NULL)
		l->prvi->prethodni = noviEl;
	else
		l->poslednji = noviEl;
	l->prvi = noviEl;
	return 1;
}

int dodaj_na_kraj_liste(Lista* l, int ocena, char sifraPredmeta[])
{
	ElementListe* noviEl = napravi_predmet(ocena, sifraPredmeta);
	if (!noviEl)
		return 0;
	noviEl->prethodni = l->poslednji;
	if (l->prvi == NULL)
	{
		l->prvi = noviEl;
		l->poslednji = noviEl;
	}
	else
	{
		l->poslednji->sledeci = noviEl;
		l->poslednji = noviEl;
	}
	return 1;
}

ElementListe* pronadji_mesto_umetanja(Lista* l, ElementListe* cvor, int ocena, char sifraPredmeta[])
{
	cvor = l->prvi;
	if (cvor == NULL)
		return NULL;
	while (cvor->sledeci != NULL && poredjenje_indexa(cvor->sledeci->sifraPredmeta, sifraPredmeta))
		cvor = cvor->sledeci;
	return cvor;
}

int dodaj_iza(ElementListe* tekuci, int ocena, char sifraPredmeta[])
{
	ElementListe* noviEl = napravi_predmet(ocena, sifraPredmeta);
	if (noviEl == NULL)
		return 0;
	noviEl->sledeci = tekuci->sledeci;
	noviEl->prethodni = tekuci;

	if (tekuci->sledeci != NULL)
		tekuci->sledeci->prethodni = noviEl;
	tekuci->sledeci = noviEl;
	return 1;
}

int dodaj_sortirano(Lista* l, ElementListe* cvor, int ocena, char sifraPredmeta[])
{
	++l->br_elem;
	if (l->prvi == NULL)
	{
		ElementListe* noviEl = napravi_predmet(ocena, sifraPredmeta);
		if (noviEl == NULL)
			return 0;
		l->prvi = noviEl;
		l->poslednji = noviEl;
		return 1;
	}
	if (poredjenje_indexa(l->prvi->sifraPredmeta, sifraPredmeta) == 0)
		return dodaj_na_pocetak_liste(l, ocena, sifraPredmeta);
	
	cvor = pronadji_mesto_umetanja(l, cvor, ocena, sifraPredmeta);
	if (dodaj_iza(cvor, ocena, sifraPredmeta) == 1)
		return 1;
	if (cvor == l->poslednji)
		l->poslednji = cvor->sledeci;
	return 1;
}

int poredjenje_indexa(char str1[], char str2[])
{
	int d1, d2, d, manji = 2;
	d1 = strlen(str1); d2 = strlen(str2); d = d1;
	if (d1 > d2) { d = d2; manji = 0; }
	else if(d2 > d1) { d = d1; manji = 1; }

	for (int i = 0; i < d; i++)
		if (str1[i] > str2[i]) { manji = 0; break; }
		else if (str1[i] < str2[i]) { manji = 1; break; }

	return(manji);  // str1 < str2 = 1  | str1 > str2 = 0  | str1 == str2 = 2
}

/*---------------------------------------------------------------------------------------------------------------*/

ElementListe* pretrazi_listu(ElementListe* cvor, char sifraPredmeta[])
{
	ElementListe* cvor1 = cvor;
	for (; cvor1 != NULL && poredjenje_indexa(cvor1->sifraPredmeta, sifraPredmeta); cvor1 = cvor1->sledeci)
	{
		if (poredjenje_indexa(cvor1->sifraPredmeta, sifraPredmeta)==2)
		{
			return cvor1;
		}
	}
	return NULL;
}

void obrisi_tekuci_predmet(Lista* l, ElementListe* cvor)
{
	if (cvor == NULL)
		return;
	if (cvor->prethodni != NULL)
		cvor->prethodni->sledeci = cvor->sledeci;
	if (cvor->sledeci != NULL)
		cvor->sledeci->prethodni = cvor->prethodni;

	if (cvor == l->prvi)
		l->prvi = cvor->sledeci;
	if (cvor == l->poslednji)
		l->poslednji = cvor->prethodni;
	free(cvor);
}

void obrisi_predmet_sortirane_liste(Lista* l, char sifraPredmeta[])
{
	ElementListe* cvor = l->prvi;
	cvor = pretrazi_listu(cvor, sifraPredmeta);
	if (cvor != NULL)
	{
		obrisi_tekuci_predmet(l, cvor);
		--l->br_elem;
	}
}

/*-----------------------------------------------------------------------------------------------------------------*/

void ispis_predmeta_odredjene_ocene_3(Lista* lista, int ocena)
{
	ElementListe* cvor = lista->prvi;
	while (cvor != NULL)
	{
		if (cvor->ocena == ocena)
			printf("Sifra predmeta: %s \n", cvor->sifraPredmeta);
		cvor = cvor->sledeci;
	}
	//return 0;
}

void obrisi_sve_predmete_5(Lista* l)
{
	ElementListe* cvor = l->prvi;
	ElementListe* cvor1 = l->prvi;
	while (cvor != NULL)
	{
		cvor1 = cvor->sledeci;
		obrisi_tekuci_predmet(l, cvor);
		cvor = cvor1;
	}
	l->br_elem = 0;
}

void ispis_svih_predmeta_i_ocena_7(Lista* lista)
{
	ElementListe* cvor = lista->prvi;
	while (cvor != NULL)
	{
		printf("Sifra predmeta: %s  Ocena: %d \n", cvor->sifraPredmeta, cvor->ocena);
		cvor = cvor->sledeci;
	}
}

void ispis_preseka_polozenih_predmeta_8(Lista* l1, Lista* l2)
{
	_Bool presek = 1;
	ElementListe* cvor1 = l1->prvi;
	ElementListe* cvor2 = l2->prvi;
	while (cvor1 != NULL)
	{
		cvor2 = l2->prvi;
		if (pretrazi_listu(cvor2, cvor1->sifraPredmeta) != NULL)
		{
			printf("Sifra predmeta: %s \n", cvor1->sifraPredmeta);
			presek = 0;
		}
		cvor1 = cvor1->sledeci;
	}
	if (presek)
		printf("Nema istih polozenih predmeta!\n");
}

/*-----------------------------------------------------------------------------------------------------------------*/

int pronadji_studenta(Student listaStudenata[], int brojUradjenihStudenata, int index)
{
	for (int i = 0; i < brojUradjenihStudenata; i++)
		if (listaStudenata[i].index == index)
			return i;
	return -1;
}

int pronadji_predmet(Lista* l, char sifraPredmeta[])
{
	ElementListe* cvor = l->prvi;
	if (cvor == NULL)
		return -1;
	while(cvor!=NULL)
	{ 
		if (strcmp(cvor->sifraPredmeta, sifraPredmeta) == 0)
			return 1;
		cvor = cvor->sledeci;
	}
	return -1;
}

/*-----------------------------------------------------------------------------------------------------------------*/

void ispisMenija()
{
	char naslov[] = "INFORMACIONI SISTEM ELEKTROTEHNICKOG FAKULTETA";
	char prvi[] = "1. DODAVANJE NOVOG STUDENTA";
	char drugi[] = "2. DODAVANJE POLOZENOG PREDMETA STUDENTU";
	char treci[] = "3. ISPIS SVIH PREDMETA SA OCENOM";
	char cetvrti[] = "4. BRISANJE POLOZENOG PREDMETA STUDENTA";
	char peti[] = "5. BRISANJE SVIH POLOZENIH PREDMETA STUDENTA";
	char sesti[] = "6. ISPIS BROJA POLOZENIH PREDMETA STUDENTA";
	char sedmi[] = "7. ISPIS SVIH POLOZENIH PREDMETA STUDENTA";
	char osmi[] = "8. ISPIS PREDMETA KOJE SU POLOZILI DVA STUDENTA";
	char nula[] = "0. IZLAZ IZ PROGRAMA";

	for (int i = 0; i <= 100; i++) printf("#");
	putchar('\n'); putchar('#');
	for (int i = 0; i < 99; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 25; i++) putchar(' ');
	printf(naslov);
	for (int i = 0; i < 28; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 99; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 2; i++) putchar(' ');
	printf(prvi);
	for (int i = 0; i < 18; i++) putchar(' ');
	printf(peti);
	for (int i = 0; i < 8; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 2; i++) putchar(' ');
	printf(drugi);
	for (int i = 0; i < 5; i++) putchar(' ');
	printf(sesti);
	for (int i = 0; i < 10; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 2; i++) putchar(' ');
	printf(treci);
	for (int i = 0; i < 13; i++) putchar(' ');
	printf(sedmi);
	for (int i = 0; i < 11; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 2; i++) putchar(' ');
	printf(cetvrti);
	for (int i = 0; i < 6; i++) putchar(' ');
	printf(osmi);
	for (int i = 0; i < 5; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 36; i++) putchar(' ');
	printf(nula);
	for (int i = 0; i < 43; i++) putchar(' ');
	putchar('#'); putchar('\n'); putchar('#');
	for (int i = 0; i < 99; i++) putchar(' ');
	putchar('#'); putchar('\n');
	for (int i = 0; i <= 100; i++) putchar('#');
	putchar('\n'); putchar('\n');

	printf("Izaberite opciju: ");
}


/*
1
Marko Markovic 100

1
Petar Petrovic 101

2
100 13S111P1 7

2
101 13S111ASP1 9

2
101 13S061F 8

2
100 13S111ASP1 9

2
101 13S111P1 8

2
100 13S081M1 7

*/