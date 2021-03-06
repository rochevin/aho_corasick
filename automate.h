#ifndef _AUTOMATE_
#define _AUTOMATE_

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

//Alphabet
int EstDansAlphabet(char*,int*,char);
char* AjouterLettre(char*,int*,char);
char* AjouterMot(char*,int*,char*);


//Fonctions Automate / Transition
Automate allouer_automate(int);
Automate init_automate(char*,int,int) ;
int* creer_estTerminal(int) ;
void mettreTerminal(int*,etat) ;
int sortie(int*,etat) ;
int estTransitionVide(Transition);
Transition* creer_transition(int) ;
void ajout_transition(Transition*,etat,etat,char) ;
void setDestination(Transition,etat) ;
int getDestination(Transition) ;
void setLettre(Transition,char) ;
char getLettre(Transition) ;
void TsetSuivant(Transition,Transition) ;
Transition TgetSuivant(Transition) ;
int EstDefinieTransition(Transition,char) ;
Transition Cible(Transition,char) ;
void ajout_suppleant(int*,etat,etat) ;

//Affichage de l'automate
void PrintAutomate(Automate) ;
void printTerminal(Automate);
void printTransitions(Automate);
void printSuppleants(Automate);

#endif
