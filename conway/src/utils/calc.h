/*
	DESCRIPTION:
		Handles mathematics related utility functions
*/


#ifndef CALC_H
#define CALC_H


// >> returns the greater value of both
#define MAX(val1, val2)                 ((val1 > val2) ? val1 : val2)

// >> returns the smaller value of both
#define MIN(val1, val2)                 ((val1 < val2) ? val1 : val2)

// >> restricts the given value to between min value and max value
#define CLAMP(val, min_val, max_val)    (MIN(MAX(val, min_val), max_val))


// >> converts screen space to world space position
int calc_world_to_screen
(float world, float offset, float scale);

// >> converts world space to screen space position
float calc_screen_to_world
(int screen, float offset, float scale);


#endif
