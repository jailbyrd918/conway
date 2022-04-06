#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "world.h"


bool world_init
(const int world_cols, const int world_rows)
{
	if (world_cols <= 0 || world_rows <= 0)
		return false;

	world.cols = world_cols;
	world.rows = world_rows;

	world.buf_cells = malloc(sizeof(int8_t) * (world_cols * world_rows));
	memset(world.buf_cells, 0, sizeof(int8_t) * (world_cols * world_rows));
	world.render_cells = malloc(sizeof(int8_t) * (world_cols * world_rows));
	world.generation = 0;

	return true;
}


/* 
	It is required that cellular automaton implemented in a simultaneous manner
	
	Such manner is achieved through a simple set of instructions in the context of this program:
	- Copy the entire buffer cells data to the rendering cells
		The rendering cells is treated as both the cells data to render and reference to old buffer cells
	- Iterate through the rendering cells and determine state of buffer cells under the same cell index
	- The entire buffer cells data is updated
	- Render the rendering cells

	On the other hand, avoidance of such manner will result in incorrect outputs
		as state for every cell is being determined in their respective epoch of time
*/

bool world_update
(const bool playing)
{
	memcpy(world.render_cells, world.buf_cells, sizeof(int8_t) * (world.cols * world.rows));

	if (!playing)
		return false;

	for (int x = 0; x < world.cols; ++x) {
		for (int y = 0; y < world.rows; ++y) {
			bool	topmost = (y == 0),
				bottommost = (y == world.rows - 1),
				leftmost = (x == 0),
				rightmost = (x == world.cols - 1);

			int8_t	celln = topmost ? 0 : world.render_cells[(y - 1) * world.cols + x],
				cells = bottommost ? 0 : world.render_cells[(y + 1) * world.cols + x],
				celle = rightmost ? 0 : world.render_cells[y * world.cols + (x + 1)],
				cellw = leftmost ? 0 : world.render_cells[y * world.cols + (x - 1)],
				cellnw = (topmost || leftmost) ? 0 : world.render_cells[(y - 1) * world.cols + (x - 1)],
				cellne = (topmost || rightmost) ? 0 : world.render_cells[(y - 1) * world.cols + (x + 1)],
				cellsw = (bottommost || leftmost) ? 0 : world.render_cells[(y + 1) * world.cols + (x - 1)],
				cellse = (bottommost || rightmost) ? 0 : world.render_cells[(y + 1) * world.cols + (x + 1)];

			int	neighbornum = celln + cells + celle + cellw + cellnw + cellne + cellsw + cellse;

			if (world.buf_cells[y * world.cols + x])
				world.buf_cells[y * world.cols + x] = (neighbornum == 2 || neighbornum == 3);
			else
				world.buf_cells[y * world.cols + x] = (neighbornum == 3);
		}
	}

	world.generation++;

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

	world.generation = 0;
}

