// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Fichiers d'entête
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Constantes
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
#define TAILLE_TAB 10000     // taille maximale des tables réservervation
#define TAILLE_TAB_PC 100000 // taille maximale des tables clients
#define TAILLE_PROD 100      // taille maximale des produits
#define TAILLE_DESC 40       // taille maximale de la description d'un produit
#define TAILLE_NUM_RESA 8    // taille maximale du numéro de réservation
#define TAILLE_CHAR 20       // pour tout les chaines de type char
#define TAILLE_TEL 16        // numéro téléphone (15 max) + caractère de fin de chaine (1)
#define TAILLE_MAIL 50       // pour tout les chaines de type mail
#define TAILLE_DATE 9        // date pour format JJMMYYYY (8) + caractère de fin de chaine (1)

#define VAL_INI -1 // valeur initiale pour entrer dans le boucle

#define DB_RESERVATIONS "liste_reservations.txt" // base de données pour les réservations
#define DB_CLIENTS "liste_clients.txt"           // base de données pour les clients
#define DB_FACTURES "liste_factures.txt"         // base de données pour les factures
#define DB_PRODUITS "liste_produits.txt"         // base de données pour les produits
#define DB_PC "liste_pc.txt"                     // base de données pour les produits commandés

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Types global
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
struct date // structure d'une date
{
    int jour;
    int mois;
    int annee;
};

struct produit_commande // structure des produits commandés
{
    int num_r; // numéro réservation
    int code;  // code produit
    int qte;   // quantité commandée
};

struct client // structure d'un client
{
    int code;                 // code client
    int fidelite;             // niveau de fidélité
    char nom[TAILLE_CHAR];    // nom
    char prenom[TAILLE_CHAR]; // prénom
    struct date date_nais;    // date de naissance
    char tel[TAILLE_TEL];     // numéro de téléphone
    char mail[TAILLE_MAIL];   // adresse mail
    float total;              // total dépensé par le client
};

struct reservation // structure d'une réservation
{
    int num_r;               // numéro réservation
    struct date date_entree; // date d'entrée
    struct date date_sortie; // date de sortie
    int chambre;             // numéro de chambre
    int nombre_pers;         // nombre de personnes
    int num_c;               // numéro client
};

struct facture // structure d'une facture
{
    int num_f;                 // numéro facture
    int num_r;                 // numéro réservation
    float total;               // total de la facture
    struct date date_fact;     // date de la facture
    struct date date_paiement; // date de paiement de la facture
    int statut;                // 0: non payée, 1: payée
};

struct produit // structure d'un produit
{
    int code;               // code produit
    char desc[TAILLE_DESC]; // description du produit
    float prix;             // prix du produit
};

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Variables globales
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
int nbresa = 0;    // nombre de réservations
int nbclient = 0;  // nombre de clients
int nbfacture = 0; // nombre de factures
int nbproduit = 0; // nombre de produits
int nbpc = 0;      // nombre de produits commandés

struct reservation tabresa[TAILLE_TAB];       // tableau principale des réservations
struct client tabclient[TAILLE_TAB];          // tableau principale des clients
struct facture tabfacture[TAILLE_TAB];        // tableau principale des factures
struct produit tabproduit[TAILLE_TAB];        // tableau principale des produits
struct produit_commande tabpc[TAILLE_TAB_PC]; // tableau principale des produits commandés

int a_sauvegarder_reservation = 0; // flag pour l'alerte à sauvegarder pour les réservations
int a_sauvegarder_client = 0;      // flag pour l'alerte à sauvegarder pour les clients
int a_sauvegarder_facture = 0;     // flag pour l'alerte à sauvegarder pour les factures
int a_sauvegarder_produit = 0;     // flag pour l'alerte à sauvegarder pour les produits
int a_sauvegarder_pc = 0;          // flag pour l'alerte à sauvegarder pour les produits commandés

int num_resa = 0;    // numéro de séquence de réservation
int num_client = 0;  // numéro de séquence de client
int num_facture = 0; // numéro de séquence de facture
int num_produit = 0; // numéro de séquence de produit

int annee_dernier_resa = 0; // année du système

int listeChambre[50] = {
    101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
    201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
    301, 302, 303, 304, 305, 306, 307, 308, 309, 310,
    401, 402, 403, 404, 405, 406, 407, 408, 409, 410,
    501, 502, 503, 504, 505, 506, 507, 508}; // Liste des chambres

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
void genererPC(int idx_resa);
void nettoyerPC(int idx_resa);

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

// Partie Restaurant&Services
void afficherMenuPC();
void menuPC();
void afficherPC(int idx_resa);
int lanceRecherchePC(int idx_resa, int code_pc);
void rechercherPC(int idx_resa);
void ajouterPC(int idx_resa);
void modifierPC(int idx_resa);
void supprimerPC(int idx_resa);
void sauvegardePC();
void chargementPC();

// Partie Factures
void afficherMenuFacturation();
void menuFacturation();
void facturerReservation();
void afficherEnTeteFactures();
void afficherFactures();
void demanderCritereRechercheFacture();
void rechercherFacture(struct facture facture_a_trouver);
int lanceRechercheFacture(int num_facture_a_rechercher);
int lanceRechercheFactureParReservation(int num_reservation_a_rechercher);
void paiementFacture();
void supprimerFacture();
void sauvegardeFactures();
void chargementFactures();

// Partie Produits
void afficherMenuProduits();
void menuProduits();
void afficherProduits();
void afficherEnTeteProduits();
void saisirProduit();
void demanderCritereRechercheProduits();
void rechercherProduit(struct produit produit_a_trouver);
int lanceRechercheProduit(int code_produit_a_rechercher);
void modifierProduit();
void supprimerProduit();
void sauvegardeProduits();
void chargementProduits();

// Autres fonctions utilitaires

bool nomValide(char nom[]);
bool telValide(char tel[]);
bool mailValide(char mail[]);
bool chambreValide(int chambre);
void convMaj(char chaine[]);
void verifSauvegarde();
bool chambreDisponible(int chambre, struct date date_entree, struct date date_sortie);
void afficherChambresDisponibles(struct date date_entree, struct date date_sortie);
int codeProduitChambre(int chambre);
bool bissextile(int annee);
bool dateExiste(struct date d);
bool dateSuperieure(struct date d1, struct date d2);
bool dateEgale(struct date d1, struct date d2);
void dateToString(struct date d, char *dateStr);
void stringToDate(char *dateStr, struct date *d);
void stringToDate(char *dateStr, struct date *d);
int obtenirAnneeActuelle();
void obtenirDateActuelle(struct date *d);
void obtenirDateDuJour(char *dateStr);
int nbJoursEntreDates(struct date d1, struct date d2);
int genererNumResa();
int genererCodeClient();
int genererNumFacture();
bool codeProduitValide(int code);
char *niveauFideliteToString(int nf);
int calculerFidelite(float total);
float appliquerFidelite(float total, int numero_client);
char *statutFactureToString(int sf);
void viderBuffer();
void quitter();

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Programme principale
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
    setlocale(LC_ALL, "fr_FR.UTF-8");              // pour afficher les caractères accentués
    setlocale(LC_ALL, "C");                        // pour afficher . comme séparateur décimal
    chargementReservations();                      // Charger les réservations depuis la base de données
    num_resa = tabresa[nbresa - 1].num_r;          // Récupérer le dernier numéro de réservation
    chargementClients();                           // Charger les clients depuis la base de données
    num_client = tabclient[nbclient - 1].code;     // Récupérer le dernier code client
    chargementProduits();                          // Charger les produits depuis la base de données
    num_produit = tabproduit[nbproduit - 1].code;  // Récupérer le dernier code produit
    chargementPC();                                // Charger les produits commandés depuis la base de données
    chargementFactures();                          // Charger les factures depuis la base de données
    num_facture = tabfacture[nbfacture - 1].num_f; // Récupérer le dernier numéro de facture
    annee_dernier_resa = obtenirAnneeActuelle();   // Récupérer l'année du système
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
            menuPC();
            break;
        case 3:
            menuClient();
            break;
        case 4:
            menuFacturation();
            break;
        case 5:
            menuProduits();
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
    printf("-2- Gérer le restaurant & services\n");
    printf("-3- Gérer les clients       \n");
    printf("-4- Gérer la facturation    \n");
    printf("-5- Gérer les produits      \n");
    printf("-0- Quitter               \n\n");
    printf("Choisissez une option : ");
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Fonctions pour la partie Reservation
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------

// Fonction pour afficher le menu de gestion des réservations
void afficherMenuReservation()
{
    printf("****** Gérer les réservations ******\n\n");
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
        lanceRechercheClient(uneresa.num_c);
        if (lanceRechercheClient(uneresa.num_c) == VAL_INI)
        {
            printf(">>> Client non trouvé. Veuillez saisir un client valide ou créer un nouveau client.\n");
            continue;
        }

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
        while (uneresa.nombre_pers <= 0 || uneresa.nombre_pers > 4)
        {
            printf(">>> Nombre de personnes invalide. Veuillez saisir un nombre valide.\n");
            printf("Nombre de personnes: ");
            scanf("%d", &uneresa.nombre_pers);
            viderBuffer();
        }

        // Afficher les chambres disponibles pour la période saisie
        afficherChambresDisponibles(uneresa.date_entree, uneresa.date_sortie);

        // Saisir et vérifier le numéro de chambre
        printf("Chambre à réserver: ");
        scanf("%d", &uneresa.chambre);
        viderBuffer();
        while (!chambreDisponible(uneresa.chambre, uneresa.date_entree, uneresa.date_sortie) || !chambreValide(uneresa.chambre))
        {
            printf(">>> Chambre non disponible pour cette période. Veuillez saisir une autre chambre.\n");
            printf("Chambre à réserver: ");
            scanf("%d", &uneresa.chambre);
            viderBuffer();
        }

        // Générer un numéro de réservation unique
        uneresa.num_r = genererNumResa();
        printf(">>> Réservation numéro %d enregistrée\n", uneresa.num_r);

        tabresa[i] = uneresa;          // Sauvegarder les données saisies dans le tableau
        genererPC(i);                  // Générer les produits commandés pour la réservation
        a_sauvegarder_reservation = 1; // Activer le flag pour sauvegarder les données
        i++;                           // Incrémenter le compteur des réservations
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

// Fonction pour demander le critère de recherche au utilisateur pour les réservations
void demanderCritereRechercheReservations()
{
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
        struct reservation recherche_resa = {0}; // Initialiser la structure de recherche
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
        do
        {
            printf("Nouveau nombre de personnes: ");
            scanf("%d", &uneresa.nombre_pers);
            viderBuffer();
        } while (uneresa.nombre_pers <= 0 || uneresa.nombre_pers > 4);

        // Mise à jour de la chambre
        printf("Nouvelle chambre à réserver: ");
        scanf("%d", &uneresa.chambre);
        viderBuffer();
        while (!chambreDisponible(uneresa.chambre, new_entree, new_sortie) || !chambreValide(uneresa.chambre))
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
        nettoyerPC(trouve); // Nettoyer les produits commandés générés automatiquement précédemment
        genererPC(trouve);  // Générer les produits commandés pour la réservation
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
        printf("%-9d %-9s %-9s %-9d %-9d %-9d\n", uneresa.num_r, date_in, date_out, uneresa.chambre, uneresa.nombre_pers, uneresa.num_c);

        // Demander confirmation pour supprimer la réservation
        char reponse[TAILLE_CHAR];
        printf("Confirmez-vous la suppression de la réservation (o/n) : ");
        scanf("%s", reponse);
        viderBuffer();
        convMaj(reponse);
        if (reponse[0] == 'O')
        {
            // Nettoyer les produits commandés générés automatiquement précédemment
            nettoyerPC(trouve);
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

// Fonction pour sauvegarder les réservations dans la base de données
void sauvegardeReservations()
{
    FILE *f1;
    f1 = fopen(DB_RESERVATIONS, "w");
    if (f1 == NULL)
    {
        printf(">>> Erreur d'ouverture de la base de données des réservations\n");
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

    if (f1 == NULL)
    {
        printf("Erreur d'ouverture de la base de données des réservations.\n");
        return;
    }

    while (fscanf(f1, "%9d %9s %9s %9d %9d %9d", &uneresa.num_r, dateEntree, dateSortie, &uneresa.chambre, &uneresa.nombre_pers, &uneresa.num_c) == 6)
    {
        // Conversion des chaînes de caractères en structures de dates
        stringToDate(dateEntree, &uneresa.date_entree);
        stringToDate(dateSortie, &uneresa.date_sortie);

        if (nbresa < TAILLE_TAB)
        {
            tabresa[nbresa] = uneresa;
            nbresa++;
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

// Fonction pour ajouter automatiquement les produit commandés pour une réservation
void genererPC(int idx_resa)
{
    int i = nbpc;
    struct produit_commande unepc;

    int nbjours = nbJoursEntreDates(tabresa[idx_resa].date_entree, tabresa[idx_resa].date_sortie);
    unepc.num_r = tabresa[idx_resa].num_r;
    unepc.code = codeProduitChambre(tabresa[idx_resa].chambre);
    unepc.qte = nbjours;

    tabpc[i++] = unepc;   // Sauvegarder les données saisies dans le tableau
    a_sauvegarder_pc = 1; // Activer le flag pour sauvegarder les données

    nbpc = i; // Mettre à jour le nombre des réservations
}

// Fonction pour "nettoyer" les produits commandés générés automatiquement pour une réservation
void nettoyerPC(int idx_resa)
{
    int i, j;
    for (i = 0; i < nbpc; i++)
    {
        if (tabpc[i].num_r == tabresa[idx_resa].num_r)
        {
            for (j = i; j < nbpc - 1; j++)
            {
                tabpc[j] = tabpc[j + 1]; // Décaler les produits commandés
            }
            nbpc--; // Décrémenter le nombre de produits commandés
            if (nbpc > 0)
            {
                tabpc[nbpc].num_r = 0;
                tabpc[nbpc].code = 0;
                tabpc[nbpc].qte = 0;
            }
            i--; // Décrémenter le compteur pour rester sur la même position
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Fonctions pour la partie Client
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------

// Fonction pour afficher le menu de gestion des clients
void afficherMenuClient()
{
    printf("****** Gérer les clients ******\n\n");
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
    printf("%-6s %-9s %-20s %-20s %-8s %-15s %-s\n", "CODE", "NIV_FI", "NOM", "PRENOM", "D_NAIS", "TEL", "MAIL");
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
            printf("%-6d %-9s %-20s %-20s %-8s %-15s %-49s\n", tabclient[i].code, niveauFideliteToString(tabclient[i].fidelite), tabclient[i].nom, tabclient[i].prenom, date_naissance, tabclient[i].tel, tabclient[i].mail);
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

        // Initialiser le niveau de fidélité du client
        unclient.fidelite = 0;

        // Initialiser le montant dépensé du client
        unclient.total = 0;

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
        printf("-7- Niveau fidelité\n");
        printf("-0- Revenir au menu précédent\n\n");
        printf("Votre choix : ");
        scanf("%d", &rechercher_choix);
        viderBuffer();
        struct client recherche_client = {0};
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
            scanf("%[^\n]", recherche_client.nom);
            viderBuffer();
            rechercherClient(recherche_client);
            break;
        case 3:
            printf("Prénom à rechercher : ");
            scanf("%[^\n]", recherche_client.prenom);
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
        case 7:
            printf("Niveau de fidelité à rechercher (0 à 4): ");
            scanf("%d", &recherche_client.fidelite);
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
            (strlen(client_a_trouver.mail) > 0 && strcmp(tabclient[i].mail, client_a_trouver.mail) == 0) ||
            (client_a_trouver.fidelite == tabclient[i].fidelite))
        {
            match = true;
        }

        // Si un match est trouvé, afficher le client
        if (match)
        {
            char date_nais[TAILLE_DATE];
            dateToString(tabclient[i].date_nais, date_nais);
            printf("%-6d %-9s %-20s %-20s %-8s %-15s %-49s\n", tabclient[i].code, niveauFideliteToString(tabclient[i].fidelite), tabclient[i].nom, tabclient[i].prenom, date_nais, tabclient[i].tel, tabclient[i].mail);
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
        printf("%-6d %-9s %-20s %-20s %-8s %-15s %-49s\n", unclient.code, niveauFideliteToString(unclient.fidelite), unclient.nom, unclient.prenom, date_nais, unclient.tel, unclient.mail);

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
            printf("Nouveau montant dépensé du client: ");
            scanf("%f", &unclient.total);
            viderBuffer();
            if (unclient.total < 0)
            {
                printf("Montant dépensé invalide.\n");
            }
        } while (unclient.total < 0);
        // Mise à jour du niveau de fidélité
        unclient.fidelite = calculerFidelite(unclient.total);

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
        printf("%-6d %-9s %-20s %-20s %-8s %-15s %-49s\n", unclient.code, niveauFideliteToString(unclient.fidelite), unclient.nom, unclient.prenom, date_nais, unclient.tel, unclient.mail);

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
    f1 = fopen(DB_CLIENTS, "w");
    if (f1 == NULL)
    {
        printf(">>> Erreur d'ouverture de la base de données des clients\n");
        return;
    }
    for (int i = 0; i < nbclient; i++) // boucle de sauvegarde
    {
        char dateNais[TAILLE_DATE];
        dateToString(tabclient[i].date_nais, dateNais);
        fprintf(f1, "%-6d %-9d %-9.2f %-20s %-20s %-8s %-15s %-49s\n", tabclient[i].code, tabclient[i].fidelite, tabclient[i].total, tabclient[i].nom, tabclient[i].prenom, dateNais, tabclient[i].tel, tabclient[i].mail);
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

    if (f1 == NULL)
    {
        printf("Erreur d'ouverture de la base de données des clients.\n");
        return;
    }

    while (fscanf(f1, "%6d %9d %9f %19[^\n] %19[^\n] %8s %15s %49s\n", &unclient.code, &unclient.fidelite, &unclient.total, unclient.nom, unclient.prenom, dateNais, unclient.tel, unclient.mail) == 8)
    {
        // Conversion des chaînes de caractères en structures de dates
        stringToDate(dateNais, &unclient.date_nais);
        if (nbclient < TAILLE_TAB)
        {
            tabclient[nbclient] = unclient;
            nbclient++;
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
// Fonctions pour la partie Produits Commandés
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------

// Fonction pour afficher le menu de gestion des Produits Commandés
void afficherMenuPC()
{
    printf("-1- Afficher les produits\n");
    printf("-2- Rechercher un produit\n");
    printf("-3- Ajouter les produits\n");
    printf("-4- Modifier les produits\n");
    printf("-5- Supprimer les produits\n");
    printf("-6- Sauvegarde des produits\n");
    printf("-0- Revenir au menu précédent\n");
    printf("\n");
    printf("Choisissez une option : ");
}

void menuPC()
{
    int numero_resa;
    printf("****** Gérer le restaurant & services ******\n\n");
    printf("Entrez le numéro de la réservation : ");
    scanf("%d", &numero_resa);
    viderBuffer();
    int trouve = lanceRecherche(numero_resa);
    while (trouve == VAL_INI)
    {
        printf(">>> Réservation numéro %d non trouvée\n", numero_resa);
        printf("Entrez le numéro de la réservation : ");
        scanf("%d", &numero_resa);
        viderBuffer();
        trouve = lanceRecherche(numero_resa);
    }

    // afficher les informations de la réservation trouvé
    afficherEnTeteReservations();
    char date_in[TAILLE_DATE];
    char date_out[TAILLE_DATE];
    dateToString(tabresa[trouve].date_entree, date_in);
    dateToString(tabresa[trouve].date_sortie, date_out);
    printf("%-9d %-9s %-9s %-9d %-9d %-9d\n", tabresa[trouve].num_r, date_in, date_out, tabresa[trouve].chambre, tabresa[trouve].nombre_pers, tabresa[trouve].num_c);

    int restaurant_choix = VAL_INI;
    while (restaurant_choix != 0)
    {
        afficherMenuPC();
        scanf("%d", &restaurant_choix);
        viderBuffer();
        // Traitement des options
        switch (restaurant_choix)
        {
        case 1:
            afficherPC(trouve);
            break;
        case 2:
            rechercherPC(trouve);
            break;
        case 3:
            ajouterPC(trouve);
            break;
        case 4:
            modifierPC(trouve);
            break;
        case 5:
            supprimerPC(trouve);
            break;
        case 6:
            sauvegardePC();
            break;
        case 0:
            break;
        default:
            printf(">>> Option invalide. Veuillez réessayer.\n");
        }
    }
}

// Fonction pour afficher les produits par réservation
void afficherPC(int idx_resa)
{
    int prod_trouve = 0;

    printf("Produits commandés pour la réservation numéro %d\n", tabresa[idx_resa].num_r);
    printf("%-6s %-40s %-6s %-10s %-10s\n", "CODE", "DESC", "QTE", "PRIX.U", "TOTAL");
    for (int i = 0; i < TAILLE_TAB_PC - 1; i++)
    {
        if (tabpc[i].num_r == tabresa[idx_resa].num_r)
        {
            int idx_p = lanceRechercheProduit(tabpc[i].code);
            float total_pc = tabpc[i].qte * tabproduit[idx_p].prix;
            printf("%-6d %-40s %-6d %-10.2f %-10.2f\n", tabpc[i].code, tabproduit[idx_p].desc, tabpc[i].qte, tabproduit[idx_p].prix, total_pc);
            prod_trouve = 1;
        }
    }
    if (!prod_trouve)
    {
        printf(">>> Aucun produit commandé pour la réservation numéro %d\n", tabresa[idx_resa].num_r);
    }
}

// Fonction pour rechercher un produit par réservation, retourne l'index du produit commandé dans la liste des produits commandés
int lanceRecherchePC(int idx_resa, int code_pc)
{
    int idx_pc = VAL_INI;
    for (int i = 0; i < TAILLE_TAB_PC - 1; i++)
    {
        if (tabpc[i].num_r == tabresa[idx_resa].num_r && tabpc[i].code == code_pc)
        {
            idx_pc = i;
            break;
        }
    }
    return idx_pc;
}

// Fonction pour rechercher un produit par réservation
void rechercherPC(int idx_resa)
{
    int code_pc;
    do // boucle pour demander le code du produit à rechercher
    {
        printf("Entrez le code du produit à rechercher : ");
        scanf("%d", &code_pc);
        viderBuffer();
        if (!codeProduitValide(code_pc))
        {
            printf(">>> Produit code %d n'existe pas.\n", code_pc);
        }
    } while (!codeProduitValide(code_pc));

    int idx_pc = lanceRecherchePC(idx_resa, code_pc);
    if (idx_pc == VAL_INI)
    {
        printf(">>> Produit code %d non trouvé pour la réservation %d\n", code_pc, tabresa[idx_resa].num_r);
    }
    else
    {
        printf("Produit trouvé : \n");
        printf("%-6s %-40s %-6s %-10s %-10s\n", "CODE", "DESC", "QTE", "PRIX.U", "TOTAL");
        int idx_p = lanceRechercheProduit(code_pc);
        float total_pc = tabpc[idx_pc].qte * tabproduit[idx_p].prix;
        printf("%-6d %-40s %-6d %-10.2f %-10.2f\n", tabpc[idx_pc].code, tabproduit[idx_p].desc, tabpc[idx_pc].qte, tabproduit[idx_p].prix, total_pc);
    }
}

// Fonction pour ajouter un produit par réservation
void ajouterPC(int idx_resa)
{
    char reponse[TAILLE_CHAR];
    int code_pc, qte_pc;
    int i = nbpc;
    if (lanceRechercheFactureParReservation(tabresa[idx_resa].num_r) != VAL_INI)
    {
        printf(">>> La réservation %d a déjà été facturé, aucune modification possible.\n", tabresa[idx_resa].num_r);
        return;
    }
    do
    {
        do
        {
            printf("Entrez le code du produit: ");
            scanf("%d", &code_pc);
            viderBuffer();
            if (!codeProduitValide(code_pc))
            {
                printf(">>> Produit code %d n'existe pas.\n", code_pc);
            }
        } while (!codeProduitValide(code_pc));
        int idx_pc = lanceRecherchePC(idx_resa, code_pc);
        if (idx_pc != VAL_INI) // Vérifier si le produit est déjà présent dans la liste des produits commandés
        {
            printf(">>> Produit code %d déjà présent\n", code_pc);
            printf("Entrez la quantité du produit à ajouter : ");
            scanf("%d", &qte_pc);
            viderBuffer();
            tabpc[idx_pc].qte += qte_pc; // Ajouter la quantité à la quantité existante
            a_sauvegarder_pc = 1;
            printf(">>> Quantité du produit code %d modifiée\n", code_pc);
        }
        else // Ajouter un nouveau produit commandé
        {
            printf("Entrez la quantité du produit: ");
            scanf("%d", &qte_pc);
            viderBuffer();
            tabpc[i].num_r = tabresa[idx_resa].num_r;
            tabpc[i].code = code_pc;
            tabpc[i].qte = qte_pc;
            i++;
            a_sauvegarder_pc = 1;
            printf(">>> Produit code %d ajouté\n", code_pc);
        }
        printf("Voulez-vous ajouter un autre produit ? (o/n) : ");
        scanf("%s", reponse);
        viderBuffer();
        convMaj(reponse);
    } while (reponse[0] == 'O');

    nbpc = i; // Mettre à jour le nombre des produits commandés
}

// Fonction pour modifier un produit par réservation
void modifierPC(int idx_resa)
{
    int code_pc;
    int idx_pc = VAL_INI;

    if (lanceRechercheFactureParReservation(tabresa[idx_resa].num_r) != VAL_INI)
    {
        printf(">>> La réservation %d a déjà été facturé, aucune modification possible.\n", tabresa[idx_resa].num_r);
        return;
    }
    do
    {
        printf("Entrez le code du produit à modifier : ");
        scanf("%d", &code_pc);
        viderBuffer();
        idx_pc = lanceRecherchePC(idx_resa, code_pc);
        if (idx_pc == VAL_INI)
        {
            printf(">>> Produit code %d non trouvé pour la réservation %d\n", code_pc, tabresa[idx_resa].num_r);
        }
    } while (idx_pc == VAL_INI);

    printf("Entrez la nouvelle quantité du produit : ");
    scanf("%d", &tabpc[idx_pc].qte);
    viderBuffer();
    printf(">>> Produit code %d modifié\n", code_pc);

    a_sauvegarder_reservation = 1;
}

// Fonction pour supprimer un produit par réservation
void supprimerPC(int idx_resa)
{
    int code_pc;
    int idx_pc = VAL_INI;
    if (lanceRechercheFactureParReservation(tabresa[idx_resa].num_r) != VAL_INI)
    {
        printf(">>> La réservation %d a déjà été facturé, aucune modification possible.\n", tabresa[idx_resa].num_r);
        return;
    }
    do
    {
        printf("Entrez le code du produit à supprimer : ");
        scanf("%d", &code_pc);
        viderBuffer();
        idx_pc = lanceRecherchePC(idx_resa, code_pc);
        if (idx_pc == VAL_INI)
        {
            printf(">>> Produit code %d non trouvé pour la réservation %d\n", code_pc, tabresa[idx_resa].num_r);
        }
    } while (idx_pc == VAL_INI);

    for (int i = idx_pc; i < TAILLE_TAB_PC - 1; i++)
    {
        tabpc[i] = tabpc[i + 1];
    }
    printf(">>> Produit code %d supprimé\n", code_pc);

    a_sauvegarder_reservation = 1;
}

// Fonction pour sauvegarder les produits commandés dans la base de données
void sauvegardePC()
{
    FILE *f1;
    f1 = fopen(DB_PC, "w");
    if (f1 == NULL)
    {
        printf(">>> Erreur d'ouverture de la base de données des produits commandés\n");
        return;
    }
    for (int i = 0; i < nbpc; i++) // boucle de sauvegarde
    {
        fprintf(f1, "%-9d %-9d %-9d\n", tabpc[i].num_r, tabpc[i].code, tabpc[i].qte);
    }
    fclose(f1);
    a_sauvegarder_pc = 0; // désactiver le flag pour sauvegarder les données
    printf(">>> %d produits commandés sauvegardés\n", nbpc);
}

// Fonction pour charger les produits commandés depuis la base de données
void chargementPC()
{
    struct produit_commande unpc;
    FILE *f1 = fopen(DB_PC, "r");
    if (f1 == NULL)
    {
        printf("Erreur d'ouverture de la base de données des produits commandés.\n");
        return;
    }
    while (fscanf(f1, "%9d %9d %9d\n", &unpc.num_r, &unpc.code, &unpc.qte) == 3)
    {
        if (nbpc < TAILLE_TAB_PC)
        {
            tabpc[nbpc] = unpc;
            nbpc++;
        }
        else
        {
            printf("Limite du tableau des produits commandés atteinte.\n");
            break;
        }
    }
    fclose(f1);
    printf("%d produits commandés chargés.\n", nbpc);
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Fonctions pour la Facturation
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------

// Fonction pour afficher le menu pour la facturation
void afficherMenuFacturation()
{
    printf("****** Facturation ******\n\n");
    printf("-1- Afficher les factures\n");
    printf("-2- Rechercher une facture\n");
    printf("-3- Facturer une réservation\n");
    printf("-4- Paiement une facture\n");
    printf("-5- Supprimer une facture\n");
    printf("-6- Sauvegarde des factures\n");
    printf("-0- Revenir au menu précédent\n");
    printf("\n");
    printf("Choisissez une option : ");
}

// Fonction pour afficher le menu Facturation
void menuFacturation()
{
    int facturation_choix = VAL_INI;
    while (facturation_choix != 0)
    {
        afficherMenuFacturation();
        scanf("%d", &facturation_choix);
        viderBuffer();
        // Traitement des options
        switch (facturation_choix)
        {
        case 1:
            afficherFactures();
            break;
        case 2:
            demanderCritereRechercheFacture();
            break;
        case 3:
            facturerReservation();
            break;
        case 4:
            paiementFacture();
            break;
        case 5:
            supprimerFacture();
            break;
        case 6:
            sauvegardeFactures();
            break;
        case 0:
            break;
        default:
            printf(">>> Option invalide. Veuillez réessayer.\n");
        }
    }
}

// Fonction pour facturer une réservation
void facturerReservation()
{
    int num_r; // Numéro de la réservation à facturer
    int i = nbfacture;
    printf("Entrez le numéro de la réservation à facturer : ");
    scanf("%d", &num_r);
    viderBuffer();
    int idx_resa = lanceRecherche(num_r);
    int idx_facture = lanceRechercheFactureParReservation(num_r);
    if (idx_resa == VAL_INI)
    {
        printf(">>> Réservation numéro %d non trouvée\n", num_r);
    }
    else if (idx_facture != VAL_INI)
    {
        printf(">>> Réservation numéro %d déjà facturée\n", num_r);
    }
    else
    {
        // Afficher les informations de la réservation
        afficherEnTeteReservations();
        char date_in[TAILLE_DATE];
        char date_out[TAILLE_DATE];
        dateToString(tabresa[idx_resa].date_entree, date_in);
        dateToString(tabresa[idx_resa].date_sortie, date_out);
        printf("%-9d %-9s %-9s %-9d %-9d %-9d\n", tabresa[idx_resa].num_r, date_in, date_out, tabresa[idx_resa].chambre, tabresa[idx_resa].nombre_pers, tabresa[idx_resa].num_c);

        // Afficher les produits commandés
        afficherPC(idx_resa);

        // Calculer le montant total de la facture
        float total_facture = 0;
        for (int i = 0; i < TAILLE_TAB_PC - 1; i++)
        {
            if (tabpc[i].num_r == num_r)
            {
                int idx_p = lanceRechercheProduit(tabpc[i].code);
                total_facture += tabpc[i].qte * tabproduit[idx_p].prix;
            }
        }
        total_facture = appliquerFidelite(total_facture, tabresa[idx_resa].num_c); // Appliquer la réduction de fidélité
        printf("Montant total de la facture : %.2f €\n", total_facture);

        // Demander confirmation pour facturer la réservation
        char reponse[TAILLE_CHAR];
        printf("Confirmez-vous la facturation de la réservation (o/n) : ");
        scanf("%s", reponse);
        viderBuffer();
        convMaj(reponse);
        if (reponse[0] == 'O')
        {
            // Générer un numéro de facture unique
            struct facture une_facture;
            une_facture.num_f = genererNumFacture();
            une_facture.num_r = num_r;
            une_facture.total = total_facture;
            obtenirDateActuelle(&une_facture.date_fact); // Date de facturation
            struct date date_paiement = {0, 0, 0};       // Date de paiement
            une_facture.date_paiement = date_paiement;
            une_facture.statut = 0; // Statut de la facture : non payée
            printf(">>> Facture numéro %d créée pour la réservation %d\n", une_facture.num_f, num_r);
            tabfacture[i++] = une_facture;
        }
        nbfacture = i; // Mettre à jour le nombre des factures
        a_sauvegarder_facture = 1;
    }
}

// Fonction pour afficher les factures
void afficherFactures()
{
    if (nbfacture == 0)
    {
        printf(">>> Aucune facture à afficher\n");
    }
    else
    {
        afficherEnTeteFactures();
        for (int i = 0; i < nbfacture; i++)
        {
            char date_fact[TAILLE_DATE];
            char date_paiement[TAILLE_DATE];
            char *statut_f;
            statut_f = statutFactureToString(tabfacture[i].statut);
            dateToString(tabfacture[i].date_fact, date_fact);
            dateToString(tabfacture[i].date_paiement, date_paiement);
            printf("%-9d %-9d %-9.2f %-9s %-9s %-9s\n", tabfacture[i].num_f, tabfacture[i].num_r, tabfacture[i].total, date_fact, date_paiement, statut_f);
        }
    }
}

// Fonction pour afficher l'en-tête du tableau des factures
void afficherEnTeteFactures()
{
    printf("%-9s %-9s %-9s %-9s %-9s %-9s\n", "NUM_F", "NUM_R", "TOTAL", "D.FACT", "D.PAIE", "STATUT");
}

// Fonction pour rechercher une facture par son numéro de facture
int lanceRechercheFacture(int num_facture_a_rechercher)
{
    int idx_facture = VAL_INI;
    for (int i = 0; i < nbfacture; i++)
    {
        if (tabfacture[i].num_f == num_facture_a_rechercher)
        {
            idx_facture = i;
            break;
        }
    }
    return idx_facture;
}

// Fonction pour rechercher une facture par son numéro de réservation
int lanceRechercheFactureParReservation(int num_reservation_a_rechercher)
{
    int idx_facture = VAL_INI;
    for (int i = 0; i < nbfacture; i++)
    {
        if (tabfacture[i].num_r == num_reservation_a_rechercher)
        {
            idx_facture = i;
            break;
        }
    }
    return idx_facture;
}

// Fonction pour demander les critères de recherche d'une facture
void demanderCritereRechercheFacture()
{
    int critere_recherche = VAL_INI;
    if (nbfacture == 0)
    {
        printf(">>> Aucune facture à rechercher\n");
        return;
    }
    while (critere_recherche != 0)
    {
        printf("Choisir critère de recherche : \n");
        printf("-1- Numéro de facture\n");
        printf("-2- Numéro de réservation\n");
        printf("-3- Total\n");
        printf("-4- Date de facture\n");
        printf("-5- Date de paiement\n");
        printf("-6- Statut\n");
        printf("-0- Revenir au menu précédent\n\n");
        printf("Votre choix : ");
        scanf("%d", &critere_recherche);
        viderBuffer();
        struct facture recherche_facture = {0}; // Initialiser les critères de recherche
        switch (critere_recherche)
        {
        case 1:
            printf("Entrez le numéro de la facture : ");
            scanf("%d", &recherche_facture.num_f);
            viderBuffer();
            rechercherFacture(recherche_facture);
            break;
        case 2:
            printf("Entrez le numéro de la réservation : ");
            scanf("%d", &recherche_facture.num_r);
            viderBuffer();
            rechercherFacture(recherche_facture);
            break;
        case 3:
            printf("Entrez le montant total de la facture : ");
            scanf("%f", &recherche_facture.total);
            viderBuffer();
            rechercherFacture(recherche_facture);
            break;
        case 4:
            printf("Entrez la date de facturation (jjmmaaaa) : ");
            scanf("%2d%2d%4d", &recherche_facture.date_fact.jour, &recherche_facture.date_fact.mois, &recherche_facture.date_fact.annee);
            viderBuffer();
            rechercherFacture(recherche_facture);
            break;
        case 5:
            printf("Entrez la date de paiement (jjmmaaaa) : ");
            scanf("%2d%2d%4d", &recherche_facture.date_paiement.jour, &recherche_facture.date_paiement.mois, &recherche_facture.date_paiement.annee);
            viderBuffer();
            rechercherFacture(recherche_facture);
            break;
        case 6:
            printf("Entrez le statut de la facture (0: non payée, 1: payée) : ");
            scanf("%d", &recherche_facture.statut);
            viderBuffer();
            rechercherFacture(recherche_facture);
            break;
        case 0:
            break;
        default:
            printf(">>> Option invalide. Veuillez réessayer.\n");
        }
    }
}

// Fonction pour rechercher une facture par critere de recherche
void rechercherFacture(struct facture facture_a_trouver)
{
    int nb_facture_trouve = 0; // Nombre de factures trouvées

    // Afficher l'en-tête seulement s'il y a des factures à afficher
    if (nbfacture > 0)
    {
        afficherEnTeteFactures();
    }

    for (int i = 0; i < nbfacture; i++)
    {
        bool match = false; // Indicateur si la facture actuelle correspond aux critères

        // Vérifier les critères de recherche
        if ((facture_a_trouver.num_f > 0 && tabfacture[i].num_f == facture_a_trouver.num_f) ||
            (facture_a_trouver.num_r > 0 && tabfacture[i].num_r == facture_a_trouver.num_r) ||
            (facture_a_trouver.total > 0 && tabfacture[i].total == facture_a_trouver.total) ||
            (dateEgale(tabfacture[i].date_fact, facture_a_trouver.date_fact) && facture_a_trouver.date_fact.jour > 0) ||
            (dateEgale(tabfacture[i].date_paiement, facture_a_trouver.date_paiement) && facture_a_trouver.date_paiement.jour > 0) ||
            (facture_a_trouver.statut >= 0 && tabfacture[i].statut == facture_a_trouver.statut))
        {
            match = true;
        }

        // Si un match est trouvé, afficher la facture
        if (match)
        {
            char date_fact[TAILLE_DATE];
            char date_paiement[TAILLE_DATE];
            char *statut_f;
            dateToString(tabfacture[i].date_fact, date_fact);
            dateToString(tabfacture[i].date_paiement, date_paiement);
            statut_f = statutFactureToString(tabfacture[i].statut);
            printf("%-9d %-9d %-9.2f %-9s %-9s %-9s\n", tabfacture[i].num_f, tabfacture[i].num_r, tabfacture[i].total, date_fact, date_paiement, statut_f);
            nb_facture_trouve++;
        }
    }

    if (nb_facture_trouve == 0)
    {
        printf(">>> Aucune facture trouvée correspondant aux critères.\n");
    }
}

// Fonction pour enregistrer la date de paiement d'une facture
void paiementFacture()
{
    int num_facture;
    printf("Entrez le numéro de la facture : ");
    scanf("%d", &num_facture);
    viderBuffer();
    int idx_facture = lanceRechercheFacture(num_facture);
    if (idx_facture == VAL_INI)
    {
        printf(">>> Facture numéro %d non trouvée\n", num_facture);
    }
    else
    {
        // Afficher les informations de la facture
        afficherEnTeteFactures();
        char date_fact[TAILLE_DATE];
        char date_paiement[TAILLE_DATE];
        char *statut_f;
        dateToString(tabfacture[idx_facture].date_fact, date_fact);
        dateToString(tabfacture[idx_facture].date_paiement, date_paiement);
        statut_f = statutFactureToString(tabfacture[idx_facture].statut);
        printf("%-9d %-9d %-9.2f %-9s %-9s %-9s\n", tabfacture[idx_facture].num_f, tabfacture[idx_facture].num_r, tabfacture[idx_facture].total, date_fact, date_paiement, statut_f);

        // Demander confirmation pour modifier la facture
        char reponse[TAILLE_CHAR];
        printf("Confirmez-vous le paiement de la facture (o/n) : ");
        scanf("%s", reponse);
        viderBuffer();
        convMaj(reponse);
        if (reponse[0] == 'O')
        {
            // Modifier la facture
            printf("Entrez la date de paiement de la facture : ");
            scanf("%2d%2d%4d", &tabfacture[idx_facture].date_paiement.jour, &tabfacture[idx_facture].date_paiement.mois, &tabfacture[idx_facture].date_paiement.annee);
            viderBuffer();
            tabfacture[idx_facture].statut = 1; // Marquer la facture comme payée

            // mettre à jour le niveau de fidelité du client
            int idx_resa = lanceRecherche(tabfacture[idx_facture].num_r);
            int idx_client = lanceRechercheClient(tabresa[idx_resa].num_c);
            tabclient[idx_client].total += tabfacture[idx_facture].total;                   // Ajouter le montant de la facture au total dépensé du client
            tabclient[idx_client].fidelite = calculerFidelite(tabclient[idx_client].total); // Calculer le niveau de fidélité du client

            a_sauvegarder_facture = 1;
            a_sauvegarder_client = 1;
            printf(">>> Facture numéro %d encaissé\n", num_facture);
        }
    }
}

// Fonction pour supprimer une facture
void supprimerFacture()
{
    int num_facture;
    printf("Entrez le numéro de la facture à supprimer : ");
    scanf("%d", &num_facture);
    viderBuffer();
    int idx_facture = lanceRechercheFacture(num_facture);
    if (idx_facture == VAL_INI)
    {
        printf(">>> Facture numéro %d non trouvée\n", num_facture);
    }
    else
    {
        // Afficher les informations de la facture
        afficherEnTeteFactures();
        char date_fact[TAILLE_DATE];
        char date_paiement[TAILLE_DATE];
        char *statut_f;
        dateToString(tabfacture[idx_facture].date_fact, date_fact);
        dateToString(tabfacture[idx_facture].date_paiement, date_paiement);
        statut_f = statutFactureToString(tabfacture[idx_facture].statut);
        printf("%-9d %-9d %-9.2f %-9s %-9s %-9s\n", tabfacture[idx_facture].num_f, tabfacture[idx_facture].num_r, tabfacture[idx_facture].total, date_fact, date_paiement, statut_f);

        // Demander confirmation pour supprimer la facture
        char reponse[TAILLE_CHAR];
        printf("Confirmez-vous la suppression de la facture (o/n) : ");
        scanf("%s", reponse);
        viderBuffer();
        convMaj(reponse);
        if (reponse[0] == 'O')
        {
            // Supprimer la facture
            for (int i = idx_facture; i < nbfacture - 1; i++)
            {
                tabfacture[i] = tabfacture[i + 1];
            }
            nbfacture--;
            a_sauvegarder_facture = 1;
            printf(">>> Facture numéro %d supprimée\n", num_facture);
        }
    }
}

// Fonction pour sauvegarder les factures dans la base de données
void sauvegardeFactures()
{
    FILE *f1;
    f1 = fopen(DB_FACTURES, "w");
    if (f1 == NULL)
    {
        printf(">>> Erreur d'ouverture de la base de données des factures\n");
        return;
    }
    for (int i = 0; i < nbfacture; i++) // boucle de sauvegarde
    {
        char date_fact[TAILLE_DATE];
        char date_paiement[TAILLE_DATE];
        dateToString(tabfacture[i].date_fact, date_fact);
        dateToString(tabfacture[i].date_paiement, date_paiement);
        fprintf(f1, "%-9d %-9d %-9.2f %-9s %-9s %-9d\n", tabfacture[i].num_f, tabfacture[i].num_r, tabfacture[i].total, date_fact, date_paiement, tabfacture[i].statut);
    }
    fclose(f1);
    a_sauvegarder_facture = 0; // désactiver le flag pour sauvegarder les données
    printf(">>> %d factures sauvegardées\n", nbfacture);
}

// Fonction pour charger les factures depuis la base de données
void chargementFactures()
{
    struct facture unefacture;
    FILE *f1 = fopen(DB_FACTURES, "r");
    char date_fact[TAILLE_DATE], date_paiement[TAILLE_DATE];

    if (f1 == NULL)
    {
        printf("Erreur d'ouverture de la base de données des factures.\n");
        return;
    }
    while (fscanf(f1, "%9d %9d %9f %9s %9s %9d\n", &unefacture.num_f, &unefacture.num_r, &unefacture.total, date_fact, date_paiement, &unefacture.statut) == 6)
    {
        stringToDate(date_fact, &unefacture.date_fact);
        stringToDate(date_paiement, &unefacture.date_paiement);
        if (nbfacture < TAILLE_TAB)
        {
            tabfacture[nbfacture] = unefacture;
            nbfacture++;
        }
        else
        {
            printf("Limite du tableau des factures atteinte.\n");
            break;
        }
    }
    fclose(f1);
    printf("%d factures chargées.\n", nbfacture);
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// Fonctions pour gerer les produits
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------

// Fonction pour afficher le menu de gestion des produits
void afficherMenuProduits()
{
    printf("****** Gérer les produits ******\n\n");
    printf("-1- Afficher les produits\n");
    printf("-2- Rechercher un produit\n");
    printf("-3- Saisir un nouveau produit\n");
    printf("-4- Modifier un produit\n");
    printf("-5- Supprimer un produit\n");
    printf("-6- Sauvegarde des produits\n");
    printf("-0- Revenir au menu précédent\n");
    printf("\n");
    printf("Choisissez une option : ");
}

// Fonction pour le menu Produits
void menuProduits()
{
    int produits_choix = VAL_INI;
    while (produits_choix != 0)
    {
        afficherMenuProduits();
        scanf("%d", &produits_choix);
        viderBuffer();
        // Traitement des options
        switch (produits_choix)
        {
        case 1:
            afficherProduits();
            break;
        case 2:
            demanderCritereRechercheProduits();
            break;
        case 3:
            saisirProduit();
            break;
        case 4:
            modifierProduit();
            break;
        case 5:
            supprimerProduit();
            break;
        case 6:
            sauvegardeProduits();
            break;
        case 0:
            break;
        default:
            printf(">>> Option invalide. Veuillez réessayer.\n");
        }
    }
}

// Fonction pour afficher les produits
void afficherProduits()
{
    if (nbproduit == 0)
    {
        printf(">>> Aucun produit à afficher\n");
    }
    else
    {
        // char reponse[TAILLE_CHAR];
        int i, debut = 0;

        printf("<!> %d produits enregistrés dans la base\n", nbproduit);
        // printf("\n");
        // if (nbproduit > 10)
        // {
        //     printf("Vous voulez afficher les 10 derniers produits ? (o/n) : ");
        //     scanf(" %s", reponse);
        //     viderBuffer();
        //     convMaj(reponse);
        //     if (reponse[0] == 'O')
        //     {
        //         debut = nbproduit - 10;
        //     }
        // }
        afficherEnTeteProduits();
        for (i = debut; i < nbproduit; i++)
        {
            printf("%-6d %-40s %-10.2f\n", tabproduit[i].code, tabproduit[i].desc, tabproduit[i].prix);
        }
        printf("\n");
    }
}

// Fonction pour afficher l'en-tête du tableau des produits
void afficherEnTeteProduits()
{
    printf("%-6s %-40s %-10s\n", "CODE", "DESCRIPTION", "PRIX-€");
}

// Fonction pour saisir un nouveau produit
void saisirProduit()
{
    int i = nbproduit;
    struct produit unproduit;
    char reponse[TAILLE_CHAR]; // Variable pour contrôler la continuation de la boucle

    // boucle de saisie pour un nouveau produit
    do
    {
        // Saisir le code du produit
        do
        {
            printf("Code produit: ");
            scanf("%d", &unproduit.code);
            viderBuffer();
            if (unproduit.code <= 0 || unproduit.code > TAILLE_TAB)
            {
                printf("Code invalide, veuillez saisir un code valide.\n");
            }
        } while (unproduit.code <= 0 || unproduit.code > TAILLE_TAB);

        // Saisir la description du produit
        do
        {
            printf("Description : ");
            scanf(" %[^\n]", unproduit.desc); // %[^\n] permet de lire une chaîne de caractères avec des espaces
            viderBuffer();
            if (strlen(unproduit.desc) == 0 || strlen(unproduit.desc) > TAILLE_DESC)
            {
                printf("Description invalide, veuillez saisir une description valide.\n");
            }
        } while (strlen(unproduit.desc) == 0 || strlen(unproduit.desc) > TAILLE_DESC);
        convMaj(unproduit.desc);

        // Saisir le prix du produit
        do
        {
            printf("Prix : ");
            scanf("%f", &unproduit.prix);
            viderBuffer();
            if (unproduit.prix <= 0)
            {
                printf("Prix invalide, veuillez saisir un prix valide.\n");
            }
        } while (unproduit.prix <= 0);

        // Afficher les informations saisies
        printf(">>> Produit numéro %d enregistré.\n", unproduit.code);
        tabproduit[i++] = unproduit; // Sauvegarder les données saisies dans le tableau
        a_sauvegarder_produit = 1;   // Activer le flag pour sauvegarder les données

        // Demander à l'utilisateur s'il souhaite continuer
        printf("Voulez-vous saisir un autre produit ? (o/n) : ");
        scanf("%s", reponse);
        viderBuffer();
        convMaj(reponse);
    } while (reponse[0] == 'O');

    nbproduit = i; // Mettre à jour le nombre des produits
}

// Fonction pour demander le critère de recherche au utilisateur pour les produits
void demanderCritereRechercheProduits()
{
    int rechercher_choix = VAL_INI;

    if (nbproduit == 0)
    {
        printf(">>> Aucun produit à afficher\n");
        return;
    }

    while (rechercher_choix != 0)
    {
        printf("Choisir critère de recherche : \n");
        printf("-1- Code produit\n");
        printf("-2- Description\n");
        printf("-3- Prix\n");
        printf("-0- Revenir au menu précédent\n\n");
        printf("Votre choix : ");
        scanf("%d", &rechercher_choix);
        viderBuffer();
        struct produit recherche_produit = {0}; // Initialiser la structure de recherche
        switch (rechercher_choix)
        {
        case 1:
            printf("Code produit à rechercher : ");
            scanf("%d", &recherche_produit.code);
            rechercherProduit(recherche_produit);
            break;
        case 2:
            printf("Description à rechercher : ");
            scanf("%s", recherche_produit.desc);
            rechercherProduit(recherche_produit);
            break;
        case 3:
            printf("Prix à rechercher : ");
            scanf("%f", &recherche_produit.prix);
            rechercherProduit(recherche_produit);
            break;
        case 0:
            break;
        default:
            printf(">>> Option invalide. Veuillez réessayer.\n");
        }
    }
}

// Fonction pour rechercher un produit par critère de recherche
void rechercherProduit(struct produit produit_a_trouver)
{
    int nbproduit_trouve = 0; // Nombre de produits trouvés

    // Afficher l'en-tête seulement s'il y a des produits à afficher
    if (nbproduit > 0)
    {
        afficherEnTeteProduits();
    }

    for (int i = 0; i < nbproduit; i++)
    {
        bool match = false; // Indicateur si le produit actuel correspond aux critères

        // Vérifier les critères de recherche
        if ((produit_a_trouver.code > 0 && tabproduit[i].code == produit_a_trouver.code) ||
            (strlen(produit_a_trouver.desc) > 0 && strcmp(tabproduit[i].desc, produit_a_trouver.desc) == 0) ||
            (produit_a_trouver.prix > 0 && tabproduit[i].prix == produit_a_trouver.prix))
        {
            match = true;
        }

        // Si un match est trouvé, afficher le produit
        if (match)
        {
            printf("%-6d %-40s %-10.2f\n", tabproduit[i].code, tabproduit[i].desc, tabproduit[i].prix);
            nbproduit_trouve++;
        }
    }

    if (nbproduit_trouve == 0)
    {
        printf(">>> Aucun produit trouvé correspondant aux critères.\n");
    }
}

// Fonction pour modifier un produit par son code
void modifierProduit()
{
    struct produit unproduit;
    int code_produit_a_modifier, trouve;

    printf("Entrez le code du produit à modifier : ");
    scanf("%d", &code_produit_a_modifier);
    viderBuffer();

    trouve = lanceRechercheProduit(code_produit_a_modifier);
    if (trouve == VAL_INI)
    {
        printf(">>> Produit code %d non trouvé\n", code_produit_a_modifier);
    }
    else
    {
        unproduit = tabproduit[trouve];

        // Afficher les informations actuelles
        printf("Produit trouvé : \n");
        afficherEnTeteProduits();
        printf("%-6d %-40s %-10.2f\n", unproduit.code, unproduit.desc, unproduit.prix);

        // mise à jour des informations
        // Saisir le nouveau code
        do
        {
            printf("Nouveau code produit : ");
            scanf("%d", &unproduit.code);
            viderBuffer();
            if (unproduit.code <= 0 || unproduit.code > TAILLE_TAB)
            {
                printf("Code invalide, veuillez saisir un code valide.\n");
            }
        } while (unproduit.code <= 0 || unproduit.code > TAILLE_TAB);
        // Saisir la nouvelle description
        do
        {
            printf("Nouvelle description : ");
            scanf(" %[^\n]", unproduit.desc); // %[^\n] permet de lire une chaîne de caractères avec des espaces
            viderBuffer();
            if (strlen(unproduit.desc) == 0 || strlen(unproduit.desc) > TAILLE_DESC)
            {
                printf("Description invalide, veuillez saisir une description valide.\n");
            }
        } while (strlen(unproduit.desc) == 0 || strlen(unproduit.desc) > TAILLE_DESC);
        convMaj(unproduit.desc);
        // Saisir le nouveau prix
        do
        {
            printf("Nouveau prix : ");
            scanf("%f", &unproduit.prix);
            viderBuffer();
            if (unproduit.prix <= 0)
            {
                printf("Prix invalide, veuillez saisir un prix valide.\n");
            }
        } while (unproduit.prix <= 0);

        // Sauvegarde des modifications
        tabproduit[trouve] = unproduit;
        a_sauvegarder_produit = 1;
        printf(">>> Produit code %d modifié\n", code_produit_a_modifier);
    }
}

// Fonction pour supprimer un produit par son code
void supprimerProduit()
{
    struct produit unproduit;
    int code_produit_a_supprimer, trouve;

    printf("Entrez le code du produit à supprimer : ");
    scanf("%d", &code_produit_a_supprimer);
    viderBuffer();

    trouve = lanceRechercheProduit(code_produit_a_supprimer);
    if (trouve == VAL_INI)
    {
        printf(">>> Produit code %d non trouvé\n", code_produit_a_supprimer);
    }
    else
    {
        unproduit = tabproduit[trouve];

        // Afficher les informations actuelles
        printf("Produit trouvé : \n");
        afficherEnTeteProduits();
        printf("%-6d %-40s %-10.2f\n", unproduit.code, unproduit.desc, unproduit.prix);

        // Demander confirmation pour supprimer le produit
        char reponse[TAILLE_CHAR];
        printf("Confirmez-vous la suppression du produit (o/n) : ");
        scanf("%s", reponse);
        viderBuffer();
        convMaj(reponse);
        if (reponse[0] == 'O')
        {
            // Supprimer le produit
            for (int i = trouve; i < nbproduit - 1; i++)
            {
                tabproduit[i] = tabproduit[i + 1];
            }
            nbproduit--;
            a_sauvegarder_produit = 1;
            printf(">>> Produit code %d supprimé\n", code_produit_a_supprimer);
        }
    }
}

// Fonction pour sauvegarder les produits dans la base de données
void sauvegardeProduits()
{
    FILE *f1 = fopen(DB_PRODUITS, "wb");
    if (f1 == NULL)
    {
        printf(">>> Erreur d'ouverture de la base de données des produits\n");
        return;
    }
    for (int i = 0; i < nbproduit; i++) // boucle de sauvegarde
    {
        fprintf(f1, "%-6d %-40s %-10.2f\n", tabproduit[i].code, tabproduit[i].desc, tabproduit[i].prix);
    }
    fclose(f1);
    a_sauvegarder_produit = 0; // désactiver le flag pour sauvegarder les données
    printf(">>> %d produits sauvegardés\n", nbproduit);
}

// Fonction pour charger les produits depuis la base de données
void chargementProduits()
{
    struct produit unproduit;
    FILE *f1 = fopen(DB_PRODUITS, "rb");
    int ret;

    if (f1 == NULL)
    {
        printf("Erreur d'ouverture de la base de données des produits.\n");
        return;
    }

    while ((ret = fscanf(f1, "%6d %39[^\n] %10f\n", &unproduit.code, unproduit.desc, &unproduit.prix)) == 3)
    {
        if (nbproduit < TAILLE_TAB)
        {
            tabproduit[nbproduit] = unproduit;
            nbproduit++;
        }
        else
        {
            printf("Limite du tableau de produits atteinte.\n");
            break;
        }
    }

    fclose(f1);
    printf("%d produits chargés.\n", nbproduit);
}

// Fonction pour rechercher un produit par son code, retourne l'index du produit dans le tableau des produits
int lanceRechercheProduit(int code_produit_a_rechercher)
{
    struct produit unproduit;
    int i, idx_code_produit = VAL_INI;

    // boucle de recherche
    for (i = 0; i < nbproduit; i++)
    {
        unproduit = tabproduit[i];
        if (unproduit.code == code_produit_a_rechercher)
        {
            idx_code_produit = i;
            break;
        }
    }
    return idx_code_produit;
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

// Fonction pour vérifier si une code produit est valide
bool codeProduitValide(int code)
{
    if (lanceRechercheProduit(code) == VAL_INI)
        return false;
    return true;
}

// Fonction pour vérifier si une numéro de chambre est valide
bool chambreValide(int chambre)
{
    for (int i = 0; i < 50; i++)
    {
        if (chambre == listeChambre[i])
            return true;
    }
    return false;
}

// Fonction pour convertir le niveau de fidélité en chaine de caractères
char *niveauFideliteToString(int nf)
{
    switch (nf)
    {
    case 0:
        return "STARTER";
    case 1:
        return "BRONZE";
    case 2:
        return "SILVER";
    case 3:
        return "GOLD";
    case 4:
        return "PLATINUM";
    default:
        return "Inconnu";
    }
}

// Fonction pour mettre à jour le niveau de fidélité d'un client en fonction du montant total des factures
int calculerFidelite(float total)
{
    if (total < 2000)
        return 0;
    if (total < 5000)
        return 1;
    if (total < 10000)
        return 2;
    if (total < 20000)
        return 3;
    return 4;
}

// Fonction pour appliquer le rabais en fonction du niveau de fidélité
float appliquerFidelite(float total, int numero_client)
{
    float rabais = 0;
    int idx_client = lanceRechercheClient(numero_client);
    if (tabclient[idx_client].fidelite == 1)
    {
        printf("Rabais de 5%% appliqué pour client BRONZE\n");
        rabais = total * 0.05;
    }
    if (tabclient[idx_client].fidelite == 2)
    {
        printf("Rabais de 10%% appliqué pour client SILVER\n");
        rabais = total * 0.1;
    }
    if (tabclient[idx_client].fidelite == 3)
    {
        printf("Rabais de 15%% appliqué pour client GOLD\n");
        rabais = total * 0.15;
    }
    if (tabclient[idx_client].fidelite == 4)
    {
        printf("Rabais de 20%% appliqué pour client PLATINUM\n");
        rabais = total * 0.2;
    }
    return (total - rabais);
}
// Fonction pour convertir le statut de la facture en chaine de caractères
char *statutFactureToString(int sf)
{
    switch (sf)
    {
    case 0:
        return "NON PAYEE";
    case 1:
        return "PAYEE";
    default:
        return "Inconnu";
    }
}

// Fonction pour vérifier si une chambre est disponible pour une période donnée
bool chambreDisponible(int chambre, struct date date_entree, struct date date_sortie)
{
    for (int i = 0; i < nbresa; i++)
    {
        if (tabresa[i].chambre == chambre)
        {
            // Vérifier si la date d'entrée ou de sortie est comprise dans une réservation existante
            if (dateSuperieure(tabresa[i].date_sortie, date_entree) && dateSuperieure(date_sortie, tabresa[i].date_entree))
            {
                return false; // Chambre non disponible
            }
        }
    }
    return true; // Chambre disponible
}

// Fonction pour afficher les chambres disponibles pour une période donnée
void afficherChambresDisponibles(struct date date_entree, struct date date_sortie)
{
    printf("Chambres disponibles pour la période du %02d/%02d/%04d au %02d/%02d/%04d\n", date_entree.jour, date_entree.mois, date_entree.annee, date_sortie.jour, date_sortie.mois, date_sortie.annee);
    printf("Chambre Standard : ");
    for (int i = 0; i < 32; i++)
    {
        if (chambreDisponible(listeChambre[i], date_entree, date_sortie))
        {
            printf("%d ", listeChambre[i]);
        }
    }
    printf("\nChambre Familiales : ");
    for (int i = 32; i < 42; i++)
    {
        if (chambreDisponible(listeChambre[i], date_entree, date_sortie))
        {
            printf("%d ", listeChambre[i]);
        }
    }
    printf("\nSuites : ");
    for (int i = 42; i < 50; i++)
    {
        if (chambreDisponible(listeChambre[i], date_entree, date_sortie))
        {
            printf("%d ", listeChambre[i]);
        }
    }
    printf("\n");
}

// Fonction pour obtenir le code produit pour une chambre donnée
int codeProduitChambre(int chambre)
{
    if (chambre >= 101 && chambre <= 310) // Chambre standard
        return 101;
    if (chambre >= 401 && chambre <= 410) // Chambre supérieure
        return 102;
    if (chambre >= 501 && chambre <= 508) // Suite junior
        return 103;
    return 0;
}

// Fonction pour vérifier s'il y a des données à sauvegarder, déclencher par le flag
void verifSauvegarde()
{
    char reponse[TAILLE_CHAR];

    if (a_sauvegarder_reservation || a_sauvegarder_client || a_sauvegarder_produit || a_sauvegarder_pc || a_sauvegarder_facture)
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
            if (a_sauvegarder_produit)
            {
                sauvegardeProduits();
            }
            if (a_sauvegarder_pc)
            {
                sauvegardePC();
            }
            if (a_sauvegarder_facture)
            {
                sauvegardeFactures();
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

// Fonction pour calculer le nombre de jours entre deux dates
int nbJoursEntreDates(struct date d1, struct date d2)
{
    int nbjours = 0;
    int jours_par_mois[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    while (!dateEgale(d1, d2))
    {
        nbjours++;
        d1.jour++;
        if (d1.mois == 2 && bissextile(d1.annee))
        {
            jours_par_mois[1] = 29; // Set February to 29 days for leap years
        }
        else
        {
            jours_par_mois[1] = 28; // Reset February back to 28 days for non-leap years
        }

        if (d1.jour > jours_par_mois[d1.mois - 1])
        {
            d1.jour = 1;
            d1.mois++;
            if (d1.mois > 12)
            {
                d1.mois = 1;
                d1.annee++;
            }
        }
    }
    return nbjours;
}

// Fonction pour vérifier si une date est supérieure à une autre, retourne true si d1 > d2
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

// Fonction pour obtenir la date actuelle
void obtenirDateActuelle(struct date *d)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    d->jour = tm.tm_mday;
    d->mois = tm.tm_mon + 1;
    d->annee = tm.tm_year + 1900;
}

// Fonction pour obtenir la date du jour sous forme de chaine de caractères
void obtenirDateDuJour(char *dateStr)
{
    struct date d;
    obtenirDateActuelle(&d);
    dateToString(d, dateStr);
}

// Fonction pour générer un numéro de réservation unique au format "yyyyxxxx"
int genererNumResa()
{
    // obtenir l'année de la dernière réservation
    int annee_dernier_resa = num_resa / 10000; // extraire l'année de la dernière réservation
    int annee_actuel = obtenirAnneeActuelle();
    if (annee_actuel != annee_dernier_resa)
    {
        annee_dernier_resa = annee_actuel;
        num_resa = annee_actuel * 10000 + 1;
    }
    else
    {
        num_resa += 1;
    }
    return num_resa;
}

// Fonction pour générer un code client unique
int genererCodeClient()
{
    nbclient += 1;
    return nbclient;
}

// Fonction pour générer un numéro de facture unique
int genererNumFacture()
{
    nbfacture += 1;
    return nbfacture;
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