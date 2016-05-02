#ifndef AC

#define AC

typedef int etat;

struct _transition {
	char lettre;
	etat destination;
	struct _transition* suivant;
};

typedef struct _transition * Transition;

struct _automate {
	int tailleAlpha;
	char* alphabet;
	int nbEtats;
	etat EtatInitial;
	int* EstTerminal;
	Transition* tabListeTrans;
	int* tabSuppleants;
};

typedef struct _automate * Automate;

char* GetTexte(char* f);
Automate allouer_automate(int etats_max);
int* creer_estTerminal(int nbE);
void mettreTerminal(int* estT, etat e);
int sortie(int* estT, etat e);
Automate init_automate(char* alphabet,int taille_alphabet,int etats_max);
Automate reallouer_automate(Automate a,etat e);
int estTransitionVide(Transition t);
void setDestination(Transition t,etat d);
int getDestination(Transition t);
void setLettre(Transition t,char l);
char getLettre(Transition t);
void setSuivant(Transition t,Transition s);
Transition getSuivant(Transition t);
Transition* creer_transition(int nbE);
void ajout_transition(Transition* tabT, etat source,etat dest, char a);
int EstDefinieTransition(Transition t,char l);
Automate ENTRER(Automate a,char* mot);
Automate COMPLETER(Automate a);
Automate pre_ac(Automate a,char** liste_mots,int nombre_mots);
void Aho_Corasick(Automate,char**,int,char*);
int EstDansAlphabet(char* alphabet,int *taille_alphabet,char lettre);
char* AjouterLettre(char* alphabet,int *taille_alphabet,char lettre);
char* AjouterMot(char* alphabet,int *taille_alphabet,char* mot);
void PrintAutomate(Automate a) ;
void printSuppleants(Automate a);
void ajout_suppleant(int* tabT, etat source,etat dest);
#endif
