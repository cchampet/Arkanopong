#ifndef DEF_VECTOR
#define DEF_VECTOR



/* a vector => x, y 
the vector is used in the program for the direction and the speed for the ball */
typedef struct Vecteur_s {
	float x;
	float y;
} Vector;



/********** Prototypes functions **********/
/* to create a vector */
Vector VectorXY(float x, float y);
/* test if a vector (the speed of a ball) is null */
int immobile(Vector v);
/* to accelerate the ball (bloc BONUS_FASTER) */
Vector accelerate (Vector speed, float n);
/* to decelerate the ball (bloc BONUS_SLOWER) */
Vector decelerate (Vector speed, float n);



#endif
