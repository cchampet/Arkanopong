#ifndef DEF_PLAYER
#define DEF_PLAYER

#include <GL/gl.h>
#include <SDL/SDL_image.h>

#include "ball.h"
#include "bar.h"
#include "texture.h"



typedef struct Player_s {
	char * name;
	int nbLife;
	Ball * ball;
	Bar * bar;
	/* for the texture of the heart */
	Texture texture;
} Player;



/********** Prototypes functions **********/
/* to draw the heart of a player (canonical) */
void drawingPlayerNbLife(Player p);
/* to create a player */
Player PlayerCreation(char * name, int nbLife, Ball * ball, Bar * bar, const char * nameImage);



#endif
