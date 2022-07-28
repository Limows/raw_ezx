/*
 * HACK: Use this overloaded SDL function for implementing hide and
 * show routines while SMS, Calls, etc. on Motorola EZX platform.
 */

#include "EzxEventLoop.h"

int EZX_SDL_PollEvent(SDL_Event *event) {
	int ret = SDL_PollEvent(event);
	if (!ret)
		return 0;
	if (event->type == SDL_ACTIVEEVENT)
		if (event->active.state == SDL_APPINPUTFOCUS && !event->active.gain) {
			SDL_PauseAudio(1);
			for (;;) {
				ret = SDL_WaitEvent(event);
				if (!ret)
					continue;
				if (event->type == SDL_QUIT)
					return 1;
				if (event->type != SDL_ACTIVEEVENT)
					continue;
				if (event->active.state == SDL_APPINPUTFOCUS && event->active.gain) {
					SDL_PauseAudio(0);
					return 1;
				}
			}
		}
	return 1;
}
