#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//Fonction d'échange nécessaire au tri max
void Echange(int* a,int* b){
	int tmp=*a;
	*a=*b;
	*b=tmp;
}

//Fonction pour trier le tableau de nombres aléatoires
void TriMax(int* t, int n) {
	int k=n-1;
	unsigned char trie=0;
	while(k>0 && !trie) {
		int p=0;
		int verif=0;
		for(int j=1;j<=k;j++) {
			if (t[j]>=t[p]) {
				p=j;
			}
		}
		if(verif==k){
			trie=1;
		}
		Echange(&t[p],&t[k]);
		k=k-1;
	}
}

//fonction principale de generation de texte
//Fonctionne de la manière suivante : ./programme fichier nombre_de_caractères motif1 mtofi2 ...
//Première étape est de générer des positions aléatoires dans le texte pour chacun des motifs
//Ensuite on insère des lettres aléatoires pour chaque position du texte
//Si une position correspond à la position décidée pour un motif, on insère le motif 
int main(int argc, char** argv) {
	if(argc<4) {
		printf("Usage : %s fichier nombre_caractères motif1 [motif2] [motif3] ...\n",argv[0]) ;
		exit(1) ;
	}	
	
	//Préparation de la fonction aléatoire
	srand(time(NULL));
	int random ; //entier qui va servir à stocker le nombre aléatoire
	//Alphabet pour générer le texte
	char car[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' ', ' ', ' ', ' ', ' ', ' '};
	//Nombres de caractères précisés par l'utilisateur
	int size = atoi(argv[2]) ; 
	
	int taille_du_mot ; //entier qui va stocker la taille du mot
	int taille_totale = 0 ; // variable qui va stocker le nombre de caractères pour chacun de nos motifs
	
	//On récupère le nombre de mots que l'utilisateur a saisi
	int nombre_mots = argc-3 ;
	//On alloue un tableau de chaines de caractères qui contiendra tous les mots
	char** liste_mots = (char**) malloc(sizeof(char*) * nombre_mots) ;
	//On enregistre chaque mot dans le tableau
	for(int i=3;i<argc;i++){
		taille_du_mot = strlen(argv[i]) ;
		liste_mots[i-3] = (char*) malloc(sizeof(char)*(1+taille_du_mot)) ;
		liste_mots[i-3] = argv[i] ;
		taille_totale += taille_du_mot ;
	}
	
	//Vérification que la taille totale des mots n'est pas plus grand que le nombre de caractères du texte demandé
	if(taille_totale > size) {
		fprintf(stderr, "le nombre de caractères total des mots ne doit pas dépasser le nombre total fixé\n") ;
		exit(EXIT_FAILURE) ;
	}

	//Tableau qui va stocker les positions ou les mots seront insérés
	int* position_mot = (int*) malloc(sizeof(int)*nombre_mots) ;

	//Génération des positions aléatoires pour les motifs
	for (int i = 0; i < nombre_mots; i++) {
		taille_du_mot = strlen(liste_mots[i]);
		random = (int) rand()%(size-taille_du_mot-1) ;
		if(i != 0){
			while(random > (position_mot[i-1] - taille_du_mot) && random < (position_mot[i-1]+strlen(liste_mots[i-1])) ){
				random = (int) rand()%size ;
			}
		}
		position_mot[i] = random;
	}

	//Tri du tableau dans l'ordre croissant
	TriMax(position_mot,nombre_mots);

	//Ouverture du fichier précisé en argument
	FILE* fichier ;
	if ((fichier = fopen(argv[1], "w")) == NULL) {
		fprintf(stderr, "Impossible d'ouvrir le fichier %s en écriture\n", argv[1]) ;
		exit(EXIT_FAILURE) ;
	}

	int j = 0 ;
	int k = 0;
	int posAl ;
	//Pour j prenant la valeur de chaque position du texte jusqu'à sa taille max
	while(j<size){
		//Si j correspond à la position du premier motif à insérer
		if(j == position_mot[k]){
			printf("Ajout du mot %s en position %d dans le texte\n",liste_mots[k],j+1);
			fprintf(fichier,"%s", liste_mots[k]);
			j += strlen(liste_mots[k]); //On incrémente j de la taille du motif
			if (k < nombre_mots)
				k++;
		} else { //Sinon on insère un caractère aléatoire de l'alphabet
			posAl = (int) rand()%32 ;
			fprintf(fichier,"%c",car[posAl]);
			j++;
		}

	}
	fclose(fichier) ; //Fermeture du fichier
}


