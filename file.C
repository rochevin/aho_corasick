#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

File FileVide(){
	File f = (File)malloc(sizeof(struct _file)) ; 
	if (f == 0) exit(1) ;
	f->longueur = 0 ;
	f->tete = f->queue = listeVide() ;
	return f ;
}

//Fonction qui renvoit une structure Liste nulle
Liste listeVide() { return NULL ;}
//Donne l'élement suivant de la liste
Liste getSuivant(Liste l) {
	// Précondition : l est non vide
	return l->suivant; 
}
//Définit l'élement suivant à partir de p
void setSuivant(Liste l,Liste p) { 
	// Précondition : l est non vide
	l->suivant = p; 
}

//Retourne l'élement de la structure Liste
int getElement(Liste l) {
	// Précondition : l est non vide
	return l->element ; 
}
//Définit l'élement de Liste
void setElement(Liste l, int x) { 
	// Précondition : l est non vide
	l->element= x ; 
}

//Renvoit une comparaison de type booléen, si NULL 1, si non NULL 0
int estListeVide(Liste l) { return l == NULL ;}


int estFileVide(File f) { return f->longueur == NULL ;}

//fonction enfiler en queue
File Enfiler(File f, int x) {
	Liste p = (Liste)malloc(sizeof(struct _liste)) ; 
	if (p == 0) exit(1) ;
	setSuivant(p, NULL) ;
	setElement(p, x) ;
	if(f->longueur == 0){
		f->tete = f->queue = p;
	}else {
		setSuivant(l, p) ;
	}
	f->longueur = f->longueur + 1 ;
	return f ; 
}

File Defiler(File f){
	if(!estListeVide(l)) return NULL ;
	Liste l = f->tete ;
	if(f->longueur == 1){
		f->tete = f->queue = listeVide();
	}else {
		f->tete = getSuivant(l) ;
		free(l->element) ;
		free(l) ;
		f->longueur = f->longueur - 1 ;
	}
	return f;
}
void FreeFile(File f) {
	while (! estFileVide(f)) {
		Defiler(f) ;
	}
}