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
	Transition* tabSuppleants;
};

typedef struct _automate * Automate;

char* GetTexte(char* f);
Automate allouer_automate();
int* creer_estTerminal(int nbE);
void mettreTerminal(int* estT, etat e);
int sortie(int* estT, etat e);
Automate init_automate(char* alphabet,int taille_alphabet,int etats_max);
Automate reallouer_automate(Automate a,char* mot, int taille_du_mot);
int estTransitionVide(Transition t);
void setDestination(Transition t,etat d);
int getDestination(Transition t);
void setLettre(Transition t,char l);
char getLettre(Transition t);
void setSuivant(Transition t,Transition s);
Transition getSuivant(Transition t);
Transition* creer_transition(int nbE);
void ajout_transition(Transition* tabT, etat source,etat dest, char a);
int EstDefinieTransition(Transition t,etat e,char l);
Automate ENTRER(Automate a,char* mot, etat e);
Automate COMPLETER(Automate a);
Automate pre_ac(Automate a,char** liste_mots,int nombre_mots);
//void AC(Automate a,char** liste_mots,int nombre_mots,char* texte, int taille_texte);
int EstDansAlphabet(char* alphabet,int *taille_alphabet,char lettre);
char* AjouterLettre(char* alphabet,int *taille_alphabet,char lettre);
char* AjouterMot(char* alphabet,int *taille_alphabet,char* mot);
void PrintAutomate(Automate a) ;
#endif
