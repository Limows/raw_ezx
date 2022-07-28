#ifndef EZXEVENTLOOP_H
#define EZXEVENTLOOP_H

/*
 * HACK: Use this overloaded SDL function for implementing hide and
 * show routines while SMS, Calls, etc. on Motorola EZX platform.
 */

#include <SDL/SDL.h>

extern int EZX_SDL_PollEvent(SDL_Event *event);

#endif // EZXEVENTLOOP_H
