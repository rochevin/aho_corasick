#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "automates.h"

//Fonctions Automate
Automate allouer_automate() {
	Automate a = (Automate)malloc(sizeof(struct _automate)) ;
	if (a == 0) exit(1) ;
	return a ;
}

int* creer_estTerminal(int nbE) {
	int* t = (int*)malloc(nbE * sizeof(int));
	for(int i=0;i<nbE;i++) {
		t[i] = 0 ;
	}
	return t ;
}

void mettreTerminal(int* estT, etat e){
	estT[e]=1 ;
}

int sortie(int* estT, etat e){
	return estT[e];
}

//Fonctions Transition

//Définit la destination à partir de d
void setDestination(Transition t,etat d) {
	// Précondition : t est non vide
	t->destination = d ;
}

int getDestination(Transition t){
	return t->destination ;
}

//Définit la lettre de t à partir de a
void setLettre(Transition t,char l) {
	// Précondition : t est non vide
	t->lettre = l ;
}

char getLettre(Transition t){
	return t->lettre ;
}

//Définit l'élement suivant à partir de s
void setSuivant(Transition t,Transition s) {
	// Précondition : t est non vide
	t->suivant = s ;
}

Transition getSuivant(Transition t){
	return t->suivant ;
}


Transition* creer_transition(int nbE){
	Transition* t = (Transition*)malloc(nbE * sizeof(struct _transition));
	for(int i=0;i<nbE;i++) {
		t[i] = NULL ;
	}
	return t ;
}

void ajout_transition(Transition* tabT, etat source,etat dest, char a){
	Transition t = (Transition)malloc(sizeof(struct _transition)) ;
	if (t == 0) exit(1) ;
	setLettre(t, a) ;
	setDestination(t, dest) ;
	setSuivant(t, tabT[source]) ;

	tabT[source] = t ;
}

int EstDefinieTransition(Transition t,etat e,char l){
	while(!estTransitionVide(t)){
		if(getLettre(t) == l)
			return 1 ;
		t=getSuivant(t);
	}
	return 0 ;
}

void ENTRER(Automate a,char mot, etat s){
	etat e = s ;
	int i = 0;
	int taille_du_mot = strlen(mot) ;
	//On parcours les lettres du mot et on recupère l'état qui est pointé par la transition via la lettre
	while (i < taille_du_mot && EstDefinieTransition(a->tabListeTrans[e],e, mot[i]) == 1) {
		e = getDestination(a->tabListeTrans[e],mot[i]) ;
		i++ ;
	}
	while (i < taille_du_mot) {
		etat d;
		i++ ;
	}
	

}

void pre_ac(Automate a,char* liste_mots, nombre_mots){
	
	for (int i = 0; i < a->tailleAlpha; i++) {
		char lettre = a->alphabet[i] ;
		ajout_transition(a->tabListeTrans, a->EtatInitial,a->EtatInitial, lettre) ;
	}

	for(int i = 0; i < nombre_mots;i++){
		ENTRER(a,liste_mots[i],a->EtatInitial);
	}

}


int EstDansAlphabet(char* alphabet,int *taille_alphabet,char lettre) {
	if(alphabet == NULL)
		return 0
	int taille = *taille_alphabet
	int i;
	for(i=0;i<taille;i++){
		if(alphabet[i]==lettre)
			return 1;
	}
	return 0;
}

void AjouterLettre(char* alphabet,int *taille_alphabet,char lettre){
	int taille = *taille_alphabet ;
	if(alphabet == NULL){
		alphabet = (char*) malloc(sizeof(char));
		alphabet[0] = lettre;
	} else {
		alphabet = (char*) realloc(alphabet, sizeof(char) * (taille+1));
		alphabet[taille] = lettre;
	}
	*taille_alphabet = taille + 1;
}



//fonction qui parcours un mot et ajoute la lettre dans l'alphabet si elle n'y est pas
void AjouterMot(char* alphabet,int *taille_alphabet,char* mot) {
	int taille = *taille_alphabet ;
	for(int i = 0, i<strlen(mot);i++){
		if(EstDansAlphabet(mot[i]) != 1)
			AjouterLettre(alphabet,taille,mot[i]) ;
	}
}



