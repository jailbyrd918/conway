/*
	DESCRIPTION:
		World editor definition and handling
*/

#ifndef EDITOR_H
#define EDITOR_H


struct s_editor {

	struct s_grid {
		float	world_x, world_y;
		int	screen_x, screen_y;

		int	size, cols, rows;
	}
	grid;

	struct s_cursor {
		int	pos_x, pos_y;
		int	start_x, start_y;

		float	world_x_pre_zoom, world_y_pre_zoom, 
			world_x_post_zoom, world_y_post_zoom;

		int	size;
	}
	cursor;

	float	scale, offset_x, offset_y;

}
editor;


void editor_init
(void);

void editor_update
(float delta_time);

void editor_draw
(void);


#endif
