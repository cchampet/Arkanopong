#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "collides.h"
#include "texture.h"
#include "ball.h"
#include "bar.h"
#include "player.h"
#include "bloc.h"



static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

static const float SCALE_X = 16.;
static const float SCALE_Y = 12.;



/* to resize the window (+ 1 call at the beginning of the program) */
void reshape() {
	/* pixels concern by openGL */
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* we work in 2D */
	gluOrtho2D(-SCALE_X, SCALE_X, -SCALE_Y, SCALE_Y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



void setVideoMode() {
	/* open a window and creation of an OpenGL context */
	if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
		fprintf(stderr, "Impossible to open the window. Close the program.\n");
		exit(EXIT_FAILURE);
	}
	reshape();
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapBuffers();
}

void initScene(Player * p1, Player * p2) {
	/* position of the balls and the bars */
	p1->ball->center = PositionXY(0., -SCALE_Y + 0.6);
	p1->bar->position = PositionXY(-1.5, -SCALE_Y + 0.2);
	p1->bar->width = 3.;
	p1->nbLife = 3;
	p1->ball->speed = VectorXY(0., 0.);
				
	p2->ball->center = PositionXY(0., SCALE_Y - 0.6);
	p2->bar->position = PositionXY(-1.5, SCALE_Y);
	p2->bar->width = 3.;
	p2->nbLife = 3;
	p2->ball->speed = VectorXY(0., 0.);
}

int main(int argc, char** argv) {

	/* init SDL */
	if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Impossible to initialise the SDL. Close the program.\n");
		return EXIT_FAILURE;
	}
   
	setVideoMode();
	/* title of the window */
	SDL_WM_SetCaption("megA-rkanpong", NULL);

	/********** data of the game **********/
	/* the ball for player 1 */
	Ball p1_ball = BallCreation(PositionXY(0., -SCALE_Y + 0.6), VectorXY(0.,0.), 0.15, 0.3, "ressources/texture_ball_p1.png");
	
	/* a bar for player 1 */
	Bar p1_bar = BarCreation(PositionXY(-1.5, -SCALE_Y + 0.2), 3., 0.2);
	/* to move the bar */
	int p1_keyPress = 0;
	int p1_goLeft = 0;
	int p1_goRight = 0;

	/* player 1 */
	Player p1 = PlayerCreation("Clement", 3, &p1_ball, &p1_bar, "ressources/nbLife_player.jpg");

	/* the ball for player 2 */
	Ball p2_ball = BallCreation(PositionXY(0.,SCALE_Y - 0.6), VectorXY(0.,0.), -0.15, 0.3, "ressources/texture_ball_p2.jpg");
	
	/* a bar for player 2 */
	Bar p2_bar = BarCreation(PositionXY(-1.5, SCALE_Y), 3., 0.2);
	/* to move the bar */
	int p2_keyPress = 0;
	int p2_goLeft = 0;
	int p2_goRight = 0;

	/* player 2 */
	Player p2 = PlayerCreation("Flavien", 3, &p2_ball, &p2_bar, "ressources/nbLife_player.jpg");
	
	/* List of blocs */
	ListBlocs listBlocs = (ListBlocs)malloc(sizeof(NodeBloc));
	ListBlocsCreation("ressources/gridBambou.txt", &listBlocs, 1);
	ListBlocs * DisplayListBlocs = &listBlocs;
	ListBlocs IntersectionListBlocs = listBlocs;

	/* Wallpaper */
	Texture textureWallpaper;
	textureWallpaper = TextureCreation(&textureWallpaper, "ressources/fond_bambou.jpg");

	/* Game Over */
	/* when player 1 win */
	Texture textureGameOverPlayerONEWin;
	textureGameOverPlayerONEWin = TextureCreation(&textureGameOverPlayerONEWin, "ressources/player_one_win.jpg");
	
	/* when player 2 win */
	Texture textureGameOverPlayerTWOWin;
	textureGameOverPlayerTWOWin = TextureCreation(&textureGameOverPlayerTWOWin, "ressources/player_two_win.jpg");
	
	int gameOver = 0;
	
	/* Menu Principal */
	Texture textureMenuPrincipal;
	textureMenuPrincipal = TextureCreation(&textureMenuPrincipal, "ressources/menu_principal.jpg");
	int menuPrincipal = 1;
	
	/* Menu secondaire */
	Texture textureMenuSecondaire_off;	
	textureMenuSecondaire_off = TextureCreation(&textureMenuSecondaire_off, "ressources/secondMenu_off.jpg");
	Texture textureMenuSecondaire_resume;	
	textureMenuSecondaire_resume = TextureCreation(&textureMenuSecondaire_resume, "ressources/secondMenu_resume.jpg");
	Texture textureMenuSecondaire_GoToMainMenu;	
	textureMenuSecondaire_GoToMainMenu = TextureCreation(&textureMenuSecondaire_GoToMainMenu, "ressources/secondMenu_gotomenu.jpg");
	int menuSecondaire = 0;
	int menuSecondaireChoix = 0;
	
	/* ---- Texture univers "bambou" ---- */
	Texture textureUniversBambou;
	textureUniversBambou = TextureCreation(&textureUniversBambou, "ressources/bouton_bambou.jpg");
	
	/* ---- Texture univers "space" ---- */
	Texture textureUniversSpace;
	textureUniversSpace = TextureCreation(&textureUniversSpace, "ressources/bouton_space.jpg");
	
	int selectUnivers = 0;

	/********** display loop **********/
	int loop = 1;
	while(loop) {
	if (p1.ball->speed.x != 0. ) {
		printf("vitesse de p1 en x : %f\n", p1.ball->speed.x);
		printf("vitesse de p1 en y : %f\n", p1.ball->speed.y);
	}
		Uint32 startTime = SDL_GetTicks();

		/********** draw **********/
		glClear(GL_COLOR_BUFFER_BIT);

		if(gameOver == 0 && menuPrincipal == 0 && menuSecondaire == 0) {
		
			/* On charge l'univers sélectionné */
			if(selectUnivers == 1) {
				/* Univers 1 */
				ListBlocsCreation("ressources/gridBambou.txt", &listBlocs, 1);
				DisplayListBlocs = &listBlocs;
				IntersectionListBlocs = listBlocs;
				printf("On charge l'univers 1\n");
				
				textureWallpaper = TextureCreation(&textureWallpaper, "ressources/fond_bambou.jpg");
				initScene(&p1, &p2);
				
				selectUnivers = 0;
			}
			else if(selectUnivers == 2) { 
				/* Univers 2 */
				ListBlocsCreation("ressources/gridSpace.txt", &listBlocs, 2);
				DisplayListBlocs = &listBlocs;
				IntersectionListBlocs = listBlocs;
				printf("On charge l'univers 2\n");
				
				textureWallpaper = TextureCreation(&textureWallpaper, "ressources/fond_espace.jpg");
				initScene(&p1, &p2);
				
				selectUnivers = 0;
			}
			
			/* wallpaper */		
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureWallpaper.textureId);
			drawingWallPaper(SCALE_X, SCALE_Y);
			glBindTexture(GL_TEXTURE_2D,0);
					
			/* heart of player 1 */
			glBindTexture(GL_TEXTURE_2D, p1.texture.textureId);
			glTranslatef(SCALE_X - 1., -SCALE_Y + 2, 0.);
			drawingPlayerNbLife(p1);
			glLoadIdentity();
			glBindTexture(GL_TEXTURE_2D,0);
	
			/* heart of player 2 */
			glBindTexture(GL_TEXTURE_2D, p2.texture.textureId);
			glTranslatef(SCALE_X - 1., SCALE_Y - 1, 0.);
			drawingPlayerNbLife(p2);
			glLoadIdentity();
			glBindTexture(GL_TEXTURE_2D,0);

			/* the ball for player 1 */
			glBindTexture(GL_TEXTURE_2D, p1.ball->texture.textureId);
			glTranslatef(p1.ball->center.x, p1.ball->center.y, 0.);
			glScalef(p1.ball->radius, p1.ball->radius, 1.);
			drawingBall();
			glLoadIdentity();
			glBindTexture(GL_TEXTURE_2D,0);

			/* the ball for player 2 */
			glBindTexture(GL_TEXTURE_2D, p2.ball->texture.textureId);
			glTranslatef(p2.ball->center.x, p2.ball->center.y, 0.);
			glScalef(p2.ball->radius, p2.ball->radius, 1.);
			drawingBall();
			glLoadIdentity();
			glBindTexture(GL_TEXTURE_2D,0);

			/* the blocs */
			while ((*DisplayListBlocs) != NULL) {
				glBindTexture(GL_TEXTURE_2D, (*DisplayListBlocs)->data->texture.textureId);
				glTranslatef((*DisplayListBlocs)->data->position.x, (*DisplayListBlocs)->data->position.y, 0.);
				glScalef((*DisplayListBlocs)->data->size, (*DisplayListBlocs)->data->size, 1.);
				drawingBloc();
				glLoadIdentity();
				glBindTexture(GL_TEXTURE_2D,0);

				DisplayListBlocs = &((*DisplayListBlocs)->next);
			}
			glDisable(GL_TEXTURE_2D);

			/* the bar of player 1 */
			glColor3ub(255, 255, 255);
			glTranslatef(p1.bar->position.x, p1.bar->position.y, 0.);			
			glScalef(p1.bar->width, p1.bar->height, 1.);
			drawingBar();
			glLoadIdentity();

			/* the bar of player 2 */
			glColor3ub(255, 255, 255);
			glTranslatef(p2.bar->position.x, p2.bar->position.y, 0.);			
			glScalef(p2.bar->width, p2.bar->height, 1.);
			drawingBar();
			glLoadIdentity();
		}
		
		else if (menuSecondaire == 1) {
			glEnable(GL_TEXTURE_2D);
			
			if (menuSecondaireChoix == 1) {
				glBindTexture(GL_TEXTURE_2D, textureMenuSecondaire_resume.textureId);
			}
			else if (menuSecondaireChoix == 2) {
				glBindTexture(GL_TEXTURE_2D, textureMenuSecondaire_GoToMainMenu.textureId);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, textureMenuSecondaire_off.textureId);
			}
			
			drawingWallPaper(SCALE_X, SCALE_Y);
			glBindTexture(GL_TEXTURE_2D,0);
			glDisable(GL_TEXTURE_2D);
		}
		
		else if (gameOver == 1 && menuPrincipal == 0) {
			/* Game Over */
			/* speed of the balls at 0 (in order to stop the game) */
			p1.ball->speed = VectorXY(0.,0.);			
			p2.ball->speed = VectorXY(0.,0.);
			
			glEnable(GL_TEXTURE_2D);
			if (p1.nbLife == 0) {
				glBindTexture(GL_TEXTURE_2D, textureGameOverPlayerTWOWin.textureId);
			}
			else if (p2.nbLife == 0) {
				glBindTexture(GL_TEXTURE_2D, textureGameOverPlayerONEWin.textureId);
			}
			drawingWallPaper(SCALE_X, SCALE_Y);
			glBindTexture(GL_TEXTURE_2D,0);
			glDisable(GL_TEXTURE_2D);
		}
		
		else {
			/* Menu principal */
			glEnable(GL_TEXTURE_2D);
			if (selectUnivers == 1) {
				glBindTexture(GL_TEXTURE_2D, textureUniversBambou.textureId);
			}
			else if (selectUnivers == 2) {
				glBindTexture(GL_TEXTURE_2D, textureUniversSpace.textureId);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, textureMenuPrincipal.textureId);
			}
			drawingWallPaper(SCALE_X, SCALE_Y);
			glBindTexture(GL_TEXTURE_2D,0);
			glDisable(GL_TEXTURE_2D);
			
		}
		
		glEnd();

		SDL_GL_SwapBuffers();

		/********** gestion event **********/
  	SDL_Event e;
  	while(SDL_PollEvent(&e)) {
    	if(e.type == SDL_QUIT) {
				loop = 0;
				break;
    	}
    
			switch(e.type) {
				/* RESIZE WINDOW */
				case SDL_VIDEORESIZE:
					WINDOW_WIDTH = e.resize.w;
					WINDOW_HEIGHT = e.resize.h;
					setVideoMode();
					break;

				/* KEY DOWN */
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym){
						case SDLK_a : 
							if (menuSecondaire == 0){
								p2_keyPress = 1;
								p2_goRight = 0;
								p2_goLeft = 1;
							}
							break;
						case SDLK_z :
							if (menuSecondaire == 0){
								p2_keyPress = 1;
								p2_goLeft = 0;
								p2_goRight = 1;
							}
							break;
						case SDLK_LEFT :
							if (menuPrincipal == 1) {
								selectUnivers = 1;	
							}
							else {
								if (menuSecondaire == 0){
									p1_keyPress = 1;
									p1_goRight = 0;
									p1_goLeft = 1;
								}
							}
							break;
						case SDLK_RIGHT :
							if (menuPrincipal == 1) {
								selectUnivers = 2;	
							}
							else {
								if (menuSecondaire == 0){
									p1_keyPress = 1;
									p1_goLeft = 0;
									p1_goRight = 1;
								}
							}
							break;
						/* controls during the secondary menu */
						case SDLK_UP :
							if (menuSecondaire == 1) {
								menuSecondaireChoix = 1;
							}
							break;
						case SDLK_DOWN :
							if (menuSecondaire == 1) {
								menuSecondaireChoix = 2;
							}
							break;
						/* press "enter" to select your choice */
						case SDLK_RETURN :
							if (menuPrincipal == 1) {
								menuPrincipal = 0;
							}
							if (menuSecondaire == 1) {
								/* you need to go back to the main menu */
								if (menuSecondaireChoix == 2) {
									menuPrincipal = 1;
								}
								menuSecondaire = 0;
							}
							break;
						/* launch the ball of the player 1 */
						case SDLK_SPACE : 
							if (immobile(p1.ball->speed)) {
								p1.ball->speed = VectorXY(0., p1.ball->speedValue);
							}
							break;						
						/* launch the ball of the player 2 */
						case SDLK_r : 
							if (immobile(p2.ball->speed)) {
								p2.ball->speed = VectorXY(0., p2.ball->speedValue);
							}
							break;
						/* quit the application */
						case SDLK_ESCAPE : 
							/*loop = 0;
							break;*/
							if (gameOver == 1) {
								gameOver = 0;
								menuPrincipal = 1;
								initScene(&p1, &p2);
							}
							else {
								if (menuSecondaire == 0){								
									menuSecondaire = 1;
								}
								else {
									menuSecondaire = 0;
								}
							}
							break;
						default : break;
				  }
				break;

				/* KEY UP */
				case SDL_KEYUP :
					switch(e.key.keysym.sym){
						case SDLK_a : 
						case SDLK_z :
							p2_keyPress = 0;
							p2_goRight = 0;
							p2_goLeft = 0;
							break;
						case SDLK_LEFT :
						case SDLK_RIGHT :
							p1_keyPress = 0;
							p1_goRight = 0;
							p1_goLeft = 0;
							break;
						default : break;
					}
				break;
				  
				default:
					break;
			}
		}

		/********** idle **********/
		/********** movement of the ball for player 1 **********/
		if (menuSecondaire == 0) {
			p1.ball->center.y += p1.ball->speed.y;
			p1.ball->center.x += p1.ball->speed.x;
		}
		if (p1.ball->center.y > SCALE_Y) {
			/* player 2 lose a life */
			p1.ball->center = PositionXY(p1.bar->position.x + p1.bar->width/2., -SCALE_Y + 0.6);
			p1.ball->speed = VectorXY(0., 0.);
			p2.nbLife--;
		}
		if (p1.ball->center.y < -SCALE_Y) {
			/* player 1 lose a life */
			p1.ball->center = PositionXY(p1.bar->position.x + p1.bar->width/2., -SCALE_Y + 0.6);
			p1.ball->speed = VectorXY(0., 0.);
			p1.nbLife--;
		}
		/* collides between the limits of the window (left and right) */
		if (p1.ball->center.x >= SCALE_X) {
			p1.ball->speed.x = - p1.ball->speed.x;
		}
		if (p1.ball->center.x <= -SCALE_X) {
			p1.ball->speed.x = - p1.ball->speed.x;
		}
		/* if the ball is immobile */
		if (immobile(p1.ball->speed)) {
			p1.ball->center = PositionXY(p1.bar->position.x + p1.bar->width/2., -SCALE_Y + 0.6);
		}

		/********** movement of the ball for player 2 **********/
		if (menuSecondaire == 0) {
			p2.ball->center.y += p2.ball->speed.y;
			p2.ball->center.x += p2.ball->speed.x;
		}
		if (p2.ball->center.y > SCALE_Y) {
			/* player 2 lose a life */
			p2.ball->center = PositionXY(p2.bar->position.x + p2.bar->width/2., SCALE_Y - 0.6);
			p2.ball->speed = VectorXY(0., 0.);
			p2.nbLife--;
		}
		if (p2.ball->center.y < -SCALE_Y) {
			/* player 1 lose a life */
			p2.ball->center = PositionXY(p2.bar->position.x + p2.bar->width/2., SCALE_Y - 0.6);
			p2.ball->speed = VectorXY(0., 0.);
			p1.nbLife--;
		}
		/* collides between the limits of the window (left and right) */
		if (p2.ball->center.x >= SCALE_X) {
			p2.ball->speed.x = - p2.ball->speed.x;
		}
		if (p2.ball->center.x <= -SCALE_X) {	
			p2.ball->speed.x = - p2.ball->speed.x;
		}
		/* if the ball is immobile */
		if (immobile(p2.ball->speed)) {
			p2.ball->center = PositionXY(p2.bar->position.x + p2.bar->width/2., SCALE_Y - 0.6);
		}

		/********** movement of the bar for player 1 **********/
		if (p1_keyPress) {
			if (p1_goLeft && p1_bar.position.x > -SCALE_X) {
				p1.bar->position.x -= 0.4;
			}
			else if (p1_goRight && p1_bar.position.x < (SCALE_X - p1_bar.width)) {
				p1.bar->position.x += 0.4;
			}
		}

		/********** movement of the bar for player 2 **********/
		if (p2_keyPress) {
			if (p2_goLeft && p2_bar.position.x > -SCALE_X) {
				p2.bar->position.x -= 0.4;
			}
			else if (p2_goRight && p2_bar.position.x < (SCALE_X - p2_bar.width)) {
				p2.bar->position.x += 0.4;
			}
		}


		/********** Intersections **********/
		/* with bar of the player 1, and his ball */
		if (IntersectionBallBar(*p1.ball, *p1.bar)) {
			p1.ball->speed = reboundBallBar(*p1.ball, *p1.bar);
		}

		/* with bar of the player 1, and the ball of player 2 */
		if (IntersectionBallBar(*p2.ball, *p1.bar)) {
			p2.ball->speed = reboundBallBar(*p2.ball, *p1.bar);
		}

		/* with bar of the player 2, and his ball */
		if (IntersectionBallBar(*p2.ball, *p2.bar)) {
			p2.ball->speed = reboundBallBar(*p2.ball, *p2.bar);
		}

		/* with bar of the player 2, and the ball of player 1 */
		if (IntersectionBallBar(*p1.ball, *p2.bar)) {
			p1.ball->speed = reboundBallBar(*p1.ball, *p2.bar);
		}

		/* between the 2 balls */
		if (IntersectionBallBall(*p1.ball, *p2.ball)) {
			p2.ball->speed.y = - p2.ball->speed.y;
			p1.ball->speed.y = - p1.ball->speed.y;
		}

		/* with the balls and the blocs */
		while (IntersectionListBlocs != NULL) {
			/* intersection between the ball of the player 2 and a bloc */
			if (IntersectionBallBloc(*p2.ball, *IntersectionListBlocs->data)) {
				/* change the direction of the ball of the player 2 */
				/* where is the intersection ? */
				if (IntersectionInTheSidesOfTheBloc(*IntersectionListBlocs->data, *p2.ball))
					p2.ball->speed.x = - p2.ball->speed.x;
				else
					p2.ball->speed.y = - p2.ball->speed.y;
				if (IntersectionListBlocs->data->type != INDESTRUCTIBLE) {
					/* if it's a special bloc */
					switch (IntersectionListBlocs->data->type) {
						case BONUS_BAR_GROW_UP :
							p2.bar->width += 0.5;
							break;
						case BONUS_BAR_NARROW :
							if (p2.bar->width >=1.) {
								p2.bar->width -= 0.5;
							}
							break;
						case BONUS_LIFE_ADD :
							p2.nbLife ++;
							break;
						case BONUS_LIFE_MINUS :
							if (p2.nbLife > 1) {
								p2.nbLife --; 
							}
							break;
						case BONUS_SLOWER :
							p2.ball->speed = decelerate(p2.ball->speed, 1.3);
							break;
						case BONUS_FASTER :
							p2.ball->speed = accelerate(p2.ball->speed, 1.3);
							break;
						default :
							break;
					}
					/* if it's the first element of the list */
					if (IntersectionListBlocs->previous == NULL) {
						listBlocs = DeleteOneBlocInTheList(&listBlocs);
						IntersectionListBlocs = listBlocs;
					}
					else 
						DeleteOneBlocInTheList(&IntersectionListBlocs);
				}
				break;
			}
			
			
			if (IntersectionListBlocs != NULL) {
				/* intersection between the ball of the player 1 and a bloc */
				if (IntersectionBallBloc(*p1.ball, *IntersectionListBlocs->data)) {
					/* change the direction of the ball of the player 1 */
					/* where is the intersection ? */
					if (IntersectionInTheSidesOfTheBloc(*IntersectionListBlocs->data, *p1.ball))
						p1.ball->speed.x = - p1.ball->speed.x;
					else
						p1.ball->speed.y = - p1.ball->speed.y;
					if (IntersectionListBlocs->data->type != INDESTRUCTIBLE) {
						/* if it's a special bloc */
						switch (IntersectionListBlocs->data->type) {
							case BONUS_BAR_GROW_UP :
								p1.bar->width += 0.5;
								break;
							case BONUS_BAR_NARROW :
								if (p1.bar->width >=1.) {
									p1.bar->width -= 0.5;
								}
								break;
							case BONUS_LIFE_ADD :
								p1.nbLife ++;
								break;
							case BONUS_LIFE_MINUS :
								if (p1.nbLife > 1) {
									p1.nbLife --;
								}
								break;
							case BONUS_SLOWER :
								p1.ball->speed = decelerate(p1.ball->speed, 1.3);
								break;
							case BONUS_FASTER :
								p1.ball->speed = accelerate(p1.ball->speed, 1.3);
								break;
							default :
								break;
						}
						/* if it's the first element of the list */
						if (IntersectionListBlocs->previous == NULL) {
							listBlocs = DeleteOneBlocInTheList(&listBlocs);
							IntersectionListBlocs = listBlocs;
						}
						else 
							DeleteOneBlocInTheList(&IntersectionListBlocs);
					}
					break;
				}
			}
			IntersectionListBlocs = IntersectionListBlocs->next;
		}
		IntersectionListBlocs = listBlocs;
		DisplayListBlocs = &listBlocs;

		/********** nbLife of the Player **********/
		if (p1.nbLife <= 0) {
			/*printf("Joueur 1 a perdu !\n");*/
			gameOver = 1;
		}
		if (p2.nbLife <= 0) {
			/*printf("Joueur 2 a perdu !\n");*/
			gameOver = 1;
		}

		Uint32 elapsedTime = SDL_GetTicks() - startTime;
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}
	}
  
	/********** Free the textures, and the list of blocs **********/
	/* the 2 balls */
	glDeleteTextures(1, &(p1.ball->texture.textureId));
	SDL_FreeSurface(p1.ball->texture.img);

	glDeleteTextures(1, &(p2.ball->texture.textureId));
	SDL_FreeSurface(p2.ball->texture.img);

	/* the 2 groups of hearts */
	glDeleteTextures(1, &(p1.texture.textureId));
	SDL_FreeSurface(p1.texture.img);

	glDeleteTextures(1, &(p2.texture.textureId));
	SDL_FreeSurface(p2.texture.img);

	/* the wallpaper during the game, and the Game Over */
	glDeleteTextures(1, &(textureWallpaper.textureId));
	SDL_FreeSurface(textureWallpaper.img);

	glDeleteTextures(1, &(textureGameOverPlayerONEWin.textureId));
	SDL_FreeSurface(textureGameOverPlayerONEWin.img);
	
	glDeleteTextures(1, &(textureGameOverPlayerTWOWin.textureId));
	SDL_FreeSurface(textureGameOverPlayerTWOWin.img);

	/* free the list of blocs */
	DeleteListBlocs(&listBlocs);

	SDL_Quit();

	return EXIT_SUCCESS;
}
