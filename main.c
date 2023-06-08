#include "simplex.c"

int main()
{
    int choix;
    do
    {
        afficher_menu();
        scanf("%d", &choix);
        switch (choix)
        {
        case 1:
            entrer_donnee();
            break;
        case 2:
            resoudre_pl();
            break;
        case 3:
            effacer_ecran();
            printf(ANSI_COLOR_CYAN "Programme termine. Au revoir!\n" ANSI_COLOR_RESET);
            break;
        default:
            effacer_ecran();
            printf(ANSI_COLOR_CYAN "Choix invalide. Veuillez reessayer.\n" ANSI_COLOR_RESET);
            continuer();
        }
    } while (choix != 3);

    // Liberer la memoire allouee
    for (int i = 0; i < lignes; i++)
    {
        free(tableau[i]);
    }
    free(tableau);

    return 0;
}
