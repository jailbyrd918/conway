#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "utils/str.h"
#include "utils/font.h"
#include "graphics.h"

#include "text.h"


text_t *text_new
(const char *message, const char *font_id, const SDL_Color color)
{
	text_t *newtxt = malloc(sizeof(text_t));

	newtxt->msg = str_copy(message);
	newtxt->font = font_get_by_id(font_id);
	newtxt->color = color;

	return newtxt;
}

bool text_free
(text_t *text)
{
	if (!text)	return false;

	free(text);
	return true;
}


bool text_set_message
(text_t *text, const char *message)
{
	if (!text || !message)
		return false;

	char *newmem = realloc(text->msg, strlen(message) + 1);
	if (!newmem)	return false;
	text->msg = newmem;

	text->msg = str_copy(message);
	return true;
}

bool text_set_color
(text_t *text, const SDL_Color color)
{
	if (!text)
		return false;

	text->color = color;
	return true;
}

bool text_draw
(text_t *text, const int x, const int y)
{
	if (!text || !text->font)
		return false;

	SDL_Surface *surface = TTF_RenderUTF8_Solid(text->font->ref, text->msg, text->color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(graphics.renderer, surface);
	SDL_FreeSurface(surface);

	int txtw = 0, txth = 0;
	SDL_QueryTexture(texture, NULL, NULL, &txtw, &txth);

	SDL_Rect rtxt = { x, y, txtw, txth };
	SDL_RenderCopy(graphics.renderer, texture, NULL, &rtxt);

	SDL_DestroyTexture(texture);
	return true;
}


