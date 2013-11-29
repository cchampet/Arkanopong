#include "collides.h"



int CollisionBetween2Box(Ball ball, Bar bar) {
	if(((bar.position.x) >= (ball.center.x + ball.radius))     							/* the bar is to the right */
		|| ((bar.position.x + bar.width) <= (ball.center.x - ball.radius))    /* the bar is to the left */
		|| ((bar.position.y - bar.height) >= (ball.center.y + ball.radius))   		/* the bar is above */
		|| ((bar.position.y) <= (ball.center.y - ball.radius)))  							/* the bar is below */
		return 0; 
	else
		return 1;
}



int CollisionBetweenPointAndBall(float x, float y, Ball ball) {
	int d2 = (x - ball.center.x) * (x - ball.center.x) + (y - ball.center.y) * (y - ball.center.y);
	if (d2 > ball.radius * ball.radius)
		return 0;
	else
		return 1;
}



int ProjectionSurSegment(float Cx, float Cy, float Ax, float Ay, float Bx, float By) {
   float ACx = Cx-Ax;
   float ACy = Cy-Ay; 
   float ABx = Bx-Ax;
   float ABy = By-Ay; 
   float BCx = Cx-Bx;
   float BCy = Cy-By; 
   int s1 = (int)(ACx*ABx) + (ACy*ABy);
   int s2 = (int)(BCx*ABx) + (BCy*ABy); 
   if (s1*s2>0)
     return 0;
   return 1;
}



int IntersectionBallBar(Ball ball, Bar bar) {
	/* first test : if the ball and the bar are too far each other */
	if (CollisionBetween2Box(ball, bar) == 0)
		return 0;   

	/* second test */
	if (CollisionBetweenPointAndBall(bar.position.x, bar.position.y, ball) == 1
		|| CollisionBetweenPointAndBall(bar.position.x, bar.position.y - bar.height, ball) == 1
		|| CollisionBetweenPointAndBall(bar.position.x + bar.width, bar.position.y, ball) == 1
		|| CollisionBetweenPointAndBall(bar.position.x + bar.width, bar.position.y - bar.height, ball) == 1)
		return 1;   
	
	/* test for release the doubt of the rectangle which contains the ball */
	int projvertical = ProjectionSurSegment(ball.center.x, ball.center.y, bar.position.x, bar.position.y, bar.position.x, bar.position.y - bar.height);
	int projhorizontal = ProjectionSurSegment(ball.center.x, ball.center.y, bar.position.x, bar.position.y, bar.position.x + bar.width, bar.position.y); 
	if (projvertical == 1 || projhorizontal == 1)
		return 1;

	return 0;   
}



int IntersectionBallBall(Ball ball1, Ball ball2) {
	int d2 = (ball1.center.x - ball2.center.x)*(ball1.center.x - ball2.center.x) + (ball1.center.y - ball2.center.y)*(ball1.center.y - ball2.center.y);
	if (d2 > (ball1.radius + ball2.radius)*(ball1.radius + ball2.radius)) {
		return 0;
	}
	return 1;
}



int CollisionBetweenBallBloc(Ball ball, Bloc bloc) {
	if(((bloc.position.x) >= (ball.center.x + ball.radius + 0.01))     					/* the bloc is at the right */
	|| ((bloc.position.x + bloc.size) <= (ball.center.x - ball.radius - 0.01))   /* the bloc is at the left */
	|| ((bloc.position.y - bloc.size) >= (ball.center.y + ball.radius + 0.01))   /* the bloc is above */
	|| ((bloc.position.y) <= (ball.center.y - ball.radius - 0.01)))  						/* the bloc is below */
		return 0; 
	else
		return 1;
}



int IntersectionBallBloc(Ball ball, Bloc bloc) {
	/* first test : if the ball and the bar are too far each other */
	if (CollisionBetweenBallBloc(ball, bloc) == 0)
		return 0;   

	/* second test */
	if (CollisionBetweenPointAndBall(bloc.position.x, bloc.position.y, ball) == 1
		|| CollisionBetweenPointAndBall(bloc.position.x, bloc.position.y - bloc.size, ball) == 1
		|| CollisionBetweenPointAndBall(bloc.position.x + bloc.size, bloc.position.y, ball) == 1
		|| CollisionBetweenPointAndBall(bloc.position.x + bloc.size, bloc.position.y - bloc.size, ball) == 1)
		return 1;   

	/* test for release the doubt of the rectangle which contains the ball */
	int projvertical = ProjectionSurSegment(ball.center.x, ball.center.y, bloc.position.x, bloc.position.y, bloc.position.x, bloc.position.y - bloc.size);
	int projhorizontal = ProjectionSurSegment(ball.center.x, ball.center.y, bloc.position.x, bloc.position.y, bloc.position.x + bloc.size, bloc.position.y); 
	if (projvertical == 1 || projhorizontal == 1)
		return 1;

	return 0;
}



Vector reboundBallBar(Ball ball, Bar bar) {
	Vector res;
	res.y = -ball.speed.y;
	float gap = (ball.center.x) - (bar.position.x + (bar.width/2.));
	res.x = normalize(gap, bar, ball);
	return res;
}



float normalize(float value, Bar bar, Ball ball) {
	float res;
	res = (value / (bar.width/2.)) * (fabs(ball.speedValue));
	return res;
}



int IntersectionInTheSidesOfTheBloc(Bloc bloc, Ball ball) {
	/* the intersection is in a side of the bloc */
	if (ball.center.y < bloc.position.y
	&& ball.center.y > (bloc.position.y - bloc.size)) 
		return 1;
	return 0;
}
