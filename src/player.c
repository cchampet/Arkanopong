#include "player.h"



void drawingPlayerNbLife(Player p) {
	int tmpNbLife = p.nbLife;
	while (tmpNbLife > 0) {
		glBegin(GL_QUADS);
		glTexCoord2f(0., 0.);
		glVertex2f(1., 0.);
		glTexCoord2f(0., 1.);
		glVertex2f(1., -1.);
		glTexCoord2f(1., 1.);
		glVertex2f(0., -1.);
		glTexCoord2f(1., 0.);
		glVertex2f(0., 0.);
		glEnd();
		glTranslatef(-1., 0., 0.);
		tmpNbLife--;
	}
}



Player PlayerCreation(char * name, int nbLife, Ball * ball, Bar * bar, const char * nameImage) {
	Player p;
	p.name = name;
	p.nbLife = nbLife;
	p.ball = ball;
	p.bar = bar;
	p.texture = TextureCreation(&p.texture, nameImage);

	return p;
}
