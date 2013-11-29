#ifndef DEF_BAR
#define DEF_BAR

#include <GL/gl.h>

#include "position.h"



typedef struct Bar_s {
	Position position;
	float width;
	float height;
} Bar;



/********** Prototypes functions **********/

/* draw a bar (canonical) */
void drawingBar();
/* to create a bar */
Bar BarCreation(Position p, float width, float height);



#endif
