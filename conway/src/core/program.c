#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "graphics/graphics.h"
#include "event.h"

#include "program.h"


static struct s_program {

	int	frames_per_sec,
		frame_duration,
		old_tick;

	float	delta_time;

	bool	active;

}
program;


static void _program_update_delta_time
(void)
{
	// delay until frame duration is reached
	int delaytime = program.frame_duration - (SDL_GetTicks() - program.old_tick);

	if (delaytime >= 0 && delaytime <= program.frame_duration)
		SDL_Delay(delaytime);

	// calculate and update delta time
	program.delta_time = (SDL_GetTicks() - program.old_tick) / 1000.f;

	// update program old ticked frame
	program.old_tick = SDL_GetTicks();
}

static void _program_update
(void)
{
	_program_update_delta_time();
}

static void _program_render
(void)
{
	graphics_clear_screen(ASSIGN_COLOR_RGB(0x00, 0x00, 0x00));



	graphics_render();
}


// -- event handlers -- //

void event_on_exit_program_handle
(void)
{
	program.active = false;
}

//////////////////////////


void program_init
(void)
{
	// -- ensure that the program is good to run -- //

	bool statusok = true;
	statusok &= (SDL_Init(SDL_INIT_EVERYTHING) == 0);	// initialize SDL core subsystems
	statusok &= (TTF_Init() == 0);				// initialize SDL TTF engine
	statusok &= (graphics_init("Conway's Game of Life", 1024, 768));


	// -- initialize program properties and components -- //

	program.frames_per_sec = 120;
	program.frame_duration = 1000 / program.frames_per_sec;
	program.old_tick = 0;
	program.delta_time = 0.f;
	program.active = statusok;

	event_init_system();

}

void program_run
(void)
{
	do {
		 event_proc_input();
		_program_update();
		_program_render();

	} while (program.active);
}

void program_quit
(void)
{
	// -- free engine properties and components -- //

	graphics_free();

	TTF_Quit();	// shut down TTF engine
	SDL_Quit();     // clean up initialize SDL core subsystems
}
