#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "str.h"

#include "font.h"


static struct s_font_list {

	font_t	*data;

	size_t	size;
	size_t	capacity;

}
font_list;


void font_init_list
(void)
{
	font_list.size = 0;
	font_list.capacity = 5;
	font_list.data = malloc(sizeof(font_t) * font_list.capacity);
}

void font_free_list
(void)
{
	for (size_t i = 0; i < font_list.size; ++i)
		TTF_CloseFont(font_list.data[i].ref);

	font_list.size = 0;
	free(font_list.data);
}

bool font_load_from_file
(const char *new_font_id, const char *filename, const int font_size)
{
	if (!new_font_id || !filename || str_empty(new_font_id) || str_empty(filename))
		return false;

	// new font id must be unique
	if (font_get_by_id(new_font_id) != NULL)
		return false;


	// -- initialize new font properties -- //

	char *newfontid = str_copy(new_font_id);
	TTF_Font *ref = TTF_OpenFont(filename, font_size);
	if (!ref)	return false;


	// -- resize list if full -- //

	if (font_list.size >= font_list.capacity) {
		font_list.capacity *= 2;

		font_t *newmem = realloc(font_list.data, sizeof(font_t) * font_list.capacity);
		if (!newmem)	return false;

		font_list.data = newmem;
	}


	// add new font to list
	font_list.data[font_list.size++] = (font_t) { newfontid, ref };

	return true;
}

font_t *font_get_by_id
(const char *font_id)
{
	if (!font_id || str_empty(font_id))
		return NULL;

	for (size_t i = 0; i < font_list.size; ++i)
		if (str_matched(font_list.data[i].id, font_id))
			return &(font_list.data[i]);

	return NULL;
}
