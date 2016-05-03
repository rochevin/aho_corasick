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
Liste LgetSuivant(Liste);
void LsetSuivant(Liste,Liste);
int getElement(Liste);
void setElement(Liste,int);
int estListeVide(Liste);
int estFileVide(File);
File Enfiler(File,int);
File Defiler(File);
void FreeFile(File);
void AfficherFile(File);

#endif
