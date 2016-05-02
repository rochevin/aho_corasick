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

int* creer_estTerminal(int nbE) {
	int* t = (int*)malloc(nbE * sizeof(int));
	for(int i=0;i<nbE;i++) {
		t[i] = 0 ;
	}
	return t ;
}

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
	a->tabSuppleants = creer_estTerminal(etats_max);
	return a ;
}



void mettreTerminal(int* estT, etat e){
	estT[e]=1 ;
}

int sortie(int* estT, etat e){ return estT[e] == 1; }

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
	//printf("Ajout transition de %d vers %d pour %c\n",source,dest,a) ;
}

int EstDefinieTransition(Transition t,char l){
	while(!estTransitionVide(t)){
		if(getLettre(t) == l){
			return 1 ;
		}
		t=TgetSuivant(t);
	}
	return 0 ;
}

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


void ajout_suppleant(int* tabT, etat source,etat dest){
	tabT[source] = dest ;
}

//Algos d'aho coco
Automate ENTRER(Automate a,char* mot){
	int i = 0; //On set le compteur qui parcours le mot à 0
	int taille_du_mot = strlen(mot) ;

	etat e = a->EtatInitial ;

	
	//Premier parcours du mot : on récupère l'état qui, si il existe une transition pour la lettre i, est la destination du plus long préfixe du mot déjà entré dans l'automate

	while (i < taille_du_mot && EstDefinieTransition(a->tabListeTrans[e], mot[i]) == 1) {
		e = getDestination(Cible(a->tabListeTrans[e],mot[i])) ; //On récupère la destination de la transition qui part de e pour mot[i]

		i++ ;
	}

	//Deuxième parcours : On créer un nouvel etat pour chaque nouvelle lettre à entrer dans l'automate, et on créer une transition de l'état précédent vers cet etat pour la lettre en position i

	while (i < taille_du_mot) {
		char lettre = mot[i];
		etat s = a->nbEtats; 
		(a->nbEtats)++; // On a créer un nouvel etat donc on incrémente le nombre d'états
		//a = reallouer_automate(a,s);
		ajout_transition(a->tabListeTrans, e,s, lettre) ;
		e = s;
		i++ ;
	}
	mettreTerminal(a->EstTerminal,e);

	return a ;
}

Automate COMPLETER(Automate a){
	File f = FileVide() ;
	etat e  = a->EtatInitial;
	etat child, s, r;

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
			ajout_suppleant(a->tabSuppleants, e,a->EtatInitial); //Retour vers l'état initial
		}
	}
	while(!estFileVide(f)){
		e = getElement(f->tete);
		Defiler(f);
		Transition t = a->tabListeTrans[e] ; //On récupère toutes les transitions de l'état
		//et on fait le même parcours que pour l'état initial

		while(!estTransitionVide(t)){
			//On récupère l'état pour chaque transition, et on l'ajoute dans la file
			char lettre_t = t->lettre ;
			child = getDestination(t) ;
			//printf("Destination de %d: %d\n",e,child);
			Enfiler(f,child) ;
			t = TgetSuivant(t);
			//AfficherFile(f);

			//On prend le suppléant de l'état actuel 
			s = a->tabSuppleants[e] ;
			//printf("Suppléant : %d\n",s);
			//Et on recherche son suppléant
			while(EstDefinieTransition(a->tabListeTrans[s], lettre_t) == 0){
				s = a->tabSuppleants[s] ;
				//printf("On récupère le suppléant de s : %d\n",s);
			}

			

			//printf("Suppléant : %d\n",s);
			r = getDestination(Cible(a->tabListeTrans[s],lettre_t)) ;
			ajout_suppleant(a->tabSuppleants, child,r);
			//printSuppleants(a); //Vérification des suppléants
			//printf("Destination finale : %d",r);
			if(sortie(a->EstTerminal,r) == 1){
				//printf(" TERMINALE");
				mettreTerminal(a->EstTerminal,child) ;
			}
			//printf("\n");

		}

	}
	FreeFile(f) ;
	return a ;
}

Automate pre_ac(Automate a,char** liste_mots,int nombre_mots){
	//PrintAutomate(a) ;
	etat e = a->EtatInitial ;
	for(int i = 0; i < nombre_mots;i++){
		printf("Ajout du mot : %s \n",liste_mots[i]) ;
		a = ENTRER(a,liste_mots[i]);
		//PrintAutomate(a) ;
	}
	for (int i = 0; i < a->tailleAlpha; i++) {
		char lettre = a->alphabet[i] ;
		if(!EstDefinieTransition(a->tabListeTrans[e],lettre)){
			ajout_transition(a->tabListeTrans, e,e, lettre) ;
		}
	}
	//PrintAutomate(a) ;
	//printf("Fonction COMPLETER\n") ;
	a = COMPLETER(a) ;
	//PrintAutomate(a) ;
	return a ;
}

void Aho_Corasick(Automate a,char** liste_mots,int nombre_mots,char* nom_fichier){
	
	char* texte = GetTexte(nom_fichier);
	char lettre;
	int taille_texte = strlen(texte);
	a = pre_ac(a,liste_mots,nombre_mots);
	etat e = a->EtatInitial ;
	int occurences = 0;
	for(int j=0;j<taille_texte;j++){
		lettre = texte[j] ;
		if(EstDansAlphabet(a->alphabet,&(a->tailleAlpha),lettre) == 0)
			continue;
		while(e != a->EtatInitial && EstDefinieTransition(a->tabListeTrans[e], lettre) == 0){
			
			e = a->tabSuppleants[e] ; //On récupère le suppléant de l'état e (une seule transition pour chaque état dans tabListeTrans)
		}
		e = getDestination(Cible(a->tabListeTrans[e], lettre)) ;
		if(sortie(a->EstTerminal,e) == 1) {
			occurences++;
			printf("occurence détéctée dans le texte en position %d\n",j+1) ;
		}
	}
	printf("%d occurences détéctées dans le texte\n",occurences) ;
}

int EstDansAlphabet(char* alphabet,int *taille_alphabet,char lettre) {
	if(alphabet == NULL)
		return 0;
	int taille = *taille_alphabet ;
	for(int i=0;i<taille;i++){
		if(alphabet[i]==lettre)
			return 1;
	}
	return 0;
}

char* AjouterLettre(char* alphabet,int *taille_alphabet,char lettre){
	int taille = *taille_alphabet ;
	if(alphabet == NULL){
		alphabet = (char*) malloc(sizeof(char));
		alphabet[0] = lettre;
	} else {
		alphabet = (char*) realloc(alphabet, sizeof(char) * (taille+1));
		alphabet[taille] = lettre;
	}
	*taille_alphabet = taille + 1;
	return alphabet ;
}



//fonction qui parcours un mot et ajoute la lettre dans l'alphabet si elle n'y est pas
char* AjouterMot(char* alphabet,int *taille_alphabet,char* mot) {
	int taille_du_mot = strlen(mot) ;
	for(int i = 0; i<taille_du_mot;i++){
		if(EstDansAlphabet(alphabet,taille_alphabet,mot[i]) != 1){
			alphabet = AjouterLettre(alphabet,taille_alphabet,mot[i]) ;
		}
	}
	return alphabet ;
}

void PrintAutomate(Automate a){
	printf("Alphabet : %s\n",a->alphabet) ;
	printf("Taille alphabet : %d\n",a->tailleAlpha) ;
	printf("Etat initial : %d\n",a->EtatInitial) ;
	printf("Nb etats : %d\n",a->nbEtats) ;
	printf("Etats terminaux : ") ;
	for(int i =0; i<a->nbEtats;i++){
		if(sortie(a->EstTerminal,i)){
			printf("%d ",i) ;
		}
	}

	printf("\nTransitions : \n") ;
	for(int i =0; i<a->nbEtats;i++){
		if(estTransitionVide(a->tabListeTrans[i])){
			continue ;
		}
		Transition t = a->tabListeTrans[i] ;
		while(!estTransitionVide(t)){
			printf("%d %c %d\n",i,getLettre(t),getDestination(t)) ;
			t=TgetSuivant(t);
		}
	}

	printf("Suppléants : \n") ;
	for(int i =0; i<a->nbEtats;i++){
		printf("%d : %d\n",i,a->tabSuppleants[i]) ;
	}

}

void printSuppleants(Automate a){
	printf("Suppléants : \n") ;
	for(int i =0; i<a->nbEtats;i++){
		printf("%d : %d\n",i,a->tabSuppleants[i]) ;
	}
}
