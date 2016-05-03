#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ac.h"



int main(int argc, char** argv) {

	if(argc<3) {
		printf("Recherche de motif dans le fichier %s\n",argv[1]) ;
		printf("Usage : %s fic.txt motif1 [motif2] [motif3] ...\n",argv[0]) ;
		exit(1) ;
	}	
	
	int nombre_mots ;
	char* alphabet = NULL;
	int taille_alphabet = 0;
	int etats_max = 1;

	//Partie qui va servir à calculer le temps d'execution des fonctions
	float temps ;
	clock_t t1,t2 ;

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
		etats_max = etats_max + taille_mot;
	}
	//On créer ensuite l'alphabet
	for(int i=0;i<nombre_mots;i++) {
		alphabet = AjouterMot(alphabet,&taille_alphabet,liste_mots[i]);
	}
	//On initialise l'automate
	Automate a = init_automate(alphabet,taille_alphabet,etats_max) ;
	t1 = clock();
	//Et on appelle la fonction Aho Corasick
	Aho_Corasick(a,liste_mots,nombre_mots,argv[1]) ;
	t2 = clock();
	//Calcule du temps d'execution
	temps = (float)(t2-t1)/CLOCKS_PER_SEC;
	fprintf(stderr, "MESSAGE : Temps d'éxecution = %f secondes\n", temps);
	return EXIT_SUCCESS ;
}

