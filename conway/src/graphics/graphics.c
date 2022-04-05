#include <stdbool.h>
#include <stdlib.h>

#include <SDL.h>

#include "graphics.h"


bool graphics_init
(const char *window_title, const int window_width, const int window_height)
{
	if (window_width <= 0 || window_height <= 0)
		return false;

	// create console window
	graphics.window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
	if (!graphics.window)    return false;

	// initialize window resolution properties
	SDL_GetWindowSize(graphics.window, &graphics.window_width, &graphics.window_height);

	// create rendering context for the window
	graphics.renderer = SDL_CreateRenderer(graphics.window, -1, SDL_RENDERER_SOFTWARE);
	if (!graphics.renderer)  return false;

	return true;
}

void graphics_free
(void)
{
	SDL_DestroyRenderer(graphics.renderer);         // destroy window rendering context
	SDL_DestroyWindow(graphics.window);             // destroy console window
}

void graphics_render
(void)
{
	SDL_RenderPresent(graphics.renderer);
}


void graphics_clear_screen
(const SDL_Color color)
{
	SDL_SetRenderDrawColor(graphics.renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(graphics.renderer);
}

void graphics_draw_line
(const int x1, const int y1, const int x2, const int y2, const SDL_Color color)
{
	SDL_SetRenderDrawColor(graphics.renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(graphics.renderer, x1, y1, x2, y2);
}

void graphics_draw_line_fp
(const float x1, const float y1, const float x2, const float y2, const SDL_Color color)
{
	SDL_SetRenderDrawColor(graphics.renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLineF(graphics.renderer, x1, y1, x2, y2);
}

void graphics_draw_rect
(const int x, const int y, const int w, const int h, const bool filled, const bool top_left, const SDL_Color color)
{
	SDL_SetRenderDrawColor(graphics.renderer, color.r, color.g, color.b, color.a);

	SDL_Rect rect = (top_left) ? (SDL_Rect) { x, y, w, h } : (SDL_Rect) { x - (w / 2), y - (h / 2), w, h };

	if (filled)
		SDL_RenderFillRect(graphics.renderer, &rect);
	else
		SDL_RenderDrawRect(graphics.renderer, &rect);
}

void graphics_draw_rect_fp
(const float x, const float y, const float w, const float h, const bool filled, const bool top_left, const SDL_Color color)
{
	SDL_SetRenderDrawColor(graphics.renderer, color.r, color.g, color.b, color.a);

	SDL_FRect rect = (top_left) ? (SDL_FRect) { x, y, w, h } : (SDL_FRect) { x - (w / 2), y - (h / 2), w, h };

	if (filled)
		SDL_RenderFillRectF(graphics.renderer, &rect);
	else
		SDL_RenderDrawRectF(graphics.renderer, &rect);
}

