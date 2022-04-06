#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "utils/str.h"
#include "utils/font.h"
#include "utils/calc.h"
#include "graphics/graphics.h"
#include "graphics/text.h"
#include "world/world.h"
#include "event.h"

#include "editor.h"


// -- editor private properties -- /////////////////////////////////////////////////////

static float	cursor_world_x = 0.f, cursor_world_y = 0.f;
static float	editor_delta_time = 0.f;

static bool	show_instructions = false, show_info = true;


// -- world cell manipulating functions -- /////////////////////////////////////////////

static bool _editor_place_cell
(void)
{
	int	cellx = (int)floorf(cursor_world_x / editor.grid.size),
		celly = (int)floorf(cursor_world_y / editor.grid.size);

	cellx = CLAMP(cellx, 0, world.cols - 1);
	celly = CLAMP(celly, 0, world.rows - 1);

	return world_set_cell(cellx, celly);
}


// -- event handers -- /////////////////////////////////////////////////////////////////

void event_on_mouse_clicked_editor_handle
(void)
{
	editor.cursor.start_x = editor.cursor.pos_x;
	editor.cursor.start_y = editor.cursor.pos_y;

	if (event_mouse_left_clicked())
		_editor_place_cell();
}

void event_on_mouse_scrolled_editor_handle
(void)
{
	// -- zooming -- // 

	switch (event_system.mouse.scroll_y) {
		case -1:
			// scroll down - zoom out
			editor.scale *= .9f;
			break;

		case 1:
			// scroll up - zoom in 
			editor.scale *= 1.1f;
			break;
	}

	editor.cursor.world_x_post_zoom = calc_screen_to_world(editor.cursor.pos_x, editor.offset_x, editor.scale);
	editor.cursor.world_y_post_zoom = calc_screen_to_world(editor.cursor.pos_y, editor.offset_y, editor.scale);

	editor.offset_x += (editor.cursor.world_x_pre_zoom - editor.cursor.world_x_post_zoom);
	editor.offset_y += (editor.cursor.world_y_pre_zoom - editor.cursor.world_y_post_zoom);

	///////////////////
}

void event_on_key_pressed_editor_handle
(void)
{
	switch (event_system.event.key.keysym.sym) {
		case SDLK_p:
			editor.state = (editor.state == STATE_PAUSED) ? STATE_PLAYING : STATE_PAUSED;
			break;

		case SDLK_r:
			world_randomize_cells();
			break;

		case SDLK_c:
			world_clear_cells();
			break;

		case SDLK_i:
			show_info = !show_info;
			break;

		case SDLK_F1:
			show_instructions = !show_instructions;
			break;

		default:
			break;
	}
}

void event_on_key_released_editor_handle
(void)
{
	switch (event_system.event.key.keysym.sym) {

		default:
			break;
	}
}


// -- drawing functions -- //////////////////////////////////////////////////////////////////////

#define	EDITOR_SCALE_SIZE(size, scale)	\
	(size / (1.f / scale))


static void _editor_draw_grid
(void)
{
	float	gridstartx = editor.grid.world_x,
		gridstarty = editor.grid.world_y,
		gridendx = gridstartx + (editor.grid.size * editor.grid.cols),
		gridendy = gridstarty + (editor.grid.size * editor.grid.rows);

	for (float x = gridstartx, y = gridstarty; x <= gridendx, y <= gridendy; x += editor.grid.size, y += editor.grid.size) {

		// -- draw vertical lines -- //

		int	scrx = calc_world_to_screen(x, editor.offset_x, editor.scale),
			scry = calc_world_to_screen(gridstarty, editor.offset_y, editor.scale);

		graphics_draw_line(scrx, scry, scrx, calc_world_to_screen(gridendy, editor.offset_y, editor.scale), ASSIGN_COLOR_RGB(0x3a, 0x3b, 0x3c));


		// -- draw horizontal lines -- //

		scrx = calc_world_to_screen(gridstartx, editor.offset_x, editor.scale);
		scry = calc_world_to_screen(y, editor.offset_y, editor.scale);

		graphics_draw_line(scrx, scry, calc_world_to_screen(gridendx, editor.offset_x, editor.scale), scry, ASSIGN_COLOR_RGB(0x3a, 0x3b, 0x3c));

	}
}

static void _editor_draw_cells
(void)
{
	for (int cellcol = 0; cellcol < world.cols; ++cellcol) {
		for (int cellrow = 0; cellrow < world.rows; ++cellrow) {
			float	cellx = (float)(cellcol * editor.grid.size),
				celly = (float)(cellrow * editor.grid.size);
			int8_t	cellstate = world.render_cells[cellrow * world.cols + cellcol];

			float	cellstartwx = floorf(cellx / editor.grid.size) * editor.grid.size,
				cellstartwy = floorf(celly / editor.grid.size) * editor.grid.size;
			int	cellstartsx = calc_world_to_screen(cellstartwx, editor.offset_x, editor.scale),
				cellstartsy = calc_world_to_screen(cellstartwy, editor.offset_y, editor.scale);

			graphics_draw_rect(
				cellstartsx,
				cellstartsy,
				(int)(EDITOR_SCALE_SIZE(editor.grid.size, editor.scale)),
				(int)(EDITOR_SCALE_SIZE(editor.grid.size, editor.scale)),
				true,
				true,
				(cellstate == 1) ? ASSIGN_COLOR_RGB(0x00, 0xff, 0x00) : ASSIGN_COLOR_RGB(0x00, 0x00, 0x00)
			);
		}
	}

}

static void _editor_draw_suggest_drawing_cell
(void)
{
	float	cellstartwx = floorf(cursor_world_x / editor.grid.size) * editor.grid.size,
		cellstartwy = floorf(cursor_world_y / editor.grid.size) * editor.grid.size;

	cellstartwx = CLAMP(cellstartwx, 0.f, (float)(editor.grid.size * editor.grid.cols) - editor.grid.size);
	cellstartwy = CLAMP(cellstartwy, 0.f, (float)(editor.grid.size * editor.grid.rows) - editor.grid.size);

	int	cellstartsx = calc_world_to_screen(cellstartwx, editor.offset_x, editor.scale),
		cellstartsy = calc_world_to_screen(cellstartwy, editor.offset_y, editor.scale);

	graphics_draw_rect(
		cellstartsx,
		cellstartsy,
		(int)(EDITOR_SCALE_SIZE(editor.grid.size, editor.scale)),
		(int)(EDITOR_SCALE_SIZE(editor.grid.size, editor.scale)),
		true,
		true,
		ASSIGN_COLOR_RGB(0xcd, 0x7f, 0x32)
	);
}

static void _editor_draw_cursor
(void)
{
	// draw vertical line
	graphics_draw_line_fp(
		(float)editor.cursor.pos_x,
		(float)editor.cursor.pos_y - (editor.cursor.size / 2),
		(float)editor.cursor.pos_x,
		(float)editor.cursor.pos_y + (editor.cursor.size / 2),
		ASSIGN_COLOR_RGB(0xff, 0xff, 0xff)
	);

	// draw horizontal line
	graphics_draw_line_fp(
		(float)editor.cursor.pos_x - (editor.cursor.size / 2),
		(float)editor.cursor.pos_y,
		(float)editor.cursor.pos_x + (editor.cursor.size / 2),
		(float)editor.cursor.pos_y,
		ASSIGN_COLOR_RGB(0xff, 0xff, 0xff)
	);
}

static bool _editor_draw_instructions
(void)
{
	if (!show_instructions)
		return false;

	graphics_draw_rect(0, graphics.window_height - 30, graphics.window_width, 30, true, true, ASSIGN_COLOR_RGB(0x00, 0x00, 0x00));

	text_t *helptxt = text_new("", "webly_sleek", ASSIGN_COLOR_RGB(0xbe, 0xbe, 0xbe));
	char sep[] = "          ";
	char str1[] = "< C > clear all cells";
	char str2[] = "< R > reset and randomize all cells";
	char str3[] = "< P > play / pause";
	text_set_message(helptxt, str_combine_strs(5, str1, sep, str2, sep, str3));
	int rpadding = 50;
	int len = strlen(helptxt->msg);
	text_draw_aligned(helptxt, graphics.window_height - 30, 50, TEXT_ALIGN_CENTER);
	text_free(helptxt);

	return true;
}

static bool _editor_draw_info
(void)
{
	if (!show_info)
		return false;

	// -- draw frame rate -- //

	text_t *fpstxt = text_new("", "webly_sleek", ASSIGN_COLOR_RGB(0xff, 0xff, 0xff));
	int fps = (int)(1.f / editor_delta_time);
	char fpsmsg[64];
	sprintf(fpsmsg, "FPS: %d", fps);
	text_set_message(fpstxt, fpsmsg);
	text_draw(fpstxt, 50 , 20);
	text_free(fpstxt);


	// -- draw state info -- //

	text_t *statetxt = text_new("", "webly_sleek", ASSIGN_COLOR_RGB(0xff, 0xff, 0xff));

	switch (editor.state) {
		case STATE_PAUSED:
			text_set_message(statetxt, "State: Paused");
			text_set_color(statetxt, ASSIGN_COLOR_RGB(0xff, 0x00, 0x00));
			break;

		case STATE_PLAYING:
			text_set_message(statetxt, "State: Playing");
			text_set_color(statetxt, ASSIGN_COLOR_RGB(0x00, 0xff, 0x00));
			break;

		default:
			break;
	}

	text_draw(statetxt, 50, 40);
	text_free(statetxt);
	
	
	// -- draw cell info -- //

	text_t *celltxt = text_new("", "webly_sleek", ASSIGN_COLOR_RGB(0xff, 0xff, 0xff));
	char cellmsg[64];

	int cellnum = 0;
	for (int i = 0; i < world.cols * world.rows; ++i)
		cellnum += world.buf_cells[i];
	
	sprintf(cellmsg, "Cells: %d", cellnum);
	text_set_message(celltxt, cellmsg);
	text_draw(celltxt, 50, 60);
	text_free(celltxt);


	// -- draw generation info -- //

	text_t *gentxt = text_new("", "webly_sleek", ASSIGN_COLOR_RGB(0xff, 0xff, 0xff));
	char genmsg[64];
	sprintf(genmsg, "Generation: %llu", world.generation);
	text_set_message(gentxt, genmsg);
	text_draw(gentxt, 50, 80);
	text_free(gentxt);
	
	
	// -- draw help text -- //

	text_t *helptxt = text_new("", "webly_sleek", ASSIGN_COLOR_RGB(0xbe, 0xbe, 0xbe));

	text_set_message(helptxt, "< I > Hide infos");
	text_draw(helptxt, 50, 110);
	
	text_set_message(helptxt, (!show_instructions) ? ("< F 1 > Show Instructions") : ("< F 1 > Hide Instructions"));
	text_draw(helptxt, 50, 130);
	
	text_set_message(helptxt, "< Esc > Quit");
	text_draw(helptxt, 50, 150);
	
	text_free(helptxt);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////


void editor_init
(void)
{
	// -- initialize editor components and properties -- //

	editor.grid.size = 64;
	editor.grid.cols = world.cols;
	editor.grid.rows = world.rows;

	editor.scale = 1.f;
	editor.offset_x = (editor.grid.size * editor.grid.cols) / 2.f;
	editor.offset_y = (editor.grid.size * editor.grid.rows) / 2.f;


	editor.grid.world_x = 0.f;
	editor.grid.world_y = 0.f;
	editor.grid.screen_x = calc_world_to_screen(editor.grid.world_x, editor.offset_x, editor.scale);
	editor.grid.screen_y = calc_world_to_screen(editor.grid.world_y, editor.offset_y, editor.scale);

	editor.cursor.pos_x = event_system.mouse.pos_x;
	editor.cursor.pos_y = event_system.mouse.pos_y;
	editor.cursor.size = 10;

	editor.state = STATE_PAUSED;

	// disable default cursor - replace with custom cursor
	SDL_ShowCursor(false);
}

void editor_update
(float delta_time)
{
	// -- record cursor screen space position -- // 

	editor.cursor.pos_x = event_system.mouse.pos_x;
	editor.cursor.pos_y = event_system.mouse.pos_y;


	// -- record cursor world space position -- //

	float	cursorwx = calc_screen_to_world(editor.cursor.pos_x, editor.offset_x, editor.scale),
		cursorwy = calc_screen_to_world(editor.cursor.pos_y, editor.offset_y, editor.scale);

	cursor_world_x = cursorwx;
	cursor_world_y = cursorwy;


	// update editor private delta time property
	editor_delta_time = delta_time;


	// -- calculate cursor world space position before zoom -- //

	editor.cursor.world_x_pre_zoom = calc_screen_to_world(editor.cursor.pos_x, editor.offset_x, editor.scale);
	editor.cursor.world_y_pre_zoom = calc_screen_to_world(editor.cursor.pos_y, editor.offset_y, editor.scale);

	if (event_mouse_right_clicked()) {

		// -- panning -- //

		editor.offset_x -= (editor.cursor.pos_x - editor.cursor.start_x) * (1.f / editor.scale);
		editor.offset_y -= (editor.cursor.pos_y - editor.cursor.start_y) * (1.f / editor.scale);

		editor.cursor.start_x = editor.cursor.pos_x;
		editor.cursor.start_y = editor.cursor.pos_y;

		///////////////////

	}

}

void editor_draw
(void)
{
	_editor_draw_cells();
	_editor_draw_suggest_drawing_cell();
	_editor_draw_grid();

	_editor_draw_instructions();
	_editor_draw_cursor();
	_editor_draw_info();

}