#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "automate.h"
#include "ac.h"
#include "file.h"
#include "fichier.h"


//AHO CORASICK

//Fonction d'ajout d'un mot dans l'automate
Automate ENTRER(Automate a,char* mot){
	int i = 0; //On set le compteur qui parcours le mot à 0
	int taille_du_mot = strlen(mot) ;

	etat e = a->EtatInitial ; //On part de l'état initial

	
	//Premier parcours du mot : on récupère l'état qui, si il existe une transition pour la lettre i, est la destination du plus long préfixe du mot déjà entré dans l'automate

	while (i < taille_du_mot && EstDefinieTransition(a->tabListeTrans[e], mot[i]) == 1) {
		e = getDestination(Cible(a->tabListeTrans[e],mot[i])) ; //On récupère la destination de la transition qui part de e pour mot[i]
		i++ ;
	}

	//Deuxième parcours : On créer un nouvel etat pour chaque nouvelle lettre à entrer dans l'automate, et on créer une transition de l'état précédent vers cet etat pour la lettre en position i

	while (i < taille_du_mot) {
		char lettre = mot[i];
		etat s = a->nbEtats; //Pour déterminer l'indice du nouvel etat créer, il suffit de connaitre le nombre d'états avant l'ajout de celui-ci
		(a->nbEtats)++; // On a créer un nouvel etat donc on incrémente le nombre d'états
		ajout_transition(a->tabListeTrans, e,s, lettre) ; //Puis on ajoute une transition vers cet etat pour la lettre
		e = s; //On continue ensuite à partir de cet etat
		i++ ;
	}
	mettreTerminal(a->EstTerminal,e); //une fois le mot terminé, on défini l'état e comme étant terminal

	return a ;
}

//Fonction de suppléance
//Redirige vers un etat qui est pointé par une transition identique
Automate COMPLETER(Automate a){
	File f = FileVide() ; //On définit une file vide
	etat e  = a->EtatInitial; //On part de l'état initial
	etat child, s, r; 

	Transition t = a->tabListeTrans[a->EtatInitial] ; //Liste de toutes les transitions partants de l'état initial
	//Remplissage de la file pour tous les enfants de la racine + ajout de suppléance pour ces enfants vers la racine
	while(!estTransitionVide(t)){
		if(getDestination(t) == a->EtatInitial){ //On ne prend pas en compte les transition de l'état initial vers l'état initial
			t = TgetSuivant(t) ;
			continue;
		}else{
			e = getDestination(t); //On récupère la destination de t
			Enfiler(f,e) ; //On met cet etat dans la file
			t = TgetSuivant(t) ; //On recupère l'élement suivant
			ajout_suppleant(a->tabSuppleants, e,a->EtatInitial); //Retour vers l'état initial
		}
	}
	//On parcours ensuite la file tant qu'elle n'est pas vide
	while(!estFileVide(f)){
		e = getElement(f->tete); //On récupère l'élement en tête de file
		Defiler(f); //On suprimme cet élément de la file
		Transition t = a->tabListeTrans[e] ; //On récupère toutes les transitions de l'état
		//Et on fait le même parcours que pour l'état initial

		//Tant qu'on a une transition partant de cet état :
		while(!estTransitionVide(t)){
			
			char lettre_t = getLettre(t) ; //On récupère la lettre correspondant à la transition
			//On récupère l'état pour chaque transition, et on l'ajoute dans la file
			child = getDestination(t) ;
			Enfiler(f,child) ;
			t = TgetSuivant(t);

			//On prend le suppléant de l'état actuel 
			s = a->tabSuppleants[e] ;
			//Et on remonte les suppléances jusqu'à trouver une transition de la même lettre qui part de cet état
			while(EstDefinieTransition(a->tabListeTrans[s], lettre_t) == 0){
				s = a->tabSuppleants[s] ;
			}

			r = getDestination(Cible(a->tabListeTrans[s],lettre_t)) ; //On récupère l'état pointé par la transition
			ajout_suppleant(a->tabSuppleants, child,r); //Et on l'ajoute comme suppléant
			//Si cet état est terminal, on set l'état child comme étant terminal
			if(sortie(a->EstTerminal,r) == 1){
				mettreTerminal(a->EstTerminal,child) ;
			}

		}

	}
	FreeFile(f) ; //Ensuite on libère la file
	return a ;
}


//fonction de prétraitement qui consiste à créer l'automate
Automate pre_ac(Automate a,char** liste_mots,int nombre_mots){
	
	//On récupère son état initial (fixé à 0)
	etat e = a->EtatInitial ;
	//On parcours les mots
	for(int i = 0; i < nombre_mots;i++){
		//printf("Ajout du mot : %s dans l'automate\n",liste_mots[i]) ;
		a = ENTRER(a,liste_mots[i]);
	}
	//Pour chaque lettre de l'alphabet qui n'est pas utilisé par une transition partant de l'état initial
	//On créer une transition de l'état initial vers l'état initial
	for (int i = 0; i < a->tailleAlpha; i++) {
		char lettre = a->alphabet[i] ;
		if(!EstDefinieTransition(a->tabListeTrans[e],lettre)){
			ajout_transition(a->tabListeTrans, e,e, lettre) ;
		}
	}
	//On complète ensuite l'automate par la fonction de suppléance
	a = COMPLETER(a) ;
	return a ;
}

//Fonction qui pour un texte donné, parcours ce texte une fois et détecte les occurences d'une suite de motif donnés
void Aho_Corasick(Automate a,char** liste_mots,int nombre_mots,char* nom_fichier){
	//Variables
	char lettre;
	int occurences = 0;
	//PRE-TRAITEMENT
	a = pre_ac(a,liste_mots,nombre_mots);
	//TEXTE OU TROUVER LES OCCURENCES
	char* texte = GetTexte(nom_fichier);
	int taille_texte = strlen(texte);
	//DEBUT DE L'ALGORITHME
	etat e = a->EtatInitial ; // On part de l'état initial de l'automate
	//Pour chaque lettre en position j du texte
	for(int j=0;j<taille_texte;j++){
		lettre = texte[j] ;
		if(EstDansAlphabet(a->alphabet,&(a->tailleAlpha),lettre) == 0) //Si la lettre n'est pas dans l'alphabet de l'automate, on passe à l'itération suivante
			continue;
		//Tant qu'on ne part pas de l'état initial, et qu'il n'y a pas de transition pour la lettre :
		while(e != a->EtatInitial && EstDefinieTransition(a->tabListeTrans[e], lettre) == 0){
			e = a->tabSuppleants[e] ; //On récupère le suppléant de l'état e 
		}
		//On récupère sa destination
		e = getDestination(Cible(a->tabListeTrans[e], lettre)) ;
		//Si cet état est terminal, on déclare une occurence
		if(sortie(a->EstTerminal,e) == 1) {
			occurences++;
			//printf("occurence détéctée dans le texte en position %d\n",j+1) ;
		}
	}
	printf("%d occurences détéctées dans le texte\n",occurences) ;
}


