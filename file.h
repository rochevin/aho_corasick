#ifndef FIFO

#define FIFO

struct _liste {
	int element;
	struct _liste * suivant;
};

typedef struct _liste * Liste;

struct _file {
	int longueur ;
	struct _liste * tete;
};

typedef struct _file * File;

File FileVide();
Liste listeVide();
Liste LgetSuivant(Liste l);
void LsetSuivant(Liste l,Liste p);
int getElement(Liste l);
void setElement(Liste l, int x);
int estListeVide(Liste l);
int estFileVide(File f);
File Enfiler(File f, int x);
File Defiler(File f);
void FreeFile(File f);
void AfficherFile(File f);

#endif
