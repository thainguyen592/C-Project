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

// Déclarations préliminaires des fonctions

void afficherMenuPrincipal();
void afficherMenuReservation();
void menuReservation();
void afficherReservations();
void saisirReservations();
void demanderCritereRecherche();
void rechercherReservation(struct reservation resa_a_trouver);
void sauvegardeReservations();
void chargementReservations();
void modifierReservations();
void menuRestaurant();
void convMaj(char chaine[]);
void verifSauvegarde();
void quitter();

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
    printf("-2- Rechercher une réservation      \n");
    printf("-3- Saisir une nouvelle réservation \n");
    printf("-4- Modifier une réservation        \n");
    printf("-5- Chargement des réservation      \n"); // automatiser lors d'ouverture du programme
    printf("-6- Sauvegarde des réservation      \n"); // automatiser lors d'ouverture du programme
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
            demanderCritereRecherche();
            break;
        case 3:
            saisirReservations();
            break;
        case 4:
            modifierReservations();
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
    int num_resa_modif, resa_trouve;
    struct reservation uneresa;
}

// Fonction pour demander le critère de recherche au utilisateur
void demanderCritereRecherche()
{
    struct reservation recherche_resa;
    {
        if (nbresa == 0)
        {
            printf(">>> Aucune réservation à afficher\n");
        }
        else
        {
            printf("Choisir critère de recherche : \n");
            printf("-1- Numéro de réservation\n");
            printf("-2- Numéro de client\n");
            printf("-3- Date d'entrée\n");
            printf("-4- Date de sortie\n");
            printf("-5- Chambre\n");
            printf("-0- Revenir au menu précédent\n");

            int rechercher_choix = VAL_INI;
            scanf("%d", &rechercher_choix);
            while (rechercher_choix < 0 || rechercher_choix > 5)
            {
                printf(">>> Option invalide. Veuillez réessayer.\n");
                scanf("%d", &rechercher_choix);
            }
            switch (rechercher_choix)
            {
            case 1:
                printf("Numéro de réservation à rechercher : ");
                scanf("%d", &recherche_resa.num_r);
                break;
            case 2:
                printf("Numéro de client à rechercher : ");
                scanf("%d", &recherche_resa.num_c);
                break;
            case 3:
                printf("Date d'entrée à rechercher : ");
                scanf("%s", recherche_resa.date_entree);
                break;
            case 4:
                printf("Date de sortie à rechercher : ");
                scanf("%s", recherche_resa.date_sortie);
                break;
            case 5:
                printf("Chambre à rechercher : ");
                scanf("%d", &recherche_resa.chambre);
                break;
            case 0:
                break;
            default:
                printf(">>> Option invalide. Veuillez réessayer.\n");
            }
            rechercherReservation(recherche_resa);
        }
    }
}

// Fonction pour rechercher une réservation par critère
void rechercherReservation(struct reservation resa_a_trouver)
{
    struct reservation uneresa;
    int i, j, nbresa_trouve = 0;
    int idx_resa_trouve[TAILLE_TAB];

    if (nbresa == 0)
    {
        printf(">>> Aucune réservation à afficher\n");
    }
    else
    {
        for (i = 0; i < nbresa; i++)
        {
            uneresa = tabresa[i];
            if (uneresa.num_r == resa_a_trouver.num_r || uneresa.num_c == resa_a_trouver.num_c || strcmp(uneresa.date_entree, resa_a_trouver.date_entree) == 0 || strcmp(uneresa.date_sortie, resa_a_trouver.date_sortie) == 0 || uneresa.chambre == resa_a_trouver.chambre)
            {
                idx_resa_trouve[nbresa_trouve] = i;
                nbresa_trouve++;
            }
        }
        if (nbresa_trouve == 0)
        {
            printf(">>> Aucune réservation trouvée\n");
        }
        else
        {
            printf(">>> %d réservations trouvées\n", nbresa_trouve);
            printf("%6s %-11s %-11s %7s %5s %8s\n", "N°RESA", "DATE ENTREE", "DATE SORTIE", "CHAMBRE", "PERS.", "N°CLIENT");
            for (j = 0; j < nbresa_trouve; j++)
            {
                uneresa = tabresa[idx_resa_trouve[j]];
                printf("%-6d %-11s %-11s %-7d %-5d %-8d\n", uneresa.num_c, uneresa.date_entree, uneresa.date_sortie, uneresa.chambre, uneresa.nombre_pers, uneresa.num_r);
            }
        }
    }
}

// Fonction pour afficher le menu pour la partie Restaurant
void menuRestaurant()
{
    printf("Fonction en cours de développement");
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