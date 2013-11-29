#ifndef DEF_COLLIDES
#define DEF_COLLIDES

#include <math.h>

#include "vector.h"
#include "bar.h"
#include "ball.h"
#include "bloc.h"



/********** Prototypes functions **********/

/********** Functions for the intersections of the game **********/
/* to test the collision between 2 squares (the ball is like a square in this function) */
int CollisionBetween2Box(Ball ball, Bar bar);
/* to test if the 4 sides of the bar intersect the ball */
int CollisionBetweenPointAndBall(float x, float y, Ball ball);
/* to do the ultimate test... */
int ProjectionSurSegment(float Cx, float Cy, float Ax, float Ay, float Bx, float By);
/* the intersection bewteen a ball and a bar.
	return 0 => no intersection
	return 1 => intersection 
*/
int IntersectionBallBar(Ball ball, Bar bar);

/* the intersection between the two ball */
int IntersectionBallBall(Ball ball1, Ball ball2);

/* to test the collision between 2 squares (the ball is like a square in this function) */
int CollisionBetweenBallBloc(Ball ball, Bloc bloc);
/* the intersection bewteen a ball and a bloc.
	return 0 => no intersection
	return 1 => intersection 
*/
int IntersectionBallBloc(Ball ball, Bloc bloc);


/********** Functions for the rebounds between the balls and the bars **********/
/* return the new vector of the speed of a ball */
Vector reboundBallBar(Ball ball, Bar bar);
/* normalize a speedValue (we don't want an acceleration !) */
float normalize(float value, Bar bar, Ball ball);


/********** Functions in order to know where is the intersection in the bloc **********/
/* return 1 if it's the right or the left of the bloc, 0 if not */
int IntersectionInTheSidesOfTheBloc(Bloc bloc, Ball ball);

#endif
