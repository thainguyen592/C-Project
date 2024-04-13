// Fichiers d'entête
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Constantes
#define TAILLE_TAB 1000                          // taille maximale des tables réservervation
#define TAILLE_CHAR 20                           // pour tout les chaines de type char
#define TAILLE_TEL 10                            // numéro téléphone français
#define TAILLE_MAIL 40                           // pour tout les chaines de type mail
#define TAILLE_DATE 8                            // date pour format JJMMYYYY
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
    char date_entree[TAILLE_DATE]; // format JJMMYYYY
    char date_sortie[TAILLE_DATE]; // format JJMMYYYY
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
int lanceRecherche(int num_resa_a_rechercher);
void modifierReservations();
void menuRestaurant();
void convMaj(char chaine[]);
void verifSauvegarde();
bool bissextile(int annee);
bool dateExiste(int jour, int mois, int annee);
bool dateValide(char date[TAILLE_DATE]);
void quitter();

// Programme principale
int main()
{
    // Reservations pour les tests
    struct reservation resa1;
    resa1.num_c = 1001;
    strcpy(resa1.date_entree, "01012021");
    strcpy(resa1.date_sortie, "05012021");
    resa1.chambre = 101;
    resa1.nombre_pers = 2;
    resa1.num_r = 2021001;
    tabresa[0] = resa1;
    nbresa += 1;

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
            printf("Date d'entrée (jjmmyyyy)    : ");
            scanf(" %s", uneresa.date_entree);
            while (!dateValide(uneresa.date_entree))
            {
                printf(">>> Date invalide. Veuillez saisir une date (jjmmyyyy): ");
                scanf(" %s", uneresa.date_entree);
            }
            printf("Date de sortie (jjmmyyyy)   : ");
            scanf(" %s", uneresa.date_sortie);
            while (!dateValide(uneresa.date_sortie))
            {
                printf(">>> Date invalide. Veuillez saisir une date (jjmmyyyy): ");
                scanf(" %s", uneresa.date_sortie);
            }
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
            while (rechercher_choix != 0)
            {
                scanf("%d", &rechercher_choix);
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
}

// Fonction pour rechercher une réservation par critère de recherche
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

// Fonction pour lancer une recherche de réservation par son numéro, retourne l'index de la réservation
int lanceRecherche(int num_resa_a_rechercher)
{
    struct reservation uneresa;
    int i, num_resa_trouve = VAL_INI;

    // boucle de recherche
    for (i = 0; i < nbresa; i++)
    {
        uneresa = tabresa[i];
        if (uneresa.num_r == num_resa_a_rechercher)
        {
            num_resa_trouve = i;
            break;
        }
    }
    return num_resa_trouve;
}

// // Fonction pour modifier une réservation par son numéro de réservation
 void modifierReservations()
{
    struct reservation uneresa;
    int i, num_resa_a_modifier, trouve = VAL_INI;

    trouve = lanceRecherche(num_resa_a_modifier);
    if (trouve == VAL_INI)
    {
        printf(">>> Réservation numéro %d non trouvée\n", num_resa_a_modifier);
    }
    else
    {
        uneresa = tabresa[trouve];
        printf("Réservation trouvée : \n");
        printf("%6s %-11s %-11s %7s %5s %8s\n", "N°RESA", "DATE ENTREE", "DATE SORTIE", "CHAMBRE", "PERS.", "N°CLIENT");
        printf("%-6d %-11s %-11s %-7d %-5d %-8d\n", uneresa.num_c, uneresa.date_entree, uneresa.date_sortie, uneresa.chambre, uneresa.nombre_pers, uneresa.num_r);

        printf("Nouvelle date d'entrée (jjmmyyyy)    : ");
        scanf(" %s", uneresa.date_entree);
        while (!dateValide(uneresa.date_entree))
        {
            printf(">>> Date invalide. Veuillez saisir une date (jjmmyyyy): ");
            scanf(" %s", uneresa.date_entree);
        }
        printf("Nouvelle date de sortie (jjmmyyyy)   : ");
        scanf(" %s", uneresa.date_sortie);
        while (!dateValide(uneresa.date_sortie))
        {
            printf(">>> Date invalide. Veuillez saisir une date (jjmmyyyy): ");
            scanf(" %s", uneresa.date_sortie);
        }
        printf("Nouvelle chambre à réserver          : ");
        scanf(" %d", &uneresa.chambre);
        printf("Nouveau nombre de personnes          : ");
        scanf(" %d", &uneresa.nombre_pers);
        printf(">>> Réservation modifiée \n");

        tabresa[trouve] = uneresa; // sauvegardé les données saisies dans le tableau
        a_sauvegarder = 1;         // activer le flag pour sauvegarder les données
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

// Fonction pour vérifier si une année est bissextile
bool bissextile(int annee)
{
    return ((annee % 4 == 0 && annee % 100 != 0) || annee % 400 == 0);
}

// Fonction pour vérifier si une date existe
bool dateExiste(int jour, int mois, int annee)
{
    if (mois == 2)
    {
        if (jour > 29 || (jour == 29 && !bissextile(annee)))
            return false;
    }
    else if (mois == 4 || mois == 6 || mois == 9 || mois == 11)
    {
        if (jour > 30)
            return false;
    }
    return true;
}

// Fonction pour vérifier si une date saisi est valide
bool dateValide(char date[TAILLE_DATE])
{
    int jour, mois, annee;

    if (strlen(date) != 8)
        return false;

    jour = (date[0] - '0') * 10 + (date[1] - '0');
    mois = (date[2] - '0') * 10 + (date[3] - '0');
    annee = (date[4] - '0') * 1000 + (date[5] - '0') * 100 + (date[6] - '0') * 10 + (date[7] - '0');

    if (jour < 1 || jour > 31 || mois < 1 || mois > 12 || annee < 1900 || annee > 2100)
        return false;

    if (!dateExiste(jour, mois, annee))
        return false;

    return true;
}

// Fonciton pour quitter le programme
void quitter()
{
    verifSauvegarde();
    printf("Au revoir, A bientôt !\n");
}