#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Macros des codes d'echappement ANSI pour la couleur du texte

#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
// definition des variables globales
int lignes;
int colonnes;
double **tableau;
double max;
double min;
int pivot_i;
int pivot_j;
int *solution;
int index2;

// Fonction pour effacer l'ecran de la console
void effacer_ecran()
{
    system("cls");
}

// Fonction pour attendre l'entree de l'utilisateur avant de continuer
void continuer()
{
    printf("\nAppuyez sur Entree pour continuer...");
    getchar(); // Consomme le caractere de nouvelle ligne
    getchar(); // Attend l'entree de l'utilisateur
}

// Fonction pour afficher le menu principal
void afficher_menu()
{
    effacer_ecran();
    printf("\n\n\t\t-----------------Implementation de l'algorithme du Simplexe---------------\n\n");
    printf("\t\t\t\t\tProjet realise par Malak Hbiyel\n");
    printf("\t\t\t\t\t\t      GI19\n");
    printf("\t\t\t\t\t\t      AIAC");
    printf("\n\n\t\t--------------------------------------------------------------------------\n\n\n\n\n\n\n\n\n\n");
    printf(ANSI_COLOR_CYAN "\t\t\t\t1. Entrer les donnees du probleme lineaire\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "\t\t\t\t2. Resoudre le probleme lineaire\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "\t\t\t\t3. Quitter\n" ANSI_COLOR_RESET);
    printf("\n");
    printf("\t\t\t\t\tEntrez votre choix (1-3): ");
}

// Fonction pour entrer les donnes du probleme lineaire
void entrer_donnee()
{
    effacer_ecran();
    int nombre_contraintes;
    int nombre_variables;
    printf("Entrez le nombre de variables : ");
    scanf("%d", &nombre_variables);
    printf("Entrez le nombre de contraintes : ");
    scanf("%d", &nombre_contraintes);

    lignes = nombre_contraintes + 1;
    colonnes = nombre_variables + nombre_contraintes + 1;
    solution = (int*)calloc(nombre_variables,sizeof(int));
    // Allouer de la memoire pour le tableau du simplexe
    tableau = (double **)calloc(lignes, sizeof(double *));
    for (int i = 0; i < lignes; i++)
    {
        tableau[i] = (double *)calloc(colonnes, sizeof(double));
    }

    // Remplir le tableau
    for (int i = 0; i < lignes - 1; i++)
    {
        printf("Entrez les coefficients de la contrainte %d :\n", i + 1);
        for (int j = 0; j < nombre_variables; j++)
        {
            printf(" X%d*", j + 1);
            scanf("%lf", &tableau[i][j]);
        }
        for (int j = 0; j < nombre_contraintes; j++)
        {
            tableau[i][nombre_variables + j] = (i == j) ? 1 : 0;
        }
        printf(" <= ");
        scanf("%lf", &tableau[i][colonnes - 1]);
    }

    printf("Entrez les coefficients de la fonction objectif:\n");
    for (int j = 0; j < nombre_variables; j++)
    {
        printf(" X%d*", j + 1);
        scanf("%lf", &tableau[lignes - 1][j]);
    }
    for (int j = nombre_variables; j < colonnes; j++)
    {
        tableau[lignes - 1][j] = 0;
    }

    printf(ANSI_COLOR_CYAN "\nLes contraintes ont ete enregistrees avec succes.\n" ANSI_COLOR_RESET);
    continuer();
}

// Fonction pour afficher le tableau du simplexe
void afficher_tableau()
{
    
    printf(ANSI_COLOR_CYAN "\n----------------------Tableau du simplexe---------------------\n" ANSI_COLOR_RESET);
    printf("\n");
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            printf("%.2lf\t", tableau[i][j]);
        }
        printf("\n");
    }
    
}

// Fonction pour trouver les coordonnees du pivot
void pivot()
{
    max = 0;
    min = INFINITY;
    pivot_i = 0;
    pivot_j = 0;
    index2 = 0;
    // Trouver la colonne du pivot
    for (int j = 0; j < colonnes - 1; j++)
    {
        if (tableau[lignes - 1][j] > max)
        {
            max = tableau[lignes - 1][j];
            pivot_j = j;
        }
    }

    // Trouver la ligne du pivot
    for (int i = 0; i < lignes - 1; i++)
    {
        if ((tableau[i][pivot_j] != 0) && (tableau[i][colonnes - 1] / tableau[i][pivot_j]) < min && (tableau[i][colonnes - 1] / tableau[i][pivot_j]) >= 0)
        {
            min = tableau[i][colonnes - 1] / tableau[i][pivot_j];
            pivot_i = i;
        }
    }

    // Afficher les coordonnees du pivot et les mouvements de variable dans le tableau
    printf("\nLe pivot est aux coordonnees i=%d et j=%d\t", pivot_i + 1, pivot_j + 1);
    if (pivot_j < colonnes)
    {
        printf("X%d --> e%d", pivot_j + 1, pivot_i + 1);
        solution[pivot_j]=pivot_i;
        index2++;
    }
    else
    {
        printf("e%d --> e%d", pivot_j + 1 - (colonnes - lignes), pivot_i + 1);
    }
}

// Fonction pour mettre à jour le tableau du simplexe
void maj_tableau()
{
    // Mettre à jour la ligne du pivot
    for (int j = 0; j < colonnes; j++)
    {
        if (j == pivot_j)
        {
            continue;
        }
        tableau[pivot_i][j] /= tableau[pivot_i][pivot_j];
    }
    tableau[pivot_i][pivot_j] = 1;

    // Mettre à jour le reste du tableau
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            if (j != pivot_j && i != pivot_i)
            {
                tableau[i][j] -= (tableau[i][pivot_j] * tableau[pivot_i][j]) / tableau[pivot_i][pivot_j];
            }
        }
    }

    // Mettre à jour la colonne du pivot
    for (int i = 0; i < lignes; i++)
    {
        if (i != pivot_i)
        {
            tableau[i][pivot_j] = 0;
        }
    }
}

// Fonction pour verifier la condition de terminaison
int condition_arret()
{
    for (int j = 0; j < colonnes; j++)
    {
        if (tableau[lignes - 1][j] > 0)
        {
            return 0; // Non optimale
        }
    }
    return 1; // Optimale
}

// Fonction pour resoudre le probleme en utilisant la methode du simplexe
void resoudre_pl()
{
    effacer_ecran();

    while (!condition_arret())
    {
        afficher_tableau();
        pivot();
        printf("\n");
        maj_tableau();
    }

    afficher_tableau();
    printf(ANSI_COLOR_CYAN "\n\t\tLa solution optimale est atteinte\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\n--------------------La solution optimale--------------------\n"ANSI_COLOR_RESET);
    printf("\n");
    printf("Max Z = %.2lf\t", -tableau[lignes - 1][colonnes - 1]);
    for (int i = 0; i < colonnes-lignes; i++)
    {
        printf("X%d = %.2lf \t", i + 1, tableau[solution[i]][colonnes - 1]);
    }
    printf("\n");
    continuer();
}


