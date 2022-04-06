#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <SDL.h>

#include "world.h"


bool world_init
(const int world_cols, const int world_rows)
{
	if (world_cols <= 0 || world_rows <= 0)
		return false;

	world.cols = world_cols;
	world.rows = world_rows;

	world.buf_cells = malloc(sizeof(int8_t) * (world_cols * world_rows));
	for (int i = 0; i < (world_cols * world_rows); ++i)
		world.buf_cells[i] = 0;

	world.render_cells = malloc(sizeof(int8_t) * (world_cols * world_rows));

	return true;
}

bool world_update
(const bool playing)
{	
	memcpy(world.render_cells, world.buf_cells, sizeof(int8_t) * (world.cols * world.rows));

	if (!playing)
		return false;

	for (int x = 1; x < world.cols - 1; ++x) {
		for (int y = 1; y < world.rows - 1; ++y) {
			int8_t	celln = world.render_cells[(y - 1) * world.cols + x],
				cells = world.render_cells[(y + 1) * world.cols + x],
				celle = world.render_cells[y * world.cols + (x + 1)],
				cellw = world.render_cells[y * world.cols + (x - 1)],
				cellnw = world.render_cells[(y - 1) * world.cols + (x - 1)],
				cellne = world.render_cells[(y - 1) * world.cols + (x + 1)],
				cellsw = world.render_cells[(y + 1) * world.cols + (x - 1)],
				cellse = world.render_cells[(y + 1) * world.cols + (x + 1)];

			int	neighbornum = celln + cells + celle + cellw + cellnw + cellne + cellsw + cellse;

			if (world.buf_cells[y * world.cols + x])
				world.buf_cells[y * world.cols + x] = (neighbornum == 2 || neighbornum == 3);
			else
				world.buf_cells[y * world.cols + x] = (neighbornum == 3);
		}
	}

	return true;
}

void world_free
(void)
{
	free(world.render_cells);
	free(world.buf_cells);
}


bool world_set_cell
(const int x, const int y)
{
	return (world.buf_cells[y * world.cols + x] = (world.buf_cells[y * world.cols + x] == 1) ? 0 : 1);
}

void world_clear_cells
(void)
{
	memset(world.buf_cells, 0, sizeof(int8_t) * (world.cols * world.rows));
}

void world_randomize_cells
(void)
{
	for (int i = 0; i < world.cols * world.rows; ++i)
		world.buf_cells[i] = rand() % 2;
}

