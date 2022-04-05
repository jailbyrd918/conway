#include <stdbool.h>
#include <stdlib.h>

#include <SDL.h>

#include "event.h"


void event_init_system
(void)
{
	event_system.pressed_key = (SDL_Scancode)(0x00);
	event_system.keyboard_state = SDL_GetKeyboardState(NULL);

	event_system.mouse.state = (uint32_t)(0x00);
}

void event_proc_input
(void)
{
	while (SDL_PollEvent(&event_system.event) != 0) {
		switch (event_system.event.type) {
			case SDL_QUIT:
				// handle program exit event on program side
				event_on_exit_program_handle();
				break;

			case SDL_KEYDOWN:
				// handle key press event on editor side
				event_on_key_pressed_editor_handle();

				switch (event_system.event.key.keysym.sym) {
					case SDLK_ESCAPE:
						// handle program exit event on program side
						event_on_exit_program_handle();
						break;

					default:
						break;
				}

				// record pressed key
				event_system.pressed_key = (!event_system.key_pressed) ? event_system.event.key.keysym.scancode : (SDL_Scancode)(0x00);
				event_system.key_pressed = true;

				break;

			case SDL_KEYUP:
				// handle key release event on editor side
				event_on_key_released_editor_handle();

				// reset pressed key
				event_system.pressed_key = (SDL_Scancode)(0x00);
				event_system.key_pressed = false;
				break;

			case SDL_MOUSEBUTTONDOWN:
				// record mouse state
				event_system.mouse.state = SDL_GetMouseState(NULL, NULL);

				// handle mouse click event on editor side
				event_on_mouse_clicked_editor_handle();

				break;

			case SDL_MOUSEBUTTONUP:
				// reset mouse state
				event_system.mouse.state = (uint32_t)(0x00);
				break;

			case SDL_MOUSEMOTION:
				// record mouse screen space position
				event_system.mouse.pos_x = event_system.event.motion.x;
				event_system.mouse.pos_y = event_system.event.motion.y;
				break;

			case SDL_MOUSEWHEEL:
				// record mouse scroll y value
				event_system.mouse.scroll_y = event_system.event.wheel.y;

				// handle mouse scroll event on editor side
				event_on_mouse_scrolled_editor_handle();
				break;

			default:
				break;
		}
	}
}


bool event_key_pressed
(SDL_Scancode key)
{
	return (event_system.key_pressed) && (event_system.pressed_key == key);
}

bool event_key_held
(SDL_Scancode key)
{
	return event_system.keyboard_state[key];
}


bool event_mouse_left_clicked
(void)
{
	return (event_system.mouse.state & SDL_BUTTON_LMASK);
}

bool event_mouse_middle_clicked
(void)
{
	return (event_system.mouse.state & SDL_BUTTON_MMASK);
}

bool event_mouse_right_clicked
(void)
{
	return (event_system.mouse.state & SDL_BUTTON_RMASK);
}