#ifndef DEF_BALL
#define DEF_BALL

#include <math.h>

#include <GL/gl.h>
#include <SDL/SDL_image.h>

#include "position.h"
#include "vector.h"
#include "texture.h"

/* problème avec la constante M_PI de la bibliothèque math.h */
#define M_PI 3.14



typedef struct Ball_s {
	Position center;
	Vector speed;
	float speedValue;
	float radius;
	/* for the texture */
	Texture texture;
} Ball;



/********** Prototypes functions **********/

/* to draw a ball (canonical) */
void drawingBall();
/* to create a ball */
Ball BallCreation(Position c, Vector s, float sv, float radius, const char * nameImage);



#endif
