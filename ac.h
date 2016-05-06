#ifndef AC

#define AC
#include "automate.h"


//Fonctions Aho Corasick
Automate ENTRER(Automate,char*);
Automate COMPLETER(Automate);
Automate pre_ac(Automate,char**,int);
void Aho_Corasick(Automate,char**,int,char*);

#endif
