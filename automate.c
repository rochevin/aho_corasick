#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "automate.h"

//Fonctions Automate / Transition

Automate allouer_automate(int etats_max) {
	Automate a = (Automate)malloc(sizeof(struct _automate)) ;
	if (a == 0) exit(1) ;
	//Tableaux de l'automate
	//Chaque tableau est alloué en fonction du nombre maximum d'états possible
	
	//On alloue le tableaux des etats terminaux
	a->EstTerminal = creer_estTerminal(etats_max) ; 
	
	//On alloue de la mémoire pour un tableau de transition
	a->tabListeTrans = creer_transition(etats_max);
	
	//On alloue de la mémoire pour le tableau de suppléants
	//Pour l'instant on set tout pointant vers 0
	a->tabSuppleants = creer_estTerminal(etats_max);
	return a ;
}

Automate init_automate(char* alphabet,int taille_alphabet,int etats_max){
	Automate a = allouer_automate(etats_max) ; //Création de l'automate (vide pour l'instant)
	//Définition de l'alphabet
	a->tailleAlpha = taille_alphabet ;
	a->alphabet = alphabet ;
	//Définition de l'état initial
	a->EtatInitial = 0 ;
	a->nbEtats = 1 ; // Pour l'instant, un seul état, l'état initial 0
	
	return a;
}

//Création du tableau etat terminaux
int* creer_estTerminal(int nbE) {
	int* t = (int*)malloc(nbE * sizeof(int));
	for(int i=0;i<nbE;i++) {
		t[i] = 0 ;
	}
	return t ;
}
//Fonction qui met un etat en etat terminal
void mettreTerminal(int* estT, etat e){
	estT[e]=1 ;
}

//Fonction qui retourne TRUE si l'état donnée est terminal
int sortie(int* estT, etat e){ return estT[e] == 1; }

//Renvoit une comparaison de type booléen, si NULL 1, si non NULL 0
int estTransitionVide(Transition t) { return t == NULL ;}

//Création du tableau de transition à NULL pour chaque etats
Transition* creer_transition(int nbE){
	Transition* t = (Transition*)malloc(nbE * sizeof(struct _transition));
	for(int i=0;i<nbE;i++) {
		t[i] = NULL ;
	}
	return t ;
}

//Création d'une transition d'un état source vers un etat dest pour la lettre a
void ajout_transition(Transition* tabT, etat source,etat dest, char a){
	Transition t = (Transition)malloc(sizeof(struct _transition)) ;
	if (t == 0) exit(1) ;
	setLettre(t, a) ;
	setDestination(t, dest) ;
	TsetSuivant(t, tabT[source]) ;

	tabT[source] = t ;
}

//Définit la destination à partir de d
void setDestination(Transition t,etat d) {
	// Précondition : t est non vide
	t->destination = d ;
}

//Retourne la destination d'une transition t donnée
int getDestination(Transition t){
	return t->destination ;
}

//Définit la lettre de t à partir de l
void setLettre(Transition t,char l) {
	// Précondition : t est non vide
	t->lettre = l ;
}

//Retourne la lettre correspondant à la transition t
char getLettre(Transition t){
	// Précondition : t est non vide
	return t->lettre ;
}

//Définit la transition s comme étant suivante de t
void TsetSuivant(Transition t,Transition s) {
	// Précondition : t est non vide
	t->suivant = s ;
}

//Retourne la transition suivante de t
Transition TgetSuivant(Transition t){
	return t->suivant ;
}


//Retourne 1 si la transition existe, 0 si la transition n'existe pas pour une lettre donnée l
int EstDefinieTransition(Transition t,char l){
	while(!estTransitionVide(t)){
		if(getLettre(t) == l){
			return 1 ;
		}
		t=TgetSuivant(t);
	}
	return 0 ;
}

//retourne la transition t pour une lettre donnée 
Transition Cible(Transition t,char lettre) {
	while(!estTransitionVide(t)) {
		if(getLettre(t) == lettre) {
			return t ;
		}
		else{
			t = TgetSuivant(t) ;
		}
	}
	return NULL ;
}

//Fonction qui définit dest comme étant suppléant de source
void ajout_suppleant(int* tabT, etat source,etat dest){
	tabT[source] = dest ;
}
