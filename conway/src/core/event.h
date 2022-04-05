/*
	DESCRIPTION:
		Event system definition and handling
*/

#ifndef EVENT_H
#define EVENT_H


struct s_event_system {

	SDL_Event	event;

	const uint8_t	*keyboard_state;
	SDL_Scancode	pressed_key;
	bool		key_pressed;

	struct s_mouse {
		uint32_t	state;
		int		pos_x, pos_y;
		int		scroll_y;
	}
	mouse;

}
event_system;


// >> initializes event system
void event_init_system
(void);

// >> processes user input events
void event_proc_input
(void);


bool event_key_pressed
(SDL_Scancode key);

bool event_key_held
(SDL_Scancode key);


bool event_mouse_left_clicked
(void);

bool event_mouse_middle_clicked
(void);

bool event_mouse_right_clicked
(void);


// -- event handlers -- ////////////////////////////////////////////

void event_on_exit_program_handle
(void);


void event_on_mouse_clicked_editor_handle
(void);

void event_on_mouse_scrolled_editor_handle
(void);

void event_on_key_pressed_editor_handle
(void);

void event_on_key_released_editor_handle
(void);


////////////////////////////////////////////////////////////////////


#endif
