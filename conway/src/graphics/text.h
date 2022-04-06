/*
	DESCRIPTION:
		Text definition, handling and rendering
*/


#ifndef TEXT_H
#define TEXT_H


typedef struct s_text_type {
	char		*msg;
	font_t		*font;

	SDL_Color	color;
}
text_t;

typedef enum e_text_alignment_type {
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_CENTER,
	TEXT_ALIGN_RIGHT,
	TEXT_ALIGN_MODES
}
text_align_t;


text_t *text_new
(const char *message, const char *font_id, const SDL_Color color);

bool text_free
(text_t *text);


bool text_set_message
(text_t *text, const char *message);

bool text_set_color
(text_t *text, const SDL_Color color);

bool text_draw
(text_t *text, const int x, const int y);

bool text_draw_aligned
(text_t *text, const int y, const int x_padding, const text_align_t text_align);



#endif
