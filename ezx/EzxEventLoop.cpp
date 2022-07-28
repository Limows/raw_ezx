/*
 * HACK: Use this overloaded SDL function for implementing hide and
 * show routines while SMS, Calls, etc. on Motorola EZX platform.
 */

#include "EzxEventLoop.h"

Mixer *g_Mixer = NULL;

int EZX_SDL_PollEvent(SDL_Event *event) {
	int ret = SDL_PollEvent(event);
	if (!ret)
		return 0;
	if (event->type == SDL_ACTIVEEVENT)
		if (event->active.state == SDL_APPINPUTFOCUS && !event->active.gain) {
			if (g_Mixer)
				g_Mixer->free();
			for (;;) {
				ret = SDL_WaitEvent(event);
				if (!ret)
					continue;
				if (event->type == SDL_QUIT)
					return 1;
				if (event->type != SDL_ACTIVEEVENT)
					continue;
				if (event->active.state == SDL_APPINPUTFOCUS && event->active.gain) {
					if (g_Mixer)
						g_Mixer->init();
					return 1;
				}
			}
		}
	return 1;
}
