/*
	DESCRIPTION:
		World definition and handling
*/

#ifndef WORLD_H
#define WORLD_H


struct s_world {
	
	int8_t		*buf_cells, *render_cells;
	uint64_t	generation;

	int		cols, rows;

}
world;


bool world_init
(const int world_cols, const int world_rows);

bool world_update
(const bool playing);

void world_free
(void);


bool world_set_cell
(const int x, const int y);

void world_clear_cells
(void);

void world_randomize_cells
(void);


#endif
