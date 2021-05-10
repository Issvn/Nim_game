#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

// --------------------------- DECLARATION DES VARIABLES GLOBALES  ---------------------------

#define VMIN 5 //nombre de ligne et col minimum
#define VMAX 30 //nombre de ligne et col max

/*
*Structure représentant une case du jeu avec des coordonnées
*/
struct T_Case {
	int x; //abscisse x de la case
	int y; //abscisse y de la case
	int nimber; //nimber de la case
	char affichage; //caractère d'affichage de la case
};
typedef struct T_Case Case;

// --------------------------- DECLARATION DES ENTETES DE FONCTIONS ------------------------

int Lire_Entier(int a, int b); //fonction qui lit un entier entre deux intervalle
int Nimber(int nlig, int ncol, int x, int y); //fonction qui calcul le nimber d'une case
Case Coup_joueur(Case pion, Case **Plateau, int nlig, int ncol); //fonction qui renvoi le coup d'un joueur
Case Coup_Ordi_Hasard(Case pion, Case **Plateau, int nlig, int ncol); //fonction qui renvoi la case joué au hasard par l'ordinateur
Case Coup_Ordi_Gagnant(Case pion, Case **Plateau, int nlig, int ncol, int niveau); //fonction qui renvoi la case gagnante joue par l'ordinateur
Case* Voisines(Case pion, Case** Plateau, int nlig, int ncol, int nbCoup); //fonction qui renvoi un tableau des cases voisine du pion sur le plateau
int NbCoup(Case pion, Case** Plateau, int nlig, int ncol); //fonction qui renvoi le nombre de coup possible pour un pion sur le plateau
int Hasard(int min, int max); //fonction qui calcul un nombre au hasard entre deux intervalles
int * Parametre(); //fonction qui initialise les parametre de jeu
void affiche_plateau(Case **tab, int a, int b); //fonction qui affiche graphiquement le plateau
void deplacer_pion(Case pion, Case** Plateau); //fonction qui deplace le pion sur le plateau
int trois(); //fonction qui renvoi un nombre entre 1 et trois pour le calcul de probabilité du coup
void affiche_Nimber(Case **tab, int a, int b);


// --------------------------- CODE PRINCIPAL ----------------------------------------------

int main()
{
	// On initialise les parametre du jeu
	int *tab = Parametre();

	int nlig = (int)(tab[0]);
	int ncol = (int)(tab[1]);

	// On initialise le pion
	Case pion;
	pion.x = 0; pion.y = 0; pion.affichage = 'o'; pion.nimber = Nimber((*tab), (*tab+1),pion.x, pion.y); //initialisation des donnée du pion a la case (0,0)

	// Boolean de fin de jeu
	bool fin = false;

	// On initialise le plateau qui est un tableau a deux dimension (double pointeur)
	Case **Plateau;

	// On alloue dynamiquement la mémoire au tableau avec nlig quantite de case
	Plateau = malloc(nlig * sizeof(Case));

	for (int i = 0; i < (*tab); i++)
	{
		Plateau[i] = malloc(*(tab + 1) * sizeof(Case)); // pour chaque ligne on alloue ncol quantite de case
	}

	// On initialise le plateau de jeu
	// On parcourt du plateau selon l'ennoncé
	  int k = 0;
    for(int i = tab[1]-1; i >= 0 ; i--) {
        for(int j = tab[0]-1; j >= 0; j--) {
            if (i == tab[1]-1 && j == tab[0]-1) {
                Case c;  c.x = i; c.y = j; c.affichage = '-'; c.nimber = 0;
                Plateau[j][i] = c; k++;
            }
            else {
                Case c;
                if (k == 1 || k == 2) {
                      c.x = i; c.y = j; c.affichage = '-'; c.nimber = 1;
                } else{
                     c.x = i; c.y = j; c.affichage = '-'; c.nimber = 0;;
                }
                k++;
                //Case c;  c.x = i; c.y = j; c.affichage = '-'; c.nimber = Nimber(tab[0], tab[1], i, j);
                //Case c;  c.x = i; c.y = j; c.affichage = '-'; c.nimber = Nimber(*(tab)+1, *(tab), i, j);
                Plateau[j][i]=c;
            }
            if (k == 3) k = 0;
        }

	}

	// On place le pion à la position de départ
	pion.nimber = Plateau[0][0].nimber; pion.x = Plateau[0][0].x; pion.y = Plateau[0][0].y;
	Plateau[0][0] = pion;

	printf("\n");
	printf("C'est parti ! \n");
	printf("\n");

	// Affichage du tableau de jeu
	affiche_plateau(Plateau, *(tab), *(tab + 1));
    //affiche_Nimber(Plateau, *(tab), *(tab + 1));


	printf("\n");


	// Début de la partie
	while (!fin) {
		switch (*(tab + 3)) { //position
		case 1: // Cas où l'ordinateur commence
			Plateau[pion.y][pion.x].affichage = '-'; //actualisation de l'affichage de la case ayant contenu le pion

			if (tab[2] == 1) { // cas ou le niveau selectionnee est 1
				pion = Coup_Ordi_Hasard(pion, Plateau, *(tab), *(tab + 1));
			}
			else{
				//sinon on joue gagnant
				pion = Coup_Ordi_Gagnant(pion, Plateau, *(tab), *(tab + 1), *(tab + 2));
			}

			if (pion.x != -1) { //si le pion n'est pas dans le puit
				// On deplace le pion
				deplacer_pion(pion, Plateau);

				// On actualise l'affichage du plateau
				affiche_plateau(Plateau, *(tab), *(tab + 1));
				//affiche_Nimber(Plateau, *(tab), *(tab + 1));

				*(tab + 3) = 2; //le tour du joueur
			}
			else {

				*(tab + 3) = 0; //le tour est fini
			}

			break;
		case 2:
 			// Cas où c'est le joueur qui commence
			Plateau[pion.y][pion.x].affichage = '-';

			pion = Coup_joueur(pion, Plateau, *(tab), *(tab + 1));

			if (pion.x != -1) {
				// On deplace le pion
				deplacer_pion(pion, Plateau);

				// On actualise l'affichage du plateau
				affiche_plateau(Plateau, *(tab), *(tab + 1));
				//affiche_Nimber(Plateau, *(tab), *(tab + 1));
				*(tab + 3) = 1; //c'est au tour de l'ordinateur
			}
			else {
				*(tab + 3) = 3; //le tour est fini
			}

			break;
		default:
			//cas ou on atteint le puit
			if (*(tab + 3) == 0) {
				printf("C'est termine. BRAVO TU AS GAGNE !\n");
			}
			else {
				printf("C'est termine. L'ORDINATEUR A GAGNE !\n");
			}

			fin = true; //fin du jeur on sort de la boucle while
			break;
		}
	}

	// On libere la memoire occupé par les parametres et le plateau
	free(tab);
	free(Plateau);

	return 0;
}

// --------------------------- SECTION DES FONCTIONS --------------------------------------


/*
* Fonction qui renvoi un nombre aléatoire entre 1 et 3
*/
int trois() {
	return 1 + rand() % 3;
}

/*
* Fonction qui permet de lire un entier entre deux intervalle
*/
int Lire_Entier(int a, int b) {

	int r;
	scanf("%d", &r); // on lit la valeur

	if (r < a) {
		r = a; // Si r est inférieur à a alors r prend la valeur a.
	}
	else if (r > b) {
		r = b; // Si r est supérieur à b alors r prend la valeur b.
	}

	return r;
}

/*
* Fonction qui calcule le nimber d'une case donnée en fonction du nombre de ligne et du nombre de colonnes
*/
int Nimber(int nlig, int ncol, int x, int y) {
	int nimber;
    if (nlig <= ncol)
        if (((nlig - x+2) % 3) == ((ncol - y+1) % 3)) { // Fonction mathématique : Si nlig - a % 3 correspond à ncol - b % 3 alors le nimber = 0, sinon le nimber = 1;
		nimber = 0;
	}
	else {
		nimber = 1;
	}
	return nimber;
}

/*
* Cette fonction gère le coup d'un joueur en proposant au joueur de choisir un coup parmis la liste de coup possible
*/
Case Coup_joueur(Case pion, Case **Plateau, int nlig, int ncol) {


	//on recupere les voisines du pion sur le plateau
	int nbCoup = NbCoup(pion, Plateau, nlig, ncol);
	Case* T_Tab_case = (Case*)malloc(sizeof(Case)*nbCoup); //on initialise un tableau de voisin avec comme taille le nombre de coup

	T_Tab_case = Voisines(pion, Plateau, nlig, ncol, nbCoup); //on calcul les voisins

	if (nbCoup == 1) {
		printf("\nA toi de jouer !\n");
		printf("Seule possibilite ");
	} else {
		printf("\nA toi de jouer !\n");
		printf("choisir la destination ");
	}

	for (int i = 0; i < nbCoup; i++) {
        printf(" %d:(%d,%d)", i + 1, T_Tab_case[i].x + 1, T_Tab_case[i].y + 1); // affichage des coups possible

	}
	printf("\n");

	if (nbCoup > 0) { // si des coups sont possible
		int r;
		printf("---> ");
		scanf("%d", &r); //on demande le choix du coup

		while (r < 1 || r > nbCoup && !isdigit(r) == 1) {
			printf("Erreur !\n");
			printf("---> ");
			scanf("%d", &r);
			printf("\n");
		}

		//on actualise les nouvelle cordonnée du pion choisi
		// -1 est appliquer car les tableaux commencent a l'index 0
		pion.x = T_Tab_case[r - 1].x;
		pion.y = T_Tab_case[r - 1].y;

		free(T_Tab_case); //on libere la memoire du tableau de voisin
	}
	else {
		//plus aucun coup possible, on est dans le puit
		//on affecte une valeur defini a -1 pour identifier que le pion est dans le puit (instruction d'arret du jeu plus tard)
		pion.x = -1;
		pion.y = -1;

		free(T_Tab_case); //on libere la memoire du tableau de voisin
	}

	return pion; //on retourne le pion
}

/*
* Fonction qui joue un coup au hasard parmis les voisines disponibles
*/
Case Coup_Ordi_Hasard(Case pion, Case **Plateau, int nlig, int ncol) {

	//on recupere les voisines du pion sur le plateau
	int nbCoup = NbCoup(pion, Plateau, nlig, ncol);
	Case* T_Tab_case = (Case*)malloc(sizeof(Case)*nbCoup);

	T_Tab_case = Voisines(pion, Plateau, nlig, ncol, nbCoup);

	printf("\n");

	if (nbCoup > 0) {
		int r = Hasard(1, nbCoup); //on choisi un coup au hasard parmi les coup possible

		//on actualise les nouvelle cordonnée du pion choisi
		pion.x = T_Tab_case[r - 1].x;
		pion.y = T_Tab_case[r - 1].y;

		printf("L'ordinateur deplace le pion en (%d,%d)\n", T_Tab_case[r-1].x + 1, T_Tab_case[r-1].y + 1);
		printf("\n");

		free(T_Tab_case);
	}
	else {
		//plus aucun coup possible, on est dans le puit
		pion.x = -1;
		pion.y = -1;
		free(T_Tab_case);
	}
	return pion;
}

/*
* Fonction qui gère les coups gagnants de l'ordinateur ( ici on gere le coup moyen, expert, ou toujours gagnant
* le coup hasard est deja pris en compte dans sa fonction coup_hasard)
*/
Case Coup_Ordi_Gagnant(Case pion, Case **Plateau, int nlig, int ncol, int niveau) {

	//on recupere les voisines du pion sur le plateau
	int nbCoup = NbCoup(pion, Plateau, nlig, ncol);
	Case* T_Tab_case = (Case*)malloc(sizeof(Case)*nbCoup);
	T_Tab_case = Voisines(pion, Plateau, nlig, ncol, nbCoup);
	bool gagnant = false;

	int x;
	srand((unsigned int)time(NULL));

	printf("\n");

	//en fonction du niveau, j'effectue les coups specifique a ces niveaux
	switch (niveau) {
	case 2: //cas niveau moyen

		if (nbCoup > 0) {
			//cas moyen
			x = trois(); //choix probabiliste

			if (x == 1) //joue gagnant (1/3)
				pion = Coup_Ordi_Gagnant(pion, Plateau, nlig, ncol, 4);
				//printf("L'ordinateur deplace le pion en (%d,%d)\n", T_Tab_case[x-1].x + 1, T_Tab_case[x-1].y + 1);
			else // sinon joue hasard
				pion = Coup_Ordi_Hasard(pion, Plateau, nlig, ncol);
				//printf("L'ordinateur deplace le pion en (%d,%d)\n", T_Tab_case[x-1].x + 1, T_Tab_case[x-1].y + 1);
		}
		else {
			//plus aucun coup possible, on est dans le puit
			pion.x = -1;
			pion.y = -1;
		}
		break;

	case 3:

		//cas expert
		x = trois();

		if (nbCoup > 0) {
			if (x == 1 || x == 2) //joue gagnant (2/3)
				pion = Coup_Ordi_Gagnant(pion, Plateau, nlig, ncol, 4);
				//printf("L'ordinateur deplace le pion en (%d,%d)\n", T_Tab_case[x-1].x + 1, T_Tab_case[x-1].y + 1);
			else  //sinon joue hasard
				pion = Coup_Ordi_Hasard(pion, Plateau, nlig, ncol);
				//printf("L'ordinateur deplace le pion en (%d,%d)\n", T_Tab_case[x-1].x + 1, T_Tab_case[x-1].y + 1);
		}
		else {
			//plus aucun coup possible, on est dans le puit
			pion.x = -1;
			pion.y = -1;
		}

		break;

	case 4:
		//l'ordinateur joue toujours gagnant c'est a dire essai d'atteindre une position nul si possible
		for (int i = 0; i < nbCoup; i++) {
			if (T_Tab_case[i].nimber == 0) { //si case de nimber 0 on va dessus
				pion = T_Tab_case[i];
				gagnant = true;
			}
		}
		//si aucun coup gagnant possible on joue un coup au hasard
		if (gagnant == false) { pion = Coup_Ordi_Hasard(pion, Plateau, nlig, ncol); }
		break;

	default:
		break;
	}

	free(T_Tab_case);// on libere la memoire

	return pion;
}

/*
* Ajout des voisines d'un pion sur le plateau dans un tableau de voisines
*/
Case* Voisines(Case pion, Case** Plateau, int nlig, int ncol, int nbCoup) {

	Case* tab = (Case*)malloc(sizeof(Case) * nbCoup);
	int k = 0; //on initialise un entier k a 0 qui sera utilisé comme index du tableau

	//4 cas possible (non optimisé)
	if (pion.x < ncol || pion.y < nlig) {
		if (pion.x + 1 < ncol) { tab[k] = Plateau[pion.y][pion.x + 1]; k++; }
		if (pion.x + 2 < ncol) { tab[k] = Plateau[pion.y][pion.x + 2]; k++; }
		if (pion.y + 1 < nlig) { tab[k] = Plateau[pion.y + 1][pion.x]; k++; }
		if (pion.y + 2 < nlig) { tab[k] = Plateau[pion.y + 2][pion.x]; k++; }
	}

	return tab;
}

/*
* Fonction qui calcul le nombre de coup possible pour un pion sur le plateau
*/
int NbCoup(Case pion, Case** Plateau, int nlig, int ncol) {
	int k = 0;

	//On compte les cas
	if (pion.x < ncol || pion.y < nlig) {
		if (pion.x + 1 < ncol) { k++; }
		if (pion.x + 2 < ncol) { k++; }
		if (pion.y + 1 < nlig) { k++; }
		if (pion.y + 2 < nlig) { k++; }
	}

	return k;
}

/*
* Fonction qui retourne un nombre aleatoire entre a et b
*/
int Hasard(int min, int max) {
	srand((unsigned int)time(NULL));
	int res = min + (rand() % (max - min + 1)); // renvoi un nombre aleatoire entre min et max
	return res;
}

/*
* Fonction parametre qui initialise les parametre du jeu
*/
int * Parametre() {

	int *params; //tableau de parametre
	params = (int *)malloc(sizeof(int) * 4);

	printf("--------------------------------\n");
	printf("\tParametre du jeu : \t\n");
	printf("--------------------------------\n");

	printf("\nNombre de lignes : ");

	params[0] = Lire_Entier(VMIN, VMAX);

	printf("Nombre de Colonnes : ");

	params[1] = Lire_Entier(VMIN, VMAX);

	printf("Niveau de 1 a 4 : ");

	params[2] = Lire_Entier(1, 4);

	printf("Qui commence ? : L'ordinateur (1) ou le joueur (2) : ");

	params[3] = Lire_Entier(1, 2);

	return params;
}

/**
* Fonction qui affiche le plateau
*/
void affiche_plateau(Case **tab, int a, int b) {

	printf("   ");
	for (int j = 0; j < b; j++) {
		printf("%4d", j + 1);
	}
	printf("\n");
	for (int i = 0; i < a; i++)
	{
		printf("%3d", i + 1);
		for (int j = 0; j < b; j++) {
			printf(" | "); printf("%c", tab[i][j].affichage);
		}
		printf(" | \n");
	}
}

/*
* Fonction qui affiche le tableau de nimber (A des fin de test)
*/
void affiche_Nimber(Case **tab, int a, int b) {

	printf("  ");
	for (int j = 0; j < b; j++) {
		printf(" %d", j + 1);
	}
	printf("\n");
	for (int i = 0; i < a; i++)
	{
		printf("%d ", i + 1);

		for (int j = 0; j < b; j++) {
			printf("|"); printf("%d", tab[i][j].nimber);
		}
		printf("|\n");
	}
}

/*
* Fonction qui deplace le pion sur le plateau
*/
void deplacer_pion(Case pion, Case** Plateau) {
	//on initialise les valeurs du pion a deplacer avec la valeur x et y de la case sur le plateau
	pion.x = Plateau[pion.y][pion.x].x;
	pion.y = Plateau[pion.y][pion.x].y;
	pion.nimber = Plateau[pion.y][pion.x].nimber;
	pion.affichage = 'o';

	Plateau[pion.y][pion.x] = pion; //on deplace le pion on initialisant la case du plateau avec le pion
}
