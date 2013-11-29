#ifndef DEF_TEXTURE
#define DEF_TEXTURE

#include <GL/gl.h>
#include <SDL/SDL_image.h>



typedef struct Texture_s {
	const char * nameImage;
	GLuint textureId;
	SDL_Surface * img;
} Texture;



/********** Prototypes functions **********/
/* to create a texture */
Texture TextureCreation(Texture * t, const char * nameImage);
/* draw the Wallpaper, and the Game Over */
void drawingWallPaper(float SCALE_X, float SCALE_Y);


#endif
