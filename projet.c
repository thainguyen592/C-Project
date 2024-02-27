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

// Déclarations préliminaires//

void afficherMenuPrincipal();
void afficherMenuReservation();
void MenuReservation();
void afficherReservations();
void saisirReservations();
void sauvegardeReservations();
void chargementReservations();
void modifierReservations();
void supprimerReservations();
void gererRestaurant();
void conv_maj(char chaine[]);
void verif_sauvegarde();
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
    int num_r;                     // numéro réservation
    char date_entree[TAILLE_DATE]; // format JJ/MM/YYYY
    char date_sortie[TAILLE_DATE]; // format JJ/MM/YYYY
    int nombre_pers;               // nombre de personnes
    int chambre;                   // numéro de chambre
    int num_c;                     // numéro client
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
            MenuReservation();
            break;
        case 2:
            gererRestaurant();
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
void afficherMenuReservation()
{
    printf("****** Gérer les réservations ******\n");
    printf("\n");
    printf("-1- Afficher les réservation        \n");
    printf("-2- Saisir une nouvelle réservation \n");
    printf("-3- Modifier une réservation        \n");
    printf("-4- Supprimer une réservation       \n");
    printf("-0- Revenir au menu précédent       \n");
    printf("\n");
    printf("Choisissez une option : ");
}

// Fonction pour le menu Réservations
void MenuReservation()
{
    int resa_choix = VAL_INI;
    while (resa_choix != 0)
    {
        afficherMenuReservation();
        scanf("%d", &resa_choix);
        switch (resa_choix)
        {
        case 1:
            chargementReservations();
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
            scanf("%s", uneresa.date_entree);
            printf("Date de sortie (jj/mm/yyyy)   : ");
            scanf("%s", uneresa.date_sortie);
            printf("Chambre à réserver            : ");
            scanf("%d", &uneresa.chambre);
            printf("Nombre de personnes           : ");
            scanf("%d", &uneresa.nombre_pers);
            printf("Numéro de réservation         : ");
            scanf("%d", &uneresa.num_r);
            printf(">>> Réservation enregistrée \n");

            tabresa[i++] = uneresa; // sauvegardé les données saisie dans le mémoire temporaire
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

    if (nbresa == 0) // si il n'y a pas de réservations
    {
        printf(">>> Aucune réservations à afficher\n");
    }
    else // si il y a des réservations
    {
        printf("Liste des réservations : \n");
        for (int i = 0; i < nbresa; i++) // boucle d'affichage
        {
            uneresa = tabresa[i];
            printf("%d;%s;%s;%d;%d;%d\n", uneresa.num_c, uneresa.date_entree, uneresa.date_sortie, uneresa.chambre, uneresa.nombre_pers, uneresa.num_r);
        }
        printf("\n");
    }
}

// Fonction pour sauvegarder les réservations dans la base de données
void sauvegardeReservations()
{
    FILE *f1;
    struct reservation unresa;

    if (nbresa == 0) // si il n'y a pas de réservations
    {
        printf(">>> Aucune note à sauvegarder\n");
    }
    else
    {
        f1 = fopen(DB_RESERVATIONS, "a"); // ouvrir la base de donnée en ajoute
        for (int i = 0; i < nbresa; i++)  // boucle de sauvegarde
        {
            unresa = tabresa[i];
            fprintf(f1, "%d;%s;%s;%d;%d;%d\n", unresa.num_c, unresa.date_entree, unresa.date_sortie, unresa.chambre, unresa.nombre_pers, unresa.num_r);
        }
        fclose(f1);
        printf(">>> %d réservations sauvegardées\n", nbresa);
        a_sauvegarder = 0;
    }
}

// Fonction pour charger les réservations depuis la base de données
void chargementReservations()
{
    struct reservation uneresa;
    int i = 0, retour;
    FILE *f1;

    verif_sauvegarde();

    f1 = fopen(DB_RESERVATIONS, "r");
    while ((!feof(f1)) && (i < TAILLE_TAB)) // boucle de chargement
    {
        retour = fscanf(f1, "%d;%s;%s;%d;%d;%d\n", &uneresa.num_c, uneresa.date_entree, uneresa.date_sortie, &uneresa.chambre, &uneresa.nombre_pers, &uneresa.num_r);
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

void supprimerReservations()
{
    printf("Fonctionnalité non implémentée pour l'instant.\n");
    printf("\n");
}

// Fonction pour gérer le restaurant
void gererRestaurant()
{
    printf("\n****** Gérer le restaurant ******\n");
    printf("\n");
    printf("Fonctionnalité non implémentée pour l'instant.\n");
    printf("\n");
}

// Fonction pour vérifier si il y a des données à sauvegarder, déclencher par le flag
void verif_sauvegarde()
{
    char reponse[TAILLE_CHAR];

    if (a_sauvegarder)
    {
        printf("Des données ont été modifiées\n");
        printf("Voulez-vous faire une sauvegarde (o/n) : ");
        scanf("%s", reponse);
        conv_maj(reponse);

        if (reponse[0] == 'O')
            sauvegardeReservations();
    }
}

// Fonction pour convertir les caractères en majuscule
void conv_maj(char chaine[])
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
    verif_sauvegarde();
    printf("Au revoir, A bientôt !\n");
}