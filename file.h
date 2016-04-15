struct _liste {
	int element;
	struct _liste * suivant;
};

typedef struct _liste * Liste;

struct _file {
	int longueur ;
	struct _liste * tete;
	struct _liste * queue;
};

typedef struct _file * File;