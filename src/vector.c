#include "vector.h"



Vector VectorXY(float x, float y) {
	Vector v;
	v.x = x;
	v.y = y;
	return v;
}



int immobile(Vector v) {
	return (v.x == 0 && v.y == 0) ? 1 : 0;	
}



Vector accelerate (Vector speed, float n) {
	/* if it's not too fast */
	if (speed.y < 0.6 || speed.y < -0.6) { 
		speed.x *= n;
		speed.y *= n;
	}
	return speed;
}


Vector decelerate (Vector speed, float n) {
	/* if it's not too slow */
	if (speed.y > 0.1 || speed.y < -0.1 ) {
		speed.x /= n;
		speed.y /= n;
	}
	return speed;
}
