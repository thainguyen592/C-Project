// Fichiers d'entête
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Constantes
#define TAILLE_TAB 1000                          // taille maximale des tables réservervation
#define TAILLE_CHAR 20                           // pour tout les chaines de type char
#define TAILLE_TEL 10                            // numéro téléphone français
#define TAILLE_MAIL 40                           // pour tout les chaines de type mail
#define TAILLE_DATE 10                           // date pour format JJ/MM/YYYY
#define VAL_INI -1                               // valeur initiale pour entrer dans le boucle
#define DB_RESERVATIONS "liste_reservations.txt" // base de données pour les réservations

// Déclarations préliminaires des fonctions

void afficherMenuPrincipal();
void afficherMenuReservation();
void menuReservation();
void afficherReservations();
void saisirReservations();
void sauvegardeReservations();
void chargementReservations();
void modifierReservations();
void supprimerReservations();
void menuRestaurant();
void convMaj(char chaine[]);
void verifSauvegarde();
void quitter();

// Types global
struct client // structure d'un client
{
    int code;
    char nom[TAILLE_CHAR];
    char prenom[TAILLE_CHAR];
    char date_nais[TAILLE_DATE];
    char tel[TAILLE_TEL];
    char mail[TAILLE_MAIL];
};

struct reservation // structure d'une réservation
{
    int num_c;                     // numéro client
    char date_entree[TAILLE_DATE]; // format JJ/MM/YYYY
    char date_sortie[TAILLE_DATE]; // format JJ/MM/YYYY
    int chambre;                   // numéro de chambre
    int nombre_pers;               // nombre de personnes
    int num_r;                     // numéro réservation
};

// Variables globales
int nbresa = 0;                         // initialiser le nombre des réservations
struct reservation tabresa[TAILLE_TAB]; // tableau principale des réservations
int a_sauvegarder = 0;                  // flag pour l'alerte à sauvegarder

// Programme principale
int main()
{
    int main_choix = VAL_INI;
    while (main_choix != 0)
    {
        afficherMenuPrincipal();
        scanf("%d", &main_choix);
        switch (main_choix)
        {
        case 1:
            menuReservation();
            break;
        case 2:
            menuRestaurant();
            break;
        case 0:
            quitter(); // quitter la programme principale
            break;
        default:
            printf("Option invalide. Veuillez réessayer.\n");
        }
    }
}

// Fonction pour afficher le menu principal
void afficherMenuPrincipal()
{
    printf("****** Menu Principal ******\n");
    printf("\n");
    printf("-1- Gérer les réservations  \n");
    printf("-2- Gérer le restaurant     \n");
    printf("-0- Quitter                 \n");
    printf("\n");
    printf("Choisissez une option : ");
}

// Fonction pour afficher le menu de gestion des réservations
// Fonction pour afficher le menu de gestion des réservations
void afficherMenuReservation()
{
    printf("****** Gérer les réservations ******\n");
    printf("\n");
    printf("-1- Afficher les réservation        \n");
    printf("-2- Saisir une nouvelle réservation \n");
    printf("-3- Modifier une réservation        \n");
    printf("-4- Supprimer une réservation       \n");
    printf("-5- Chargement des réservation      \n");
    printf("-6- Sauvegarde des réservation      \n");
    printf("-0- Revenir au menu précédent       \n");
    printf("\n");
    printf("Choisissez une option : ");
}

// Fonction pour le menu Réservations
void menuReservation()
{
    int resa_choix = VAL_INI;
    while (resa_choix != 0)
    {
        afficherMenuReservation();
        scanf("%d", &resa_choix);
        // Traitement des options
        switch (resa_choix)
        {
        case 1:
            afficherReservations();
            break;
        case 2:
            saisirReservations();
            break;
        case 3:
            modifierReservations();
            break;
        case 4:
            supprimerReservations();
            break;
        case 5:
            chargementReservations();
            break;
        case 6:
            sauvegardeReservations();
            break;
        case 0:
            break;
        default:
            printf(">>> Option invalide. Veuillez réessayer.\n");
        }
    }
}

// Fonction pour saisie une nouvelle réservation
void saisirReservations()
{
    int i = nbresa;
    struct reservation uneresa;

    while (uneresa.num_c != 0) // tant que "0" n'est pas entré, continue la boucle
    {
        printf("Numéro client (0 pour terminer): ");
        scanf("%d", &uneresa.num_c);
        if (uneresa.num_c != 0)
        {
            printf("Date d'entrée (jj/mm/yyyy)    : ");
            scanf(" %s", uneresa.date_entree);
            printf("Date de sortie (jj/mm/yyyy)   : ");
            scanf(" %s", uneresa.date_sortie);
            printf("Chambre à réserver            : ");
            scanf(" %d", &uneresa.chambre);
            printf("Nombre de personnes           : ");
            scanf(" %d", &uneresa.nombre_pers);
            printf("Numéro de réservation         : ");
            scanf(" %d", &uneresa.num_r);
            printf(">>> Réservation enregistrée \n");

            tabresa[i++] = uneresa; // sauvegardé les données saisies dans le tableau
            a_sauvegarder = 1;      // activer le flag pour sauvegarder les données
        }
    }
    nbresa = i; // mettre à jour le nombre des réservations

    // à implémenter une prompt pour demander si user veut continuer au lieu d'entrer 0 pour terminer
}

// Fonction pour afficher tout les réservations dans la BD
void afficherReservations()
{
    struct reservation uneresa;

    if (nbresa == 0)
    {
        printf(">>> Aucune réservation à afficher\n");
    }
    else
    {
        printf("Liste des réservations : \n");
        printf("%6s %-11s %-11s %7s %5s %8s\n", "N°RESA", "DATE ENTREE", "DATE SORTIE", "CHAMBRE", "PERS.", "N°CLIENT");
        for (int i = 0; i < nbresa; i++)
        {
            uneresa = tabresa[i];
            printf("%-6d %-11s %-11s %-7d %-5d %-8d\n", uneresa.num_c, uneresa.date_entree, uneresa.date_sortie, uneresa.chambre, uneresa.nombre_pers, uneresa.num_r);
        }
        printf("\n");
    }
}

// Fonction pour sauvegarder les réservations dans la base de données
void sauvegardeReservations()
{
    FILE *f1;
    f1 = fopen(DB_RESERVATIONS, "a+");
    for (int i = 0; i < nbresa; i++) // boucle de sauvegarde
    {
        fprintf(f1, "%-6d %-11s %-11s %-7d %-5d %-8d\n", tabresa[i].num_r, tabresa[i].date_entree, tabresa[i].date_sortie, tabresa[i].chambre, tabresa[i].nombre_pers, tabresa[i].num_c);
    }
    fclose(f1);
    a_sauvegarder = 0; // désactiver le flag pour sauvegarder les données
    printf(">>> %d réservations sauvegardées\n", nbresa);
}

// Fonction pour charger les réservations depuis la base de données
void chargementReservations()
{
    struct reservation uneresa;
    int i = 0, retour;
    FILE *f1;

    verifSauvegarde();

    f1 = fopen(DB_RESERVATIONS, "r+");
    while (!feof(f1)) // boucle de chargement
    {
        retour = fscanf(f1, "%6d %11s %11s %7d %5d %8d", &uneresa.num_r, uneresa.date_entree, uneresa.date_sortie, &uneresa.chambre, &uneresa.nombre_pers, &uneresa.num_c);
        if (retour != EOF)
        {
            tabresa[i++] = uneresa;
        }
    }
    fclose(f1);
    nbresa = i;
    printf(">>> %d réservations chargées\n", nbresa);
}

// Fonction pour modifier les réservations
void modifierReservations()
{
    printf("Fonctionnalité non implémentée pour l'instant.\n");
    printf("\n");
}

// Fonction pour rechercher une réservation
int rechercherReservations(int num_resa_rechercher)
{
    struct reservation uneresa;
    int resa_trouve = 0; // initialiser la variable de recherche
    {
        if (nbresa == 0)
        {
            printf(">>> Aucune réservation à afficher\n");
        }
        else
        {
            // boucle de recherche
            for (int i = 0; i < nbresa; i++)
            {
                uneresa = tabresa[i];
                if (uneresa.num_r == num_resa_rechercher)
                {
                    resa_trouve = 1;
                    break;
                }
            }
        }
        return resa_trouve;
    }
}

void supprimerReservations()
{
    printf("Fonctionnalité non implémentée pour l'instant.\n");
    printf("\n");
}

// Fonction pour gérer le restaurant
void menuRestaurant()
{
    printf("\n****** Gérer le restaurant ******\n");
    printf("\n");
    printf("Fonctionnalité non implémentée pour l'instant.\n");
    printf("\n");
}

// Fonction pour vérifier si il y a des données à sauvegarder, déclencher par le flag
void verifSauvegarde()
{
    char reponse[TAILLE_CHAR];

    if (a_sauvegarder)
    {
        printf("Des données ont été modifiées\n");
        printf("Voulez-vous faire une sauvegarde (o/n) : ");
        scanf("%s", reponse);
        convMaj(reponse);

        if (reponse[0] == 'O')
            sauvegardeReservations();
    }
}

// Fonction pour convertir les caractères en majuscule
void convMaj(char chaine[])
{
    int i, taille;

    taille = strlen(chaine);
    for (i = 0; i < taille; i++)
    {
        chaine[i] = toupper(chaine[i]);
    }
}

// Fonciton pour quitter le programme
void quitter()
{
    verifSauvegarde();
    printf("Au revoir, A bientôt !\n");
}