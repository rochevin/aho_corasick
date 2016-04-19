#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ac.h"
#include "file.h"

//Fichiers
char* GetTexte(char* f){
	FILE *fichier ;
	if ((fichier = fopen(f, "r")) == NULL) {
		fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", f) ;
		exit(EXIT_FAILURE) ;
	}
	fseek(fichier, 0, SEEK_END) ;
	long taille = ftell(fichier) ;
	rewind(fichier) ;
	char* texte = (char*)malloc(taille*sizeof(char)) ;

	if (texte == NULL) {
		fprintf(stderr, "Problème allocation mémoire...\n") ;
		exit(EXIT_FAILURE) ;
	}

	fread(texte , taille, 1, fichier) ;

	fclose(fichier) ;

	return texte ;
}



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

int sortie(int* estT, etat e){ return estT[e] == 1; }

Automate init_automate(char* alphabet,int taille_alphabet){
	Automate a = allouer_automate() ; //Création de l'automate (vide pour l'instant)
	a->tailleAlpha = taille_alphabet ;
	a->alphabet = alphabet ;
	a->EtatInitial = 0 ;
	a->nbEtats = 1 ; // Pour l'instant, un seul état, l'état initial 0
	a->EstTerminal = (int*)malloc(sizeof(int)) ; //On alloue la place dans le tableau des etats terminaux pour 0
	a->EstTerminal[0] = 0 ; // On set l'état 0 comme étant non terminal
	a->tabListeTrans = (Transition*)malloc(sizeof(struct _transition)); ; //On alloue de la mémoire pour un tableau de taille 1 pour l'état initial
	a->tabListeTrans[0] = NULL; //On set l'état initial comme n'ayant aucune transition
	return a;
}

//Fonction qui modifie l'automate pour ajouter l'espace mémoire nécessaire pour le mot donné.
Automate reallouer_automate(Automate a,char* mot, int taille_du_mot){
	int etats_avant = a->nbEtats ; //On garde en mémoire le nombre d'états précédents

	a->nbEtats = a->nbEtats + taille_du_mot ; // Il y'a autant d'états que le nombre de lettres dans chaque mot + etat initial
	a->EstTerminal = (int*)realloc(a->EstTerminal, sizeof(int) * (a->nbEtats)); ; //On realloue la mémoire nécessaire pour contenir tous les états
	//On set tous les nouveaux etats comme étant non terminaux
	for(int i=etats_avant;i<a->nbEtats;i++) {
		a->EstTerminal[i] = 0 ;
	}
	a->tabListeTrans = (Transition*)realloc(a->tabListeTrans, sizeof(struct _transition) * (a->nbEtats)) ; //On alloue de la mémoire pour un tableau de taille 1 pour l'état initial
	//On set tous les nouveaux états comme n'ayant aucune transition
	for(int i=etats_avant;i<a->nbEtats;i++) {
		a->tabListeTrans[i] = NULL ;
	}
	return a ;
}

//Fonctions Transition

//Renvoit une comparaison de type booléen, si NULL 1, si non NULL 0
int estTransitionVide(Transition t) { return t == NULL ;}
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
void TsetSuivant(Transition t,Transition s) {
	// Précondition : t est non vide
	t->suivant = s ;
}

Transition TgetSuivant(Transition t){
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
	TsetSuivant(t, tabT[source]) ;

	tabT[source] = t ;
}

int EstDefinieTransition(Transition t,etat e,char l){
	while(!estTransitionVide(t)){
		if(getLettre(t) == l)
			return 1 ;
		t=TgetSuivant(t);
	}
	return 0 ;
}

Transition Cible(Transition t,char lettre) {
	while(!estTransitionVide(TgetSuivant(t))) {
		if(getLettre(t) == lettre) {
			return t ;
		}
		else{
			t = TgetSuivant(t) ;
		}
	}
	return NULL ;
}

//Algos d'aho coco
Automate ENTRER(Automate a,char* mot, etat e){
	int i = 0;
	int taille_du_mot = strlen(mot) ;

	//Avant de commencer on modifie l'automate pour accepter le nouveau mot
	int nb_etats_avant_realloc = a->nbEtats ;
	a = reallouer_automate(a,mot,taille_du_mot) ;

	//On parcours les lettres du mot et on recupère l'état qui est pointé par la transition via la lettre
	while (i < taille_du_mot && EstDefinieTransition(a->tabListeTrans[e],e, mot[i]) == 1) {
		e = getDestination(Cible(a->tabListeTrans[e],mot[i])) ; //On récupère la destination de la transition qui part de e pour mot[i]
		i++ ;
	}
	while (i < taille_du_mot) {
		char lettre = mot[i];
		etat s = nb_etats_avant_realloc + 1;
		ajout_transition(a->tabListeTrans, e,s, lettre) ;
		i++ ;
	}
	mettreTerminal(a->EstTerminal,e);

	return a ;
}

Automate COMPLETER(Automate a){
	File f = FileVide() ;
	etat e ;
	
	Transition t = a->tabListeTrans[a->EtatInitial] ; //Liste de toutes les transitions partants de l'état initial
	//Remplissage de la file pour tous les enfants de la racine + ajout de suppléance pour ces enfants vers la racine
	while(!estTransitionVide(t)){
		if(getDestination(t) == a->EtatInitial){ //On ne prend pas en compte les transition de l'état initial vers l'état initial
			t = TgetSuivant(t) ;
			continue;
		}else{
			e = getDestination(t);
			Enfiler(f,e) ;
			t = TgetSuivant(t) ;
			ajout_transition(a->tabSuppleants, e,a->EtatInitial,getLettre(t));
		}
	}
	while(!estFileVide(f)){
		e = getElement(f->tete);
		Defiler(f);

		Transition t = a->tabListeTrans[e] ; //On récupère toutes les transitions de l'état
		//et on fait le même parcours que pour l'état initial
		while(!estTransitionVide(t)){
			//On récupère l'état pour chaque transition, et on l'ajoute dans la file
			e = getDestination(t);
			Enfiler(f,e) ;
			t = TgetSuivant(t) ;

			//On prend le suppléant de l'état actuel 
			etat s = getDestination(a->tabSuppleants[e]) ;
			//Et on recherche son suppléant
			while(!EstDefinieTransition(t, s, t->lettre)){
				s = getDestination(a->tabSuppleants[s]) ;
			}
			ajout_transition(a->tabSuppleants,e,s,getLettre(t));
			mettreTerminal(a->EstTerminal,s) ;
		}

	}
	FreeFile(f) ;
	return a ;
}

Automate pre_ac(Automate a,char** liste_mots,int nombre_mots){
	
	for (int i = 0; i < a->tailleAlpha; i++) {
		char lettre = a->alphabet[i] ;
		ajout_transition(a->tabListeTrans, a->EtatInitial,a->EtatInitial, lettre) ;
	}

	for(int i = 0; i < nombre_mots;i++){
		a = ENTRER(a,liste_mots[i],a->EtatInitial);
	}
	a = COMPLETER(a) ;
	return a ;
}

// void AC(Automate a,char** liste_mots,int nombre_mots,char* texte, int taille_texte){
// 	a = pre_ac(a,liste_mots,nombre_mots) ;
// 	etat e = a->EtatInitial ;
// 	for(int j=0;j<taille_texte;j++){
// 		while(EstDefinieTransition(a->tabListeTrans[e],e,getLettre(a->tabListeTrans[e])) != 1){
// 			e = getDestination(a->tabListeTrans[e]) ; //On récupère le suppléant de l'état e (une seule transition pour chaque état dans tabListeTrans)
// 		}
// 		e = getDestination(Cible(a->tabListeTrans[e],getLettre(a->tabListeTrans[e]))) ;
// 		if(sortie(a->EstTerminal,e)) {
			
// 		}
// 	}

// }

int EstDansAlphabet(char* alphabet,int taille_alphabet,char lettre) {
	if(alphabet == NULL)
		return 0;
	for(int i=0;i<taille_alphabet;i++){
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
	for(int i = 0; i<strlen(mot);i++){
		if(EstDansAlphabet(alphabet,taille,mot[i]) != 1)
			AjouterLettre(alphabet,&taille,mot[i]) ;
	}
}



