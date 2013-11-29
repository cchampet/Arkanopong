#include "ball.h"



void drawingBall() {
	glBegin(GL_POLYGON);
	int nbDivisionCircle = 100;
	int i;
	float x, y;
	for(i=0; i < nbDivisionCircle; i++) {
		x = cos(i * 2 * M_PI/nbDivisionCircle);
		y = sin(i * 2 * M_PI/nbDivisionCircle);
		glTexCoord2f(x, y);
		glVertex2f(x, y);
	}
	glEnd();
}



Ball BallCreation(Position c, Vector s, float sv, float radius, const char * nameImage) {
	Ball ball;
	ball.center = c;
	ball.speed = s;
	ball.speedValue = sv;
	ball.radius = radius;
	ball.texture = TextureCreation(&ball.texture, nameImage);

	return ball;
}
