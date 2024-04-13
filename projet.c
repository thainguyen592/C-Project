// Fichiers d'entête
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

// Constantes
#define TAILLE_TAB 10000                                   // taille maximale des tables réservervation
#define TAILLE_NUM_RESA 8                                  // taille maximale du numéro de réservation
#define TAILLE_CHAR 20                                     // pour tout les chaines de type char
#define TAILLE_TEL 10                                      // numéro téléphone français
#define TAILLE_MAIL 40                                     // pour tout les chaines de type mail
#define TAILLE_DATE 12                                     // date pour format JJMMYYYY (8) + caractère de fin de chaine (1)
#define VAL_INI -1                                         // valeur initiale pour entrer dans le boucle
#define DB_RESERVATIONS "liste_reservations_generated.txt" // base de données pour les réservations

// Types global
struct date // structure d'une date
{
    int jour;
    int mois;
    int annee;
};

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
    int num_c; // numéro client
    struct date date_entree;
    struct date date_sortie;
    int chambre;     // numéro de chambre
    int nombre_pers; // nombre de personnes
    int num_r;       // numéro réservation
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
void supprimerReservations();
void menuRestaurant();
void convMaj(char chaine[]);
void verifSauvegarde();
bool chambreDisponible(int chambre, struct date date_entree, struct date date_sortie);
bool bissextile(int annee);
bool dateExiste(struct date d);
bool dateSuperieure(struct date d1, struct date d2);
bool dateEgale(struct date d1, struct date d2);
void dateToString(struct date d, char *dateStr);
void stringToDate(char *dateStr, struct date *d);
void stringToDate(char *dateStr, struct date *d);
int obtenirAnneeActuelle();
int genererNumResa();
void quitter();

// Programme principale
int main()
{
    chargementReservations(); // Charger les réservations depuis la base de données
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
    printf("-0- Quitter                 \n\n");
    printf("Choisissez une option : ");
}

// Fonction pour afficher le menu de gestion des réservations
void afficherMenuReservation()
{
    printf("****** Gérer les réservations ******\n");
    printf("\n");
    printf("-1- Afficher les réservation        \n");
    printf("-2- Rechercher une réservation      \n");
    printf("-3- Saisir une nouvelle réservation \n");
    printf("-4- Modifier une réservation        \n");
    printf("-5- Supprimer une réservation       \n");
    printf("-6- Sauvegarde des réservation      \n");
    printf("-0- Revenir au menu précédent       \n");
    printf("<!> %d réservations enregistrées    \n", nbresa);
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
            supprimerReservations();
            break;
        case 6:
            chargementReservations();
            break;
        case 7:
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
    uneresa.num_c = VAL_INI; // Initialiser le numéro client pour entrer dans la boucle

    while (uneresa.num_c != 0)
    {
        printf("Numéro client (0 pour terminer): ");
        scanf("%d", &uneresa.num_c);
        if (uneresa.num_c == 0)
        {
            break; // Sortie de la boucle si le numéro client est 0
        }

        // Saisir et vérifier la date d'entrée
        printf("Date d'entrée (jjmmyyyy): ");
        scanf("%2d%2d%4d", &uneresa.date_entree.jour, &uneresa.date_entree.mois, &uneresa.date_entree.annee);
        while (!dateExiste(uneresa.date_entree))
        {
            printf(">>> Date d'entrée invalide. Veuillez saisir une date valide (jjmmyyyy).\n");
            printf("Date d'entrée (jjmmyyyy): ");
            scanf("%2d%2d%4d", &uneresa.date_entree.jour, &uneresa.date_entree.mois, &uneresa.date_entree.annee);
        }

        // Saisir et vérifier la date de sortie
        printf("Date de sortie (jjmmyyyy): ");
        scanf("%2d%2d%4d", &uneresa.date_sortie.jour, &uneresa.date_sortie.mois, &uneresa.date_sortie.annee);
        while (!dateExiste(uneresa.date_sortie) || !dateSuperieure(uneresa.date_sortie, uneresa.date_entree))
        {
            printf(">>> Date de sortie invalide. Veuillez saisir une date valide (jjmmyyyy) supérieure à la date d'entrée.\n");
            printf("Date de sortie (jjmmyyyy): ");
            scanf("%2d%2d%4d", &uneresa.date_sortie.jour, &uneresa.date_sortie.mois, &uneresa.date_sortie.annee);
        }

        // Saisir et vérifier le nombre de personnes
        printf("Nombre de personnes: ");
        scanf("%d", &uneresa.nombre_pers);

        /*
        ***----------------------------------------------------------------***
           Liste des chambres disponibles aux besoins du client à afficher
        ***----------------------------------------------------------------***
        */

        // Saisir et vérifier le numéro de chambre
        printf("Chambre à réserver: ");
        scanf("%d", &uneresa.chambre);
        while (!chambreDisponible(uneresa.chambre, uneresa.date_entree, uneresa.date_sortie))
        {
            printf(">>> Chambre non disponible pour cette période. Veuillez saisir une autre chambre.\n");
            printf("Chambre à réserver: ");
            scanf("%d", &uneresa.chambre);
        }

        uneresa.num_r = genererNumResa(); // Générer un numéro de réservation unique
        printf(">>> Réservation numéro %d enregistrée\n", uneresa.num_r);

        tabresa[i++] = uneresa; // Sauvegarder les données saisies dans le tableau
        a_sauvegarder = 1;      // Activer le flag pour sauvegarder les données
    }
    nbresa = i; // Mettre à jour le nombre des réservations
}

// Fonction pour afficher tout les réservations ou 10 dernieres dans la BD
void afficherReservations()
{
    if (nbresa == 0)
    {
        printf(">>> Aucune réservation à afficher\n");
    }
    else
    {
        char reponse;
        int i, debut;
        printf("Vous voulez afficher les 10 dernières réservations ? (o/n) : ");
        scanf(" %c", &reponse);
        convMaj(&reponse);
        if (reponse == 'O' && nbresa > 10)
        {
            debut = nbresa - 10;
        }
        else
        {
            debut = 0;
        }

        // Afficher l'en-tête
        printf("%-8s %-11s %-11s %-7s %-5s %-8s\n", "N_RESA", "DATE ENTREE", "DATE SORTIE", "CHAMBRE", "PERS.", "N_CLIENT");
        for (i = debut; i < nbresa; i++)
        {
            char date_in[TAILLE_DATE];
            char date_out[TAILLE_DATE];
            dateToString(tabresa[i].date_entree, date_in);
            dateToString(tabresa[i].date_sortie, date_out);
            printf("%-8d %-11s %-11s %7d %5d %8d\n", tabresa[i].num_r, date_in, date_out, tabresa[i].chambre, tabresa[i].nombre_pers, tabresa[i].num_c);
        }
    }
}

// Fonction pour sauvegarder les réservations dans la base de données
void sauvegardeReservations()
{
    FILE *f1;
    f1 = fopen(DB_RESERVATIONS, "a+");
    if (f1 == NULL)
    {
        printf(">>> Erreur d'ouverture de la base de données\n");
        return;
    }
    for (int i = 0; i < nbresa; i++) // boucle de sauvegarde
    {
        char date_in[TAILLE_DATE];
        char date_out[TAILLE_DATE];
        dateToString(tabresa[i].date_entree, date_in);
        dateToString(tabresa[i].date_sortie, date_out);
        fprintf(f1, "%-8d %-11s %-11s %-7d %-5d %-8d\n", tabresa[i].num_r, date_in, date_out, tabresa[i].chambre, tabresa[i].nombre_pers, tabresa[i].num_c);
    }
    fclose(f1);
    a_sauvegarder = 0; // désactiver le flag pour sauvegarder les données
    printf(">>> %d réservations sauvegardées\n", nbresa);
}

// Fonction pour charger les réservations depuis la base de données
void chargementReservations()
{
    struct reservation uneresa;
    FILE *f1 = fopen(DB_RESERVATIONS, "r");
    char dateEntree[TAILLE_DATE], dateSortie[TAILLE_DATE];
    int ret;

    if (f1 == NULL)
    {
        printf("Erreur d'ouverture de la base de données.\n");
        return;
    }

    while ((ret = fscanf(f1, "%8d %11s %11s %7d %5d %8d",
                         &uneresa.num_r, dateEntree, dateSortie,
                         &uneresa.chambre, &uneresa.nombre_pers, &uneresa.num_c)) == 6)
    {
        // Conversion des chaînes de caractères en structures de dates
        stringToDate(dateEntree, &uneresa.date_entree);
        stringToDate(dateSortie, &uneresa.date_sortie);

        if (nbresa < TAILLE_TAB)
        {
            tabresa[nbresa++] = uneresa;
        }
        else
        {
            printf("Limite du tableau de réservations atteinte.\n");
            break;
        }
    }

    fclose(f1);
    printf("%d réservations chargées.\n", nbresa);
}

// Fonction pour demander le critère de recherche au utilisateur
void demanderCritereRecherche()
{
    struct reservation recherche_resa = {0}; // Initialiser la structure de recherche
    int rechercher_choix = VAL_INI;

    if (nbresa == 0)
    {
        printf(">>> Aucune réservation à afficher\n");
        return;
    }

    printf("Choisir critère de recherche : \n");
    printf("-1- Numéro de réservation\n");
    printf("-2- Numéro de client\n");
    printf("-3- Date d'entrée\n");
    printf("-4- Date de sortie\n");
    printf("-5- Chambre\n");
    printf("-0- Revenir au menu précédent\n\n");
    printf("Votre choix : ");

    while (rechercher_choix != 0)
    {
        scanf("%d", &rechercher_choix);
        switch (rechercher_choix)
        {
        case 1:
            printf("Numéro de réservation à rechercher : ");
            scanf("%d", &recherche_resa.num_r);
            rechercherReservation(recherche_resa);
            break;
        case 2:
            printf("Numéro de client à rechercher : ");
            scanf("%d", &recherche_resa.num_c);
            rechercherReservation(recherche_resa);
            break;
        case 3:
            printf("Date d'entrée à rechercher (jjmmyyyy): ");
            scanf("%2d%2d%4d", &recherche_resa.date_entree.jour, &recherche_resa.date_entree.mois, &recherche_resa.date_entree.annee);
            rechercherReservation(recherche_resa);
            break;
        case 4:
            printf("Date de sortie à rechercher (jjmmyyyy): ");
            scanf("%2d%2d%4d", &recherche_resa.date_sortie.jour, &recherche_resa.date_sortie.mois, &recherche_resa.date_sortie.annee);
            rechercherReservation(recherche_resa);
            break;
        case 5:
            printf("Chambre à rechercher : ");
            scanf("%d", &recherche_resa.chambre);
            rechercherReservation(recherche_resa);
            break;
        case 0:
            break;
        default:
            printf(">>> Option invalide. Veuillez réessayer.\n");
        }
        if (rechercher_choix != 0)
        {
            printf("Votre choix : ");
        }
    }
}

// Fonction pour rechercher une réservation par critère de recherche
void rechercherReservation(struct reservation resa_a_trouver)
{
    int nbresa_trouve = 0; // Nombre de réservations trouvées

    // Afficher l'en-tête seulement s'il y a des réservations à afficher
    if (nbresa > 0)
    {
        printf("%8s %11s %11s %7s %5s %8s\n", "N_RESA", "DATE ENTREE", "DATE SORTIE", "CHAMBRE", "PERS.", "N_CLIENT");
    }

    for (int i = 0; i < nbresa; i++)
    {
        bool match = false; // Indicateur si la réservation actuelle correspond aux critères

        // Vérifier les critères de recherche
        if ((resa_a_trouver.num_r > 0 && tabresa[i].num_r == resa_a_trouver.num_r) ||
            (resa_a_trouver.num_c > 0 && tabresa[i].num_c == resa_a_trouver.num_c) ||
            (resa_a_trouver.chambre > 0 && tabresa[i].chambre == resa_a_trouver.chambre) ||
            (dateEgale(tabresa[i].date_entree, resa_a_trouver.date_entree) && resa_a_trouver.date_entree.jour > 0) ||
            (dateEgale(tabresa[i].date_sortie, resa_a_trouver.date_sortie) && resa_a_trouver.date_sortie.jour > 0))
        {
            match = true;
        }

        // Si un match est trouvé, afficher la réservation
        if (match)
        {
            char date_in[TAILLE_DATE];
            char date_out[TAILLE_DATE];
            dateToString(tabresa[i].date_entree, date_in);
            dateToString(tabresa[i].date_sortie, date_out);
            printf("%-8d %-11s %-11s %7d %5d %8d\n", tabresa[i].num_r, date_in, date_out, tabresa[i].chambre, tabresa[i].nombre_pers, tabresa[i].num_c);
            nbresa_trouve++;
        }
    }

    if (nbresa_trouve == 0)
    {
        printf(">>> Aucune réservation trouvée correspondant aux critères.\n");
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
    int num_resa_a_modifier, trouve;

    printf("Entrez le numéro de la réservation à modifier : ");
    scanf("%d", &num_resa_a_modifier);

    trouve = lanceRecherche(num_resa_a_modifier);
    if (trouve == VAL_INI)
    {
        printf(">>> Réservation numéro %d non trouvée\n", num_resa_a_modifier);
    }
    else
    {
        uneresa = tabresa[trouve];
        char date_in[TAILLE_DATE];
        char date_out[TAILLE_DATE];
        struct date new_entree, new_sortie;

        // Afficher les informations actuelles
        dateToString(uneresa.date_entree, date_in);
        dateToString(uneresa.date_sortie, date_out);
        printf("Réservation trouvée : \n");
        printf("%8s %11s %11s %7s %5s %8s\n", "N_RESA", "DATE ENTREE", "DATE SORTIE", "CHAMBRE", "PERS.", "N_CLIENT");
        printf("%-8d %-11s %-11s %7d %5d %8d\n", uneresa.num_c, date_in, date_out, uneresa.chambre, uneresa.nombre_pers, uneresa.num_r);

        // Mise à jour de la date d'entrée
        do
        {
            printf("Nouvelle date d'entrée (jjmmyyyy): ");
            scanf("%2d%2d%4d", &new_entree.jour, &new_entree.mois, &new_entree.annee);
        } while (!dateExiste(new_entree));

        // Mise à jour de la date de sortie
        do
        {
            printf("Nouvelle date de sortie (jjmmyyyy): ");
            scanf("%2d%2d%4d", &new_sortie.jour, &new_sortie.mois, &new_sortie.annee);
        } while (!dateExiste(new_sortie) || !dateSuperieure(new_sortie, new_entree));

        // Mise à jour des autres informations
        printf("Nouvelle chambre à réserver: ");
        scanf("%d", &uneresa.chambre);
        printf("Nouveau nombre de personnes: ");
        scanf("%d", &uneresa.nombre_pers);

        // Sauvegarde des modifications
        uneresa.date_entree = new_entree;
        uneresa.date_sortie = new_sortie;
        tabresa[trouve] = uneresa;
        a_sauvegarder = 1;
        printf(">>> Réservation numéro %d modifiée\n", num_resa_a_modifier);
    }
}

// Fonction pour supprimer une réservation par son numéro de réservation
void supprimerReservations()
{
    struct reservation uneresa;
    int num_resa_a_supprimer, trouve;

    printf("Entrez le numéro de la réservation à supprimer : ");
    scanf("%d", &num_resa_a_supprimer);

    trouve = lanceRecherche(num_resa_a_supprimer);
    if (trouve == VAL_INI)
    {
        printf(">>> Réservation numéro %d non trouvée\n", num_resa_a_supprimer);
    }
    else
    {
        uneresa = tabresa[trouve];
        char date_in[TAILLE_DATE];
        char date_out[TAILLE_DATE];

        // Afficher les informations actuelles
        dateToString(uneresa.date_entree, date_in);
        dateToString(uneresa.date_sortie, date_out);
        printf("Réservation trouvée : \n");
        printf("%8s %11s %11s %7s %5s %8s\n", "N_RESA", "DATE ENTREE", "DATE SORTIE", "CHAMBRE", "PERS.", "N_CLIENT");
        printf("%-8d %-11s %-11s %7d %5d %8d\n", uneresa.num_c, date_in, date_out, uneresa.chambre, uneresa.nombre_pers, uneresa.num_r);

        // Demander confirmation pour supprimer la réservation
        char reponse[TAILLE_CHAR];
        printf("Confirmez-vous la suppression de la réservation (o/n) : ");
        scanf("%s", reponse);
        convMaj(reponse);
        if (reponse[0] == 'O')
        {
            // Supprimer la réservation
            for (int i = trouve; i < nbresa - 1; i++)
            {
                tabresa[i] = tabresa[i + 1];
            }
            nbresa--;
            a_sauvegarder = 1;
            printf(">>> Réservation numéro %d supprimée\n", num_resa_a_supprimer);
        }
    }
}

// Fonction pour afficher le menu pour la partie Restaurant
void menuRestaurant()
{
    printf("Fonction en cours de développement");
}

// Fonction pour vérifier si une chambre est disponible pour une période donnée
bool chambreDisponible(int chambre, struct date date_entree, struct date date_sortie)
{
    for (int i = 0; i < nbresa; i++)
    {
        if (tabresa[i].chambre == chambre)
        {
            if ((dateEgale(date_entree, tabresa[i].date_entree) || dateEgale(date_entree, tabresa[i].date_sortie)) ||
                (dateEgale(date_sortie, tabresa[i].date_entree) || dateEgale(date_sortie, tabresa[i].date_sortie)) ||
                (dateSuperieure(date_entree, tabresa[i].date_entree) && dateSuperieure(tabresa[i].date_sortie, date_sortie)))
            {
                return false;
            }
        }
    }
    return true;
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
bool dateExiste(struct date d)
{
    int jours_par_mois[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (d.annee < 1 || d.mois < 1 || d.mois > 12 || d.jour < 1)
        return false;
    if (d.mois == 2 && bissextile(d.annee))
        jours_par_mois[1] = 29;
    return d.jour <= jours_par_mois[d.mois - 1];
}

// Fonction pour vérifier si une date est supérieure à une autre
bool dateSuperieure(struct date d1, struct date d2)
{
    if (d1.annee > d2.annee)
        return true;
    if (d1.annee == d2.annee && d1.mois > d2.mois)
        return true;
    if (d1.annee == d2.annee && d1.mois == d2.mois && d1.jour > d2.jour)
        return true;
    return false;
}

// Fonction pour vérifier si deux dates sont égales
bool dateEgale(struct date d1, struct date d2)
{
    return (d1.annee == d2.annee && d1.mois == d2.mois && d1.jour == d2.jour);
}

// Fonction pour convertir une structure date en chaine de caractères
void dateToString(struct date d, char *dateStr)
{
    sprintf(dateStr, "%02d%02d%04d", d.jour, d.mois, d.annee);
}

// Fonction pour convertir une chaîne de caractères en structure de date
void stringToDate(char *dateStr, struct date *d)
{
    sscanf(dateStr, "%2d%2d%4d", &d->jour, &d->mois, &d->annee);
}

// Fonction pour obtenir l'année actuelle
int obtenirAnneeActuelle()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_year + 1900;
}

// Fonction pour générer un numéro de réservation unique au format "yyyyxxxx"
int genererNumResa()
{
    static int num = 1;                          // Variable statique pour garder le compte entre les appels
    static int derniere_annee = 0;               // Variable statique pour suivre l'année lors du dernier appel
    int annee_actuelle = obtenirAnneeActuelle(); // Obtention de l'année actuelle

    // Vérification si l'année a changé
    if (annee_actuelle != derniere_annee)
    {
        num = 1;                         // Réinitialisation du compteur si l'année a changé
        derniere_annee = annee_actuelle; // Mise à jour de derniere_annee à l'année actuelle
    }

    return annee_actuelle * 10000 + ++num;
}

// Fonciton pour quitter le programme
void quitter()
{
    verifSauvegarde();
    printf("Au revoir, A bientôt !\n");
}