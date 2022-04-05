/*
	DESCRIPTION:
		Entry point of the program
*/

#include "core/program.h"


int main
(void)
{
	program_init();
	
	program_run();

	program_quit();
	return 0;
}

