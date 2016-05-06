#include <stdio.h>
#include <stdlib.h>

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
