#include "bar.h"



void drawingBar() {
	glBegin(GL_QUADS);
	glVertex2f(1., 0.);
	glVertex2f(1., -1.);
	glVertex2f(0., -1.);
	glVertex2f(0., 0.);
	glEnd();
}



Bar BarCreation(Position p, float width, float height) {
	Bar bar;
	bar.position = p;
	bar.width = width;
	bar.height = height;
	return bar;
}
