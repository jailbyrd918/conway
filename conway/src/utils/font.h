/*
	DESCRIPTION:
		TrueType font definition and handling
*/

#ifndef FONT_H
#define FONT_H


typedef struct s_font_type {
	char		*id;
	TTF_Font	*ref;
} font_t;


// >> initializes font internal storage list
void font_init_list
(void);

// >> frees font internal storage list
void font_free_list
(void);


bool font_load_from_file
(const char *new_font_id, const char *filename, const int font_size);


font_t *font_get_by_id
(const char *font_id);



#endif
