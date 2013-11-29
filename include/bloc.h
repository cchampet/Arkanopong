#ifndef DEF_BLOC
#define DEF_BLOC

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include <SDL/SDL_image.h>

#include "position.h"
#include "texture.h"


enum typeBloc {
	CLASSIQUE, 
	BONUS_BAR_GROW_UP, 
	BONUS_BAR_NARROW, 
	BONUS_LIFE_ADD, 
	BONUS_LIFE_MINUS, 
	BONUS_FASTER, 
	BONUS_SLOWER, 
	INDESTRUCTIBLE,
	EMPTY
	};

typedef enum typeBloc typeBloc;



typedef struct Bloc_s {
	Position position;
	typeBloc type;
	float size;
	Texture texture;
} Bloc;



typedef struct nodeBloc_s {
	Bloc * data;
	struct nodeBloc_s * next;
	struct nodeBloc_s * previous;
} NodeBloc, *ListBlocs;




/********** Prototypes functions **********/

/* draw a bloc (canonical) */
void drawingBloc();
/* to alloc a bloc */
Bloc * AllocBloc(Position p, typeBloc type, float size, const char * nameImage);

/* to create a node, element of the list of blocs */
NodeBloc * OneBlocInTheList(Bloc * bloc);
/* add a bloc to a list of bloc */
void AddBlocToList(Bloc * bloc, ListBlocs * list);
/* init the list of blocs for the game, help by a file (in ressources directory, "gridBlocs.txt") */
void ListBlocsCreation(char * filepath, ListBlocs * list, int theme);
/* delete one specific bloc in the list */
ListBlocs DeleteOneBlocInTheList(ListBlocs * list);
/* delete the blocs of the list */
void DeleteListBlocs(ListBlocs * list);



#endif
