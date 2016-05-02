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

int main(int argc, char** argv) {
	if(argc<3) {
		printf("Usage : %s nombre_caractères motif1 [motif2] [motif3] ...\n",argv[0]) ;
		exit(1) ;
	}	
	
	srand(time(NULL));

	char car[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' ', ' ', ' ', ' ', ' ', ' '};
	//Nombres de caractères
	int size = atoi(argv[1]) ; 
	int random ; //entier qui va servir à stocker le nombre aléatoire
	int taille_du_mot ; //entier qui va stocker la taille du mot
	int taille_totale =0;
	//On récupère le nombre de mots que l'utilisateur a saisi
	int nombre_mots = argc-2 ;
	//On alloue un tableau de chaines de caractères qui contiendra tous les mots
	char** liste_mots = (char**) malloc(sizeof(char*) * nombre_mots) ;
	//On enregistre chaque mot dans le tableau
	for(int i=1;i<argc;i++){
		taille_du_mot = strlen(argv[i]) ;
		liste_mots[i-2] = (char*) malloc(sizeof(char)*(1+taille_du_mot)) ;
		liste_mots[i-2] = argv[i] ;
		taille_totale += taille_du_mot ;
	}
	
	if(taille_totale > size) {
		fprintf(stderr, "le nombre de caractères total des mots ne doit pas dépasser le nombre total fixé\n") ;
		exit(EXIT_FAILURE) ;
	}

	int* position_mot = (int*) malloc(sizeof(int)*nombre_mots) ;

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

	TriMax(position_mot,nombre_mots);

	int j = 0 ;
	int k = 0;
	int posAl ;
	while(j<size){
		if(j == position_mot[k]){
			printf("%s", liste_mots[k]);
			j += strlen(liste_mots[k]);
			if (k < nombre_mots)
				k++;
		} else {
			posAl = (int) rand()%32 ;
			printf("%c",car[posAl]);
			j++;
		}

	}
}


