#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "utils/str.h"
#include "utils/font.h"
#include "utils/calc.h"
#include "graphics/graphics.h"
#include "graphics/text.h"
#include "event.h"

#include "editor.h"


// -- editor private properties -- /////////////////////////////////////////////////////

static float	cursor_world_x = 0.f, cursor_world_y = 0.f;
static float	editor_delta_time = 0.f;


// -- event handers -- /////////////////////////////////////////////////////////////////

void event_on_mouse_clicked_editor_handle
(void)
{
	editor.cursor.start_x = editor.cursor.pos_x;
	editor.cursor.start_y = editor.cursor.pos_y;
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
			break;

		case SDLK_d:
			printf("debug");
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

static void _editor_draw_info
(void)
{
	// -- log draw frame rate -- //

	text_t *fpstxt = text_new("", "webly_sleek", ASSIGN_COLOR_RGB(0xff, 0xff, 0xff));
	int fps = (int)(1.f / editor_delta_time);
	char fpsmsg[64];
	sprintf(fpsmsg, "FPS: %d", fps);
	text_set_message(fpstxt, fpsmsg);
	text_draw(fpstxt, 50, 20);
	text_free(fpstxt);


	//// -- log draw player info -- //

	//text_t *playertxt = text_new("", "webly_sleek_14", ASSIGN_COLOR_RGB(0x00, 0xff, 0x00));
	//char coordmsg[128];
	//sprintf(coordmsg, "Player: [x: %g, y: %g], angle: %d, sector: %d", player.pos_x, player.pos_y, (int)RAD_TO_DEG(player.yaw), player_get_current_sector());
	//text_set_message(playertxt, coordmsg);
	//text_draw(playertxt, 50, 40);
	//text_free(playertxt);


	//// -- log draw map infos -- //

	//// -- walls -- //

	//text_t *wallstxt = text_new("", "webly_sleek_14", ASSIGN_COLOR_RGB(0xff, 0xff, 0xff));
	//char wallsmsg[128];
	//sprintf(wallsmsg, "Walls: %d / %d", currmap->num_walls, MAX_NUM_WALLS);
	//text_set_message(wallstxt, wallsmsg);
	//text_draw(wallstxt, 50, 60);
	//text_free(wallstxt);

	//// -- sectors -- //

	//text_t *sectstxt = text_new("", "webly_sleek_14", ASSIGN_COLOR_RGB(0xff, 0xff, 0xff));
	//char sectsmsg[128];
	//sprintf(sectsmsg, "Sectors: %d / %d", currmap->num_sectors, MAX_NUM_SECTORS);
	//text_set_message(sectstxt, sectsmsg);
	//text_draw(sectstxt, 50, 80);
	//text_free(sectstxt);

}

/////////////////////////////////////////////////////////////////////////////////////////////////


void editor_init
(void)
{
	// -- initialize editor components and properties -- //

	editor.grid.size = 64;
	editor.grid.cols = 1024;
	editor.grid.rows = 1024;

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

	// disable default cursor - replace with custom cursor
	SDL_ShowCursor(false);

}

void editor_update
(float delta_time)
{
	// -- record cursor screen space position -- // 

	editor.cursor.pos_x = event_system.mouse.pos_x;
	editor.cursor.pos_y = event_system.mouse.pos_y;


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


	// -- determine draw/potential wall position -- //

	float	cursorwx = calc_screen_to_world(editor.cursor.pos_x, editor.offset_x, editor.scale),
		cursorwy = calc_screen_to_world(editor.cursor.pos_y, editor.offset_y, editor.scale);

	cursor_world_x = cursorwx;
	cursor_world_y = cursorwy;


	// update editor private delta time property
	editor_delta_time = delta_time;

}



void editor_draw
(void)
{
	_editor_draw_grid();

	_editor_draw_cursor();
	_editor_draw_info();

}