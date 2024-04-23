// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Fichiers d'entête
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Constantes
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
#define TAILLE_TAB 10000                                   // taille maximale des tables réservervation
#define TAILLE_NUM_RESA 8                                  // taille maximale du numéro de réservation
#define TAILLE_CHAR 20                                     // pour tout les chaines de type char
#define TAILLE_TEL 16                                      // numéro téléphone (15 max) + caractère de fin de chaine (1)
#define TAILLE_MAIL 50                                     // pour tout les chaines de type mail
#define TAILLE_DATE 9                                      // date pour format JJMMYYYY (8) + caractère de fin de chaine (1)
#define VAL_INI -1                                         // valeur initiale pour entrer dans le boucle
#define DB_RESERVATIONS "liste_reservations_generated.txt" // base de données pour les réservations
#define DB_CLIENTS "liste_clients_generated.txt"           // base de données pour les clients

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Types global
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
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
    struct date date_nais;
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

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Variables globales
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
int nbresa = 0, nbclient = 0;           // initialiser le nombre des réservations et des clients
struct reservation tabresa[TAILLE_TAB]; // tableau principale des réservations
struct client tabclient[TAILLE_TAB];    // tableau principale des clients
int a_sauvegarder_reservation = 0;      // flag pour l'alerte à sauvegarder pour les réservations
int a_sauvegarder_client = 0;           // flag pour l'alerte à sauvegarder pour les clients

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Déclarations préliminaires des fonctions
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------

// Partie principale

void afficherMenuPrincipal();

// Partie Réservations

void afficherMenuReservation();
void afficherEnTeteReservations();
void menuReservation();
void afficherReservations();
void saisirReservations();
void demanderCritereRechercheReservations();
void rechercherReservation(struct reservation resa_a_trouver);
int lanceRecherche(int num_resa_a_rechercher);
void modifierReservations();
void supprimerReservations();
void sauvegardeReservations();
void chargementReservations();

// Partie Clients

void afficherMenuClient();
void afficherEnTeteClients();
void menuClient();
void afficherClients();
void saisirClient();
void demanderCritereRechercheClients();
void rechercherClient(struct client client_a_trouver);
int lanceRechercheClient(int code_client_a_rechercher);
void modifierClient();
void supprimerClient();
void sauvegardeClients();
void chargementClients();

// Autres fonctions utilitaires

bool nomValide(char nom[]);
bool telValide(char tel[]);
bool mailValide(char mail[]);
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
int genererCodeClient();
void viderBuffer();
void quitter();

// Programme principale
int main()
{
    chargementReservations(); // Charger les réservations depuis la base de données
    chargementClients();      // Charger les clients depuis la base de données
    int main_choix = VAL_INI;
    while (main_choix != 0)
    {
        afficherMenuPrincipal();
        scanf("%d", &main_choix);
        viderBuffer();
        switch (main_choix)
        {
        case 1:
            menuReservation();
            break;
        case 2:
            menuRestaurant();
            break;
        case 3:
            menuClient();
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
    printf("-3- Gérer les clients       \n");
    printf("-0- Quitter                 \n\n");
    printf("Choisissez une option : ");
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Fonctions pour la partie Restaurant
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------

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
    printf("\n");
    printf("Choisissez une option : ");
}

// Fonction pour afficher l'en-tête du tableau des réservations*
void afficherEnTeteReservations()
{
    printf("%-9s %-9s %-9s %-9s %-9s %-9s\n", "N_RESA", "D.ENTREE", "D.SORTIE", "CHAMBRE", "NB.PERS", "N_CLIENT");
}
// Fonction pour le menu Réservations
void menuReservation()
{
    int resa_choix = VAL_INI;
    while (resa_choix != 0)
    {

        afficherMenuReservation();
        scanf("%d", &resa_choix);
        viderBuffer();
        // Traitement des options
        switch (resa_choix)
        {
        case 1:
            afficherReservations();
            break;
        case 2:
            demanderCritereRechercheReservations();
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
    char reponse[TAILLE_CHAR]; // Variable pour contrôler la continuation de la boucle
    struct reservation uneresa;

    do
    {
        // Saisir et vérifier le numéro du client
        printf("Numéro client: ");
        scanf("%d", &uneresa.num_c);
        viderBuffer();

        // Saisir et vérifier la date d'entrée
        printf("Date d'entrée (jjmmyyyy): ");
        scanf("%2d%2d%4d", &uneresa.date_entree.jour, &uneresa.date_entree.mois, &uneresa.date_entree.annee);
        viderBuffer();
        while (!dateExiste(uneresa.date_entree))
        {
            printf(">>> Date d'entrée invalide. Veuillez saisir une date valide (jjmmyyyy).\n");
            printf("Date d'entrée (jjmmyyyy): ");
            scanf("%2d%2d%4d", &uneresa.date_entree.jour, &uneresa.date_entree.mois, &uneresa.date_entree.annee);
            viderBuffer();
        }

        // Saisir et vérifier la date de sortie
        printf("Date de sortie (jjmmyyyy): ");
        scanf("%2d%2d%4d", &uneresa.date_sortie.jour, &uneresa.date_sortie.mois, &uneresa.date_sortie.annee);
        viderBuffer();
        while (!dateExiste(uneresa.date_sortie) || !dateSuperieure(uneresa.date_sortie, uneresa.date_entree))
        {
            printf(">>> Date de sortie invalide ou antérieure à la date d'entrée. Veuillez saisir une date valide (jjmmyyyy) supérieure à la date d'entrée.\n");
            printf("Date de sortie (jjmmyyyy): ");
            scanf("%2d%2d%4d", &uneresa.date_sortie.jour, &uneresa.date_sortie.mois, &uneresa.date_sortie.annee);
            viderBuffer();
        }

        // Saisir et vérifier le nombre de personnes
        printf("Nombre de personnes: ");
        scanf("%d", &uneresa.nombre_pers);
        viderBuffer();

        // Saisir et vérifier le numéro de chambre
        printf("Chambre à réserver: ");
        scanf("%d", &uneresa.chambre);
        viderBuffer();
        while (!chambreDisponible(uneresa.chambre, uneresa.date_entree, uneresa.date_sortie))
        {
            printf(">>> Chambre non disponible pour cette période. Veuillez saisir une autre chambre.\n");
            printf("Chambre à réserver: ");
            scanf("%d", &uneresa.chambre);
            viderBuffer();
        }

        uneresa.num_r = genererNumResa(); // Générer un numéro de réservation unique
        printf(">>> Réservation numéro %d enregistrée\n", uneresa.num_r);

        tabresa[i++] = uneresa;        // Sauvegarder les données saisies dans le tableau
        a_sauvegarder_reservation = 1; // Activer le flag pour sauvegarder les données

        // Demander à l'utilisateur s'il souhaite continuer
        printf("Voulez-vous saisir une autre réservation ? (o/n) : ");
        scanf("%s", reponse); // L'espace avant %c permet de sauter les blancs comme les retours à la ligne
        viderBuffer();
        convMaj(reponse);
    } while (reponse[0] == 'O');

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
        char reponse[TAILLE_CHAR];
        int i, debut = 0;

        printf("<!> %d réservations enregistrées dans la base\n", nbresa);
        printf("\n");
        if (nbresa > 10)
        {
            printf("Vous voulez afficher les 10 dernières réservations ? (o/n) : ");
            scanf("%s", reponse);
            viderBuffer();
            convMaj(reponse);
            if (reponse[0] == 'O')
            {
                debut = nbresa - 10;
            }
        }

        afficherEnTeteReservations();
        for (i = debut; i < nbresa; i++)
        {
            char date_in[TAILLE_DATE];
            char date_out[TAILLE_DATE];
            dateToString(tabresa[i].date_entree, date_in);
            dateToString(tabresa[i].date_sortie, date_out);
            printf("%-9d %-9s %-9s %-9d %-9d %-9d\n", tabresa[i].num_r, date_in, date_out, tabresa[i].chambre, tabresa[i].nombre_pers, tabresa[i].num_c);
        }
        printf("\n");
    }
}

// Fonction pour sauvegarder les réservations dans la base de données
void sauvegardeReservations()
{
    FILE *f1;
    f1 = fopen(DB_RESERVATIONS, "a");
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
        fprintf(f1, "%-9d %-9s %-9s %-9d %-9d %-9d\n", tabresa[i].num_r, date_in, date_out, tabresa[i].chambre, tabresa[i].nombre_pers, tabresa[i].num_c);
    }
    fclose(f1);
    a_sauvegarder_reservation = 0; // désactiver le flag pour sauvegarder les données
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

    while ((ret = fscanf(f1, "%9d %9s %9s %9d %9d %9d", &uneresa.num_r, dateEntree, dateSortie, &uneresa.chambre, &uneresa.nombre_pers, &uneresa.num_c)) == 6)
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

// Fonction pour demander le critère de recherche au utilisateur pour les réservations
void demanderCritereRechercheReservations()
{
    struct reservation recherche_resa = {0}; // Initialiser la structure de recherche
    int rechercher_choix = VAL_INI;

    if (nbresa == 0)
    {
        printf(">>> Aucune réservation à afficher\n");
        return;
    }

    while (rechercher_choix != 0)
    {
        printf("Choisir critère de recherche : \n");
        printf("-1- Numéro de réservation\n");
        printf("-2- Numéro de client\n");
        printf("-3- Date d'entrée\n");
        printf("-4- Date de sortie\n");
        printf("-5- Chambre\n");
        printf("-0- Revenir au menu précédent\n\n");
        printf("Votre choix : ");
        scanf("%d", &rechercher_choix);
        viderBuffer();
        switch (rechercher_choix)
        {
        case 1:
            printf("Numéro de réservation à rechercher : ");
            scanf("%d", &recherche_resa.num_r);
            viderBuffer();
            rechercherReservation(recherche_resa);
            break;
        case 2:
            printf("Numéro de client à rechercher : ");
            scanf("%d", &recherche_resa.num_c);
            viderBuffer();
            rechercherReservation(recherche_resa);
            break;
        case 3:
            printf("Date d'entrée à rechercher (jjmmyyyy): ");
            scanf("%2d%2d%4d", &recherche_resa.date_entree.jour, &recherche_resa.date_entree.mois, &recherche_resa.date_entree.annee);
            viderBuffer();
            rechercherReservation(recherche_resa);
            break;
        case 4:
            printf("Date de sortie à rechercher (jjmmyyyy): ");
            scanf("%2d%2d%4d", &recherche_resa.date_sortie.jour, &recherche_resa.date_sortie.mois, &recherche_resa.date_sortie.annee);
            viderBuffer();
            rechercherReservation(recherche_resa);
            break;
        case 5:
            printf("Chambre à rechercher : ");
            scanf("%d", &recherche_resa.chambre);
            viderBuffer();
            rechercherReservation(recherche_resa);
            break;
        case 0:
            break;
        default:
            printf(">>> Option invalide. Veuillez réessayer.\n");
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
        afficherEnTeteReservations();
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
            printf("%-9d %-9s %-9s %-9d %-9d %-9d\n", tabresa[i].num_r, date_in, date_out, tabresa[i].chambre, tabresa[i].nombre_pers, tabresa[i].num_c);
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

// Fonction pour modifier une réservation par son numéro de réservation
void modifierReservations()
{
    struct reservation uneresa;
    int num_resa_a_modifier, trouve;

    printf("Entrez le numéro de la réservation à modifier : ");
    scanf("%d", &num_resa_a_modifier);
    viderBuffer();

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
        afficherEnTeteReservations();
        printf("%-9d %-9s %-9s %-9d %-9d %-9d\n", uneresa.num_r, date_in, date_out, uneresa.chambre, uneresa.nombre_pers, uneresa.num_c);

        // Mise à jour de la date d'entrée
        do
        {
            printf("Nouvelle date d'entrée (jjmmyyyy): ");
            scanf("%2d%2d%4d", &new_entree.jour, &new_entree.mois, &new_entree.annee);
            viderBuffer();
        } while (!dateExiste(new_entree));

        // Mise à jour de la date de sortie
        do
        {
            printf("Nouvelle date de sortie (jjmmyyyy): ");
            scanf("%2d%2d%4d", &new_sortie.jour, &new_sortie.mois, &new_sortie.annee);
            viderBuffer();
        } while (!dateExiste(new_sortie) || !dateSuperieure(new_sortie, new_entree));

        // Mise à jour nombre de personnes
        printf("Nouveau nombre de personnes: ");
        scanf("%d", &uneresa.nombre_pers);
        viderBuffer();

        // Mise à jour de la chambre
        printf("Nouvelle chambre à réserver: ");
        scanf("%d", &uneresa.chambre);
        viderBuffer();
        while (!chambreDisponible(uneresa.chambre, new_entree, new_sortie))
        {
            printf(">>> Chambre non disponible pour cette période. Veuillez saisir une autre chambre.\n");
            printf("Nouvelle chambre à réserver: ");
            scanf("%d", &uneresa.chambre);
            viderBuffer();
        }

        // Sauvegarde des modifications
        uneresa.date_entree = new_entree;
        uneresa.date_sortie = new_sortie;
        tabresa[trouve] = uneresa;
        a_sauvegarder_reservation = 1;
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
    viderBuffer();

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
        afficherEnTeteReservations();
        printf("%-9d %-9s %-9s %-9d %-9d %-9d\n", uneresa.num_c, date_in, date_out, uneresa.chambre, uneresa.nombre_pers, uneresa.num_r);

        // Demander confirmation pour supprimer la réservation
        char reponse[TAILLE_CHAR];
        printf("Confirmez-vous la suppression de la réservation (o/n) : ");
        scanf("%s", reponse);
        viderBuffer();
        convMaj(reponse);
        if (reponse[0] == 'O')
        {
            // Supprimer la réservation
            for (int i = trouve; i < nbresa - 1; i++)
            {
                tabresa[i] = tabresa[i + 1];
            }
            nbresa--;
            a_sauvegarder_reservation = 1;
            printf(">>> Réservation numéro %d supprimée\n", num_resa_a_supprimer);
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Fonctions pour la partie Client
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------

// Fonction pour afficher le menu de gestion des clients
void afficherMenuClient()
{
    printf("****** Gérer les clients ******\n");
    printf("\n");
    printf("-1- Afficher les clients        \n");
    printf("-2- Rechercher un client         \n");
    printf("-3- Saisir un nouveau client     \n");
    printf("-4- Modifier un client           \n");
    printf("-5- Supprimer un client          \n");
    printf("-6- Sauvegarde des clients       \n");
    printf("-0- Revenir au menu précédent    \n");
    printf("\n");
    printf("Choisissez une option : ");
}

// Fonction pour afficher l'en-tête du tableau des clients
void afficherEnTeteClients()
{
    printf("%-6s %-20s %-20s %-9s %-16s %-s\n", "CODE", "NOM", "PRENOM", "DATE_NAIS", "TEL", "MAIL");
}

// Fonction pour le menu Clients
void menuClient()
{
    int client_choix = VAL_INI;
    while (client_choix != 0)
    {
        afficherMenuClient();
        scanf("%d", &client_choix);
        viderBuffer();
        // Traitement des options
        switch (client_choix)
        {
        case 1:
            afficherClients();
            break;
        case 2:
            demanderCritereRechercheClients();
            break;
        case 3:
            saisirClient();
            break;
        case 4:
            modifierClient();
            break;
        case 5:
            supprimerClient();
            break;
        case 6:
            sauvegardeClients();
            break;
        case 0:
            break;
        default:
            printf(">>> Option invalide. Veuillez réessayer.\n");
        }
    }
}

// Fonction pour afficher tout les clients ou 10 dernieres dans la BD
void afficherClients()
{
    if (nbclient == 0)
    {
        printf(">>> Aucun client à afficher\n");
    }
    else
    {
        char reponse[TAILLE_CHAR];
        int i, debut = 0;

        printf("<!> %d clients enregistrés dans la base\n", nbclient);
        printf("\n");
        if (nbclient > 10)
        {
            printf("Vous voulez afficher les 10 derniers clients ? (o/n) : ");
            scanf(" %s", reponse);
            viderBuffer();
            convMaj(reponse);
            if (reponse[0] == 'O')
            {
                debut = nbclient - 10;
            }
        }

        afficherEnTeteClients();
        for (i = debut; i < nbclient; i++)
        {
            char date_naissance[TAILLE_DATE];
            dateToString(tabclient[i].date_nais, date_naissance);
            printf("%-6d %-20s %-20s %-9s %-16s %-s\n", tabclient[i].code, tabclient[i].nom, tabclient[i].prenom, date_naissance, tabclient[i].tel, tabclient[i].mail);
        }
        printf("\n");
    }
}

// Fonction pour la saisir d'un nouveau client
void saisirClient()
{
    int i = nbclient;
    struct client unclient;
    char reponse[TAILLE_CHAR]; // Variable pour contrôler la continuation de la boucle

    // boucle de saisie pour un nouveau client
    do
    {
        // Saisir le nom du client
        do
        {
            printf("Nom : ");
            scanf(" %[^\n]", unclient.nom); // %[^\n] permet de lire une chaîne de caractères avec des espaces
            viderBuffer();
            if (!nomValide(unclient.nom))
            {
                printf("Nom invalide, veuillez saisir un nom valide.\n");
            }
        } while (!nomValide(unclient.nom));
        convMaj(unclient.nom);

        // Saisir le prénom du client
        do
        {
            printf("Prénom : ");
            scanf(" %[^\n]", unclient.prenom);
            viderBuffer();
            if (!nomValide(unclient.prenom))
            {
                printf("Prénom invalide, veuillez saisir un prénom valide.\n");
            }
        } while (!nomValide(unclient.prenom));
        convMaj(unclient.prenom);

        // Saisir la date de naissance du client
        do
        {
            printf("Date de naissance (jjmmyyyy) : ");
            scanf("%2d%2d%4d", &unclient.date_nais.jour, &unclient.date_nais.mois, &unclient.date_nais.annee);
            viderBuffer();
            if (!dateExiste(unclient.date_nais))
            {
                printf("Date de naissance invalide, veuillez saisir une date valide (jjmmyyyy).\n");
            }
        } while (!dateExiste(unclient.date_nais));

        // Saisir le numéro de téléphone du client
        do
        {
            printf("Téléphone (saisis '00' au lieu de '+') : ");
            scanf("%s", unclient.tel);
            viderBuffer();
            if (!telValide(unclient.tel))
            {
                printf("Téléphone invalide, veuillez saisir un numéro de téléphone valide.\n");
            }
        } while (!telValide(unclient.tel));

        // Saisir l'adresse mail du client
        do
        {
            printf("Adresse mail : ");
            scanf("%s", unclient.mail);
            viderBuffer();
            if (!mailValide(unclient.mail))
            {
                printf("Adresse mail invalide, veuillez saisir une adresse mail valide.\n");
            }
        } while (!mailValide(unclient.mail));

        // Générer un code client unique
        unclient.code = genererCodeClient();

        // Afficher les informations saisies

        printf(">>> Client numéro %d enregistré.\n", unclient.code);

        tabclient[i++] = unclient; // Sauvegarder les données saisies dans le tableau
        a_sauvegarder_client = 1;  // Activer le flag pour sauvegarder les données

        // Demander à l'utilisateur s'il souhaite continuer
        printf("Voulez-vous saisir un autre client ? (o/n) : ");
        scanf("%s", reponse);
        viderBuffer();
        convMaj(reponse);
    } while (reponse[0] == 'O');

    nbclient = i; // Mettre à jour le nombre des clients
}

// Fonction pour demander le critère de recherche au utilisateur pour les clients
void demanderCritereRechercheClients()
{
    struct client recherche_client = {0}; // Initialiser la structure de recherche
    int rechercher_choix = VAL_INI;

    if (nbclient == 0)
    {
        printf(">>> Aucun client à afficher\n");
        return;
    }

    while (rechercher_choix != 0)
    {
        printf("Choisir critère de recherche : \n");
        printf("-1- Code client\n");
        printf("-2- Nom\n");
        printf("-3- Prénom\n");
        printf("-4- Date de naissance\n");
        printf("-5- Téléphone\n");
        printf("-6- Mail\n");
        printf("-0- Revenir au menu précédent\n\n");
        printf("Votre choix : ");
        scanf("%d", &rechercher_choix);
        viderBuffer();
        switch (rechercher_choix)
        {
        case 1:
            printf("Code client à rechercher : ");
            scanf("%d", &recherche_client.code);
            viderBuffer();
            rechercherClient(recherche_client);
            break;
        case 2:
            printf("Nom à rechercher : ");
            scanf("%s", recherche_client.nom);
            viderBuffer();
            rechercherClient(recherche_client);
            break;
        case 3:
            printf("Prénom à rechercher : ");
            scanf("%s", recherche_client.prenom);
            viderBuffer();
            rechercherClient(recherche_client);
            break;
        case 4:
            printf("Date de naissance à rechercher (jjmmyyyy): ");
            scanf("%2d%2d%4d", &recherche_client.date_nais.jour, &recherche_client.date_nais.mois, &recherche_client.date_nais.annee);
            viderBuffer();
            rechercherClient(recherche_client);
            break;
        case 5:
            printf("Téléphone à rechercher : ");
            scanf("%s", recherche_client.tel);
            viderBuffer();
            rechercherClient(recherche_client);
            break;
        case 6:
            printf("Mail à rechercher : ");
            scanf("%s", recherche_client.mail);
            viderBuffer();
            rechercherClient(recherche_client);
            break;
        case 0:
            break;
        default:
            printf(">>> Option invalide. Veuillez réessayer.\n");
        }
    }
}

// Fonction pour rechercher un client par critère de recherche
void rechercherClient(struct client client_a_trouver)
{
    int nbclient_trouve = 0; // Nombre de clients trouvés

    // Afficher l'en-tête seulement s'il y a des clients à afficher
    if (nbclient > 0)
    {
        afficherEnTeteClients();
    }

    for (int i = 0; i < nbclient; i++)
    {
        bool match = false; // Indicateur si le client actuel correspond aux critères

        // Vérifier les critères de recherche
        if ((client_a_trouver.code > 0 && tabclient[i].code == client_a_trouver.code) ||
            (strlen(client_a_trouver.nom) > 0 && strcmp(tabclient[i].nom, client_a_trouver.nom) == 0) ||
            (strlen(client_a_trouver.prenom) > 0 && strcmp(tabclient[i].prenom, client_a_trouver.prenom) == 0) ||
            (dateEgale(tabclient[i].date_nais, client_a_trouver.date_nais) && client_a_trouver.date_nais.jour > 0) ||
            (strlen(client_a_trouver.tel) > 0 && strcmp(tabclient[i].tel, client_a_trouver.tel) == 0) ||
            (strlen(client_a_trouver.mail) > 0 && strcmp(tabclient[i].mail, client_a_trouver.mail) == 0))
        {
            match = true;
        }

        // Si un match est trouvé, afficher le client
        if (match)
        {
            char date_nais[TAILLE_DATE];
            dateToString(tabclient[i].date_nais, date_nais);
            printf("%-6d %-20s %-20s %-9s %-16s %-s\n", tabclient[i].code, tabclient[i].nom, tabclient[i].prenom, date_nais, tabclient[i].tel, tabclient[i].mail);
            nbclient_trouve++;
        }
    }

    if (nbclient_trouve == 0)
    {
        printf(">>> Aucun client trouvé correspondant aux critères.\n");
    }
}

// Fonction pour rechercher un client par son code
int lanceRechercheClient(int code_client_a_rechercher)
{
    struct client unclient;
    int i, code_client_trouve = VAL_INI;

    // boucle de recherche
    for (i = 0; i < nbclient; i++)
    {
        unclient = tabclient[i];
        if (unclient.code == code_client_a_rechercher)
        {
            code_client_trouve = i;
            break;
        }
    }
    return code_client_trouve;
}

// Fonction pour modifier un client par son code
void modifierClient()
{
    struct client unclient;
    int code_client_a_modifier, trouve;

    printf("Entrez le code du client à modifier : ");
    scanf("%d", &code_client_a_modifier);
    viderBuffer();

    trouve = lanceRechercheClient(code_client_a_modifier);
    if (trouve == VAL_INI)
    {
        printf(">>> Client code %d non trouvé\n", code_client_a_modifier);
    }
    else
    {
        unclient = tabclient[trouve];
        char date_nais[TAILLE_DATE];
        struct date new_date_nais;

        // Afficher les informations actuelles
        dateToString(unclient.date_nais, date_nais);
        printf("Client trouvé : \n");
        afficherEnTeteClients();
        printf("%-6d %-20s %-20s %-9s %-10s %-s\n", unclient.code, unclient.nom, unclient.prenom, date_nais, unclient.tel, unclient.mail);

        // mise à jour des informations
        do
        {
            printf("Nouveau nom : ");
            scanf(" %[^\n]", unclient.nom); // %[^\n] permet de lire une chaîne de caractères avec des espaces
            viderBuffer();
            if (!nomValide(unclient.nom))
            {
                printf("Nom invalide, veuillez saisir un nom valide.\n");
            }
        } while (!nomValide(unclient.nom));
        convMaj(unclient.nom);

        do
        {
            printf("Nouveau prénom : ");
            scanf(" %[^\n]", unclient.prenom);
            viderBuffer();
            if (!nomValide(unclient.prenom))
            {
                printf("Prénom invalide, veuillez saisir un prénom valide.\n");
            }
        } while (!nomValide(unclient.prenom));
        convMaj(unclient.prenom);

        do
        {
            printf("Nouvelle date de naissance (jjmmyyyy) : ");
            scanf("%2d%2d%4d", &new_date_nais.jour, &new_date_nais.mois, &new_date_nais.annee);
            viderBuffer();
            if (!dateExiste(new_date_nais))
            {
                printf("Date de naissance invalide, veuillez saisir une date valide (jjmmyyyy).\n");
            }
        } while (!dateExiste(new_date_nais));

        do
        {
            printf("Nouveau numéro de téléphone : ");
            scanf("%s", unclient.tel);
            viderBuffer();
            if (!telValide(unclient.tel))
            {
                printf("Téléphone invalide, veuillez saisir un numéro de téléphone valide.\n");
            }
        } while (!telValide(unclient.tel));

        do
        {
            printf("Nouvelle adresse mail : ");
            scanf("%s", unclient.mail);
            viderBuffer();
            if (!mailValide(unclient.mail))
            {
                printf("Adresse mail invalide, veuillez saisir une adresse mail valide.\n");
            }
        } while (!mailValide(unclient.mail));

        // Sauvegarde des modifications
        unclient.date_nais = new_date_nais;
        tabclient[trouve] = unclient;
        a_sauvegarder_client = 1;
        printf(">>> Client code %d modifié\n", code_client_a_modifier);
    }
}

// Fonction pour supprimer un client par son code
void supprimerClient()
{
    struct client unclient;
    int code_client_a_supprimer, trouve;

    printf("Entrez le code du client à supprimer : ");
    scanf("%d", &code_client_a_supprimer);
    viderBuffer();

    trouve = lanceRechercheClient(code_client_a_supprimer);
    if (trouve == VAL_INI)
    {
        printf(">>> Client code %d non trouvé\n", code_client_a_supprimer);
    }
    else
    {
        unclient = tabclient[trouve];
        char date_nais[TAILLE_DATE];

        // Afficher les informations actuelles
        dateToString(unclient.date_nais, date_nais);
        printf("Client trouvé : \n");
        afficherEnTeteClients();
        printf("%-6d %-20s %-20s %-9s %-10s %-s\n", unclient.code, unclient.nom, unclient.prenom, date_nais, unclient.tel, unclient.mail);

        // Demander confirmation pour supprimer le client
        char reponse[TAILLE_CHAR];
        printf("Confirmez-vous la suppression du client (o/n) : ");
        scanf("%s", reponse);
        viderBuffer();
        convMaj(reponse);
        if (reponse[0] == 'O')
        {
            // Supprimer le client
            for (int i = trouve; i < nbclient - 1; i++)
            {
                tabclient[i] = tabclient[i + 1];
            }
            nbclient--;
            a_sauvegarder_client = 1;
            printf(">>> Client code %d supprimé\n", code_client_a_supprimer);
        }
    }
}

// Fonction pour sauvegarder les clients dans la base de données
void sauvegardeClients()
{
    FILE *f1;
    f1 = fopen(DB_CLIENTS, "a");
    if (f1 == NULL)
    {
        printf(">>> Erreur d'ouverture de la base de données\n");
        return;
    }
    for (int i = 0; i < nbclient; i++) // boucle de sauvegarde
    {
        char date_nais[TAILLE_DATE];
        dateToString(tabclient[i].date_nais, date_nais);
        fprintf(f1, "%-6d %-20s %-20s %-9s %-10s %-s\n", tabclient[i].code, tabclient[i].nom, tabclient[i].prenom, date_nais, tabclient[i].tel, tabclient[i].mail);
    }
    fclose(f1);
    a_sauvegarder_client = 0; // désactiver le flag pour sauvegarder les données
    printf(">>> %d clients sauvegardés\n", nbclient);
}

// Fonction pour charger les clients depuis la base de données
void chargementClients()
{
    struct client unclient;
    FILE *f1 = fopen(DB_CLIENTS, "r");
    char dateNais[TAILLE_DATE];
    int ret;

    if (f1 == NULL)
    {
        printf("Erreur d'ouverture de la base de données.\n");
        return;
    }

    while ((ret = fscanf(f1, "%6d %20s %20s %9s %10s %s\n", &unclient.code, unclient.nom, unclient.prenom, dateNais, unclient.tel, unclient.mail)) == 6)
    {
        // Conversion des chaînes de caractères en structures de dates
        stringToDate(dateNais, &unclient.date_nais);

        if (nbclient < TAILLE_TAB)
        {
            tabclient[nbclient++] = unclient;
        }
        else
        {
            printf("Limite du tableau de clients atteinte.\n");
            break;
        }
    }

    fclose(f1);
    printf("%d clients chargés.\n", nbclient);
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Fonctions pour la partie Restaurant
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------

// Fonction pour afficher le menu pour la partie Restaurant
void menuRestaurant()
{
    printf("Fonction en cours de développement");
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Fonctions utilitaires
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------

// Fonction pour vérifier si le nom ou prénom d'un client est valide
bool nomValide(char nom[])
{
    int i, taille;
    taille = strlen(nom);
    if (taille == 0 || taille > TAILLE_CHAR)
        return false;
    for (i = 0; i < taille; i++)
    {
        if (!isalpha(nom[i]) && nom[i] != '-' && nom[i] != ' ')
            return false;
    }
    return true;
}

// Fonction pour vérifier si un numéro de téléphone est valide
bool telValide(char tel[])
{
    for (int i = 0; i < strlen(tel); i++)
    {
        if (!isdigit(tel[i]))
            return false;
    }
    return true;
}

// Fonction pour vérifier si une adresse mail est valide
bool mailValide(char mail[])
{
    int i, taille;
    taille = strlen(mail);
    int nbat = 0;
    int nbdot = 0;

    if (taille == 0 || taille > TAILLE_MAIL)
        return false;
    for (i = 0; i < taille; i++)
    {
        if (!isalnum(mail[i]) && mail[i] != '@' && mail[i] != '.' && mail[i] != '_')
            return false;
        if (mail[i] == '@')
            nbat++;
        if (mail[i] == '.')
            nbdot++;
    }
    if (nbat != 1 || nbdot == 0)
        return false;
    return true;
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

    if (a_sauvegarder_reservation || a_sauvegarder_client)
    {
        printf("Des données ont été modifiées\n");
        printf("Voulez-vous faire une sauvegarde (o/n) : ");
        scanf("%s", reponse);
        viderBuffer();
        convMaj(reponse);

        if (reponse[0] == 'O')
        {
            if (a_sauvegarder_reservation)
            {
                sauvegardeReservations();
            }
            if (a_sauvegarder_client)
            {
                sauvegardeClients();
            }
        }
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

// Fonction pour générer un code client unique
int genererCodeClient()
{
    static int code = 1; // Variable statique pour garder le compte entre les appels
    return code++;
}

void viderBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Fonciton pour quitter le programme
void quitter()
{
    verifSauvegarde();
    printf("Au revoir, A bientôt !\n");
}