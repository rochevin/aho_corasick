#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ac.h"
#include "file.h"


int main(int argc, char** argv) {

	int nombre_mots ;
	char* alphabet = NULL;
	int taille_alphabet = 0;
	//int nombre_etats = 1; //Pour l'instant on sait qu'on a au moins l'état initial

	if(argc<3) {
		printf("Recherche de motif dans le fichier %s\n",argv[1]) ;
		printf("Usage : %s fic.txt motif1 [motif2] [motif3] ...\n",argv[0]) ;
		exit(1) ;
	}	
	
	//On récupère le nombre de mots que l'utilisateur a saisi
	nombre_mots = argc-2 ;
	//On alloue un tableau de chaines de caractères qui contiendra tous les mots
	char** liste_mots = (char**) malloc(sizeof(char*) * nombre_mots) ;
	//On enregistre chaque mot dans le tableau
	//Et on en profite pour déterminer le nombre d'états
	for(int i=2;i<argc;i++){
		int taille_mot = strlen(argv[i]) ;
		liste_mots[i-2] = (char*) malloc(sizeof(char)*(1+taille_mot)) ;
		liste_mots[i-2] = argv[i] ;
		//nombre_etats = nombre_etats + taille_mot;
	}
	//On créer ensuite l'alphabet
	for(int i=0;i<nombre_mots;i++) {
		AjouterMot(alphabet,&taille_alphabet,liste_mots[i]);
	}
	//AjouterLettre(alphabet,&taille_alphabet,'/0') ;

	//Ensuite on initialise l'automate
	Automate a = init_automate(alphabet,taille_alphabet) ;


	return EXIT_SUCCESS ;
}

