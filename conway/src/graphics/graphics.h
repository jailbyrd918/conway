/*
	DESCRIPTION:
		Handles primitive drawing functions
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H


struct s_graphics {

	SDL_Window      *window;        // >> console window
	SDL_Renderer    *renderer;      // >> window rendering context

	int             window_width;   // >> window width
	int             window_height;  // >> window height
}
graphics;


#define ASSIGN_COLOR_RGB(r, g, b)       \
        (SDL_Color) {r, g, b, 0xff}

#define ASSIGN_COLOR_RGBA(r, g, b, a)   \
        (SDL_Color) {r, g, b, a}


bool graphics_init
(const char *window_title, const int window_width, const int window_height);

void graphics_free
(void);

void graphics_render
(void);


void graphics_clear_screen
(const SDL_Color color);

void graphics_draw_line
(const int x1, const int y1, const int x2, const int y2, const SDL_Color color);

void graphics_draw_line_fp
(const float x1, const float y1, const float x2, const float y2, const SDL_Color color);

void graphics_draw_rect
(const int x, const int y, const int w, const int h, const bool filled, const bool top_left, const SDL_Color color);

void graphics_draw_rect_fp
(const float x, const float y, const float w, const float h, const bool filled, const bool top_left, const SDL_Color color);


#endif
