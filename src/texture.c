#include "texture.h"



Texture TextureCreation(Texture * t, const char * nameImage) {
	t->nameImage = nameImage;

	/* load image */
	t->img = IMG_Load(t->nameImage);
	if (t->img == NULL) {
		fprintf(stderr, "Bad alloc for the image.\n");
		exit(EXIT_FAILURE);
	}

	/* ask space memory to graphics card in order to put the texture */
	glGenTextures(1, &(t->textureId));
	/* bind the texture  */
	glBindTexture(GL_TEXTURE_2D, t->textureId);
	/* ask linear filter */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLenum format;
	switch(t->img->format->BytesPerPixel) {
	case 1 : 
		format = GL_RED;
		break;
	case 3: 
		format = GL_RGB;
		break;
	case 4 : 
		format = GL_RGBA;
		break;
	default : 
		fprintf(stderr, "Error in the colors of the image.\n");
		exit(EXIT_FAILURE);
	}

	/* send data to openGL */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t->img->w, t->img->h, 0, format, GL_UNSIGNED_BYTE, t->img->pixels);
	/* debind the texture */
	glBindTexture(GL_TEXTURE_2D,0);

	return *t;
}


void drawingWallPaper(float SCALE_X, float SCALE_Y) {
	glBegin(GL_QUADS);
	glTexCoord2f(1., 0.);
	glVertex2f(SCALE_X, SCALE_Y);
	glTexCoord2f(1., 1.);
	glVertex2f(SCALE_X, -SCALE_Y);
	glTexCoord2f(0., 1.);
	glVertex2f(-SCALE_X, -SCALE_Y);
	glTexCoord2f(0., 0.);
	glVertex2f(-SCALE_X, SCALE_Y);
	glEnd();
}
