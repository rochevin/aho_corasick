#ifndef AC

#define AC
#include "automate.h"

//Alphabet
int EstDansAlphabet(char*,int*,char);
char* AjouterLettre(char*,int*,char);
char* AjouterMot(char*,int*,char*);

//Fonctions Aho Corasick
Automate ENTRER(Automate,char*);
Automate COMPLETER(Automate);
Automate pre_ac(Automate,char**,int);
void Aho_Corasick(Automate,char**,int,char*);

//Affichage de l'automate
void PrintAutomate(Automate) ;
void printTerminal(Automate);
void printTransitions(Automate);
void printSuppleants(Automate);
#endif
