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
	struct _transition* tabListeTrans;
};

typedef struct _automate * Automate;