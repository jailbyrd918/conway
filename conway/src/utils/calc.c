#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include <SDL.h>

#include "calc.h"


int calc_world_to_screen
(float world, float offset, float scale)
{
	return (int)((world - offset) * scale);
}

float calc_screen_to_world
(int screen, float offset, float scale)
{
	return (screen / scale) + offset;
}

