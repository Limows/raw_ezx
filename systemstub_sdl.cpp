
/*
 * Another World engine rewrite
 * Copyright (C) 2004-2005 Gregory Montoir (cyx@users.sourceforge.net)
 */

#include <SDL.h>
#include "graphics.h"
#include "systemstub.h"
#include "util.h"

struct SystemStub_SDL : SystemStub {

	static const int kJoystickIndex = 0;
	static const int kJoystickCommitValue = 16384;
	static const float kAspectRatio;

	int _w, _h;
	float _aspectRatio[4];
//	SDL_Window *_window;
//	SDL_Renderer *_renderer;
//	SDL_GLContext _glcontext;

	SDL_Surface *_surface;
	SDL_Surface *_sclscreen;
//	uint8_t *_offscreen;
//	uint16_t _pal[16];

	int _texW, _texH;
//	SDL_Texture *_texture;
	SDL_Joystick *_joystick;
//	SDL_GameController *_controller;
	int _screenshot;

	SystemStub_SDL();
	virtual ~SystemStub_SDL() {}

	virtual void init(const char *title, const DisplayMode *dm);
	virtual void fini();

	virtual void prepareScreen(int &w, int &h, float ar[4]);
	virtual void updateScreen();
	virtual void setScreenPixels555(const uint16_t *data, int w, int h);

	virtual void processEvents();
	virtual void sleep(uint32_t duration);
	virtual uint32_t getTimeStamp();

	void setAspectRatio(int w, int h);

	void Stretch240(void);
};

const float SystemStub_SDL::kAspectRatio = 16.f / 10.f;

SystemStub_SDL::SystemStub_SDL()
	: _w(0), _h(0), _texW(0), _texH(0), _surface(0), _sclscreen(0) {
}

int raa(int min, int max) {
	return min + (rand() % static_cast<int>(max - min + 1));
}

void SystemStub_SDL::init(const char *title, const DisplayMode *dm) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);
	SDL_ShowCursor(SDL_DISABLE);
	// SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	int windowW = 0;
	int windowH = 0;
	int flags = SDL_HWSURFACE;
	windowW = dm->width;
	windowH = dm->height;
	if (dm->mode == DisplayMode::WINDOWED) {
		;
	} else {
		flags |= SDL_FULLSCREEN;
	}

	SDL_WM_SetCaption(title, NULL);

	static const int kBitDepth = 16;
	_surface = SDL_SetVideoMode(windowW, windowH, kBitDepth, flags);
	if (!_surface) {
		error("SystemStub_SDL::prepareGraphics() Unable to allocate _screen buffer");
	}
	_sclscreen = SDL_CreateRGBSurface(SDL_SWSURFACE, windowW, windowH, kBitDepth,
						_surface->format->Rmask,
						_surface->format->Gmask,
						_surface->format->Bmask,
						_surface->format->Amask);
	if (!_sclscreen) {
		error("SDLStub::prepareGfxMode() unable to allocate _sclscreen buffer");
	}

	_aspectRatio[0] = _aspectRatio[1] = 0.;
	_aspectRatio[2] = _aspectRatio[3] = 1.;
	_joystick = 0;
//	_controller = 0;
	if (SDL_NumJoysticks() > 0) {

#if SDL_COMPILEDVERSION >= SDL_VERSIONNUM(2,0,2)
		SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
#endif

//		if (SDL_IsGameController(kJoystickIndex)) {
//			_controller = SDL_GameControllerOpen(kJoystickIndex);
//		}
//		if (!_controller) {
			_joystick = SDL_JoystickOpen(kJoystickIndex);
//		}
	}
	_screenshot = 1;
	_dm = *dm;
}

void SystemStub_SDL::fini() {
//	if (_texture) {
//		SDL_DestroyTexture(_texture);
//		_texture = 0;
//	}
	if (_joystick) {
		SDL_JoystickClose(_joystick);
		_joystick = 0;
	}
//	if (_controller) {
//		SDL_GameControllerClose(_controller);
//		_controller = 0;
//	}
//	if (_renderer) {
//		SDL_DestroyRenderer(_renderer);
//		_renderer = 0;
//	}
//	if (_glcontext) {
//		SDL_GL_DeleteContext(_glcontext);
//		_glcontext = 0;
//	}
//	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void SystemStub_SDL::prepareScreen(int &w, int &h, float ar[4]) {
	w = _w;
	h = _h;
	ar[0] = _aspectRatio[0];
	ar[1] = _aspectRatio[1];
	ar[2] = _aspectRatio[2];
	ar[3] = _aspectRatio[3];
}

void SystemStub_SDL::updateScreen() {
	SDL_UpdateRect(_surface, 0, 0, 0, 0);
}

void SystemStub_SDL::setScreenPixels555(const uint16_t *data, int w, int h) {
//	SDL_Rect r;
//	r.w = w;
//	r.h = h;
//	if (w != _texW && h != _texH) {
//		r.x = (_texW - w) / 2;
//		r.y = (_texH - h) / 2;
//	} else {
//		r.x = 0;
//		r.y = 0;
//	}

//	int x = 0;
//	int y = 0;

//	int pitch = 600;

//	uint8_t *buf = (uint8_t *) data;
//	buf += y * pitch + x;
//	uint16_t *p = (uint16_t *) _offscreen;
//	while (h--) {
//		for (int i = 0; i < w / 2; ++i) {
//			uint8_t p1 = *(buf + i) >> 4;
//			uint8_t p2 = *(buf + i) & 0xF;
//			*(p + i * 2 + 0) = _pal[p1];
//			*(p + i * 2 + 1) = _pal[p2];
//		}
//		p += 320;
//		buf += pitch;
//	}
	SDL_LockSurface(_sclscreen);

	uint16_t *dst = (uint16_t *) _sclscreen->pixels;
	uint16_t dstPitch = _sclscreen->pitch;
	uint16_t srcPitch = w;

	// Convert RGB555 to RGB666
	#define RGB555_TO_RGB565(c)	(c = ( ((c & 0x7FE0) << 1) | (c & 0x1F)))
	uint16_t *src = (uint16_t *) data;
	for (int i = 0; i < h * w; ++i)
		src[i] = RGB555_TO_RGB565(src[i]);

	dstPitch >>= 1;
	while (h--) {
		memcpy(dst, src, w * 2);
		dst += dstPitch;
		src += dstPitch;
	}

	SDL_UnlockSurface(_sclscreen);

	SDL_Rect dest;
	SDL_BlitSurface(_sclscreen, NULL, _surface, &dest);
	Stretch240();

	SDL_UpdateRect(_surface, 0, 0, 0, 0);
}

void SystemStub_SDL::processEvents() {
	SDL_Event ev;
	while(SDL_PollEvent(&ev)) {
		switch (ev.type) {
		case SDL_QUIT:
			_pi.quit = true;
			break;
		case SDL_KEYUP:
			switch (ev.key.keysym.sym) {
			case SDLK_LEFT:
				_pi.dirMask &= ~PlayerInput::DIR_LEFT;
				break;
			case SDLK_RIGHT:
				_pi.dirMask &= ~PlayerInput::DIR_RIGHT;
				break;
			case SDLK_UP:
				_pi.dirMask &= ~PlayerInput::DIR_UP;
				break;
			case SDLK_DOWN:
				_pi.dirMask &= ~PlayerInput::DIR_DOWN;
				break;
			case SDLK_SPACE:
			case SDLK_RETURN:
				_pi.action = false;
				break;
			case SDLK_LSHIFT:
			case SDLK_RSHIFT:
				_pi.jump = false;
				break;
			case SDLK_s:
				_pi.screenshot = true;
				break;
			case SDLK_c:
				_pi.code = true;
				break;
			case SDLK_p:
				_pi.pause = true;
				break;
			case SDLK_ESCAPE:
				_pi.back = true;
				break;
			case SDLK_q:
				_pi.quit = true;
				break;
			default:
				break;
			}
			break;
		case SDL_KEYDOWN:
			if (ev.key.keysym.mod & KMOD_ALT) {
				if (ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_KP_ENTER) {
				} else if (ev.key.keysym.sym == SDLK_x) {
					_pi.quit = true;
				}
				break;
			} else if (ev.key.keysym.mod & KMOD_CTRL) {
				if (ev.key.keysym.sym == SDLK_f) {
					_pi.fastMode = true;
				}
				break;
			}
			_pi.lastChar = ev.key.keysym.sym;
			switch(ev.key.keysym.sym) {
			case SDLK_LEFT:
				_pi.dirMask |= PlayerInput::DIR_LEFT;
				break;
			case SDLK_RIGHT:
				_pi.dirMask |= PlayerInput::DIR_RIGHT;
				break;
			case SDLK_UP:
				_pi.dirMask |= PlayerInput::DIR_UP;
				break;
			case SDLK_DOWN:
				_pi.dirMask |= PlayerInput::DIR_DOWN;
				break;
			case SDLK_SPACE:
			case SDLK_RETURN:
				_pi.action = true;
				break;
			case SDLK_LSHIFT:
			case SDLK_RSHIFT:
				_pi.jump = true;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}

void SystemStub_SDL::sleep(uint32_t duration) {
	SDL_Delay(duration);
}

uint32_t SystemStub_SDL::getTimeStamp() {
	return SDL_GetTicks();
}

void SystemStub_SDL::setAspectRatio(int w, int h) {
	const float currentAspectRatio = w / (float)h;
	if (int(currentAspectRatio * 100) == int(kAspectRatio * 100)) {
		_aspectRatio[0] = 0.f;
		_aspectRatio[1] = 0.f;
		_aspectRatio[2] = 1.f;
		_aspectRatio[3] = 1.f;
		return;
	}
	// pillar box
	if (currentAspectRatio > kAspectRatio) {
		const float inset = 1.f - kAspectRatio / currentAspectRatio;
		_aspectRatio[0] = inset / 2;
		_aspectRatio[1] = 0.f;
		_aspectRatio[2] = 1.f - inset;
		_aspectRatio[3] = 1.f;
		return;
	}
	// letter box
	if (currentAspectRatio < kAspectRatio) {
		const float inset = 1.f - currentAspectRatio / kAspectRatio;
		_aspectRatio[0] = 0.f;
		_aspectRatio[1] = inset / 2;
		_aspectRatio[2] = 1.f;
		_aspectRatio[3] = 1.f - inset;
		return;
	}
}

void SystemStub_SDL::Stretch240(void) {
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 320;
	src.h = 200;

	dest.x = 0;
	dest.y = 0;
	dest.w = 320;
	dest.h = 240;

	SDL_SoftStretch(_sclscreen, &src, _surface, &dest);
}

SystemStub *SystemStub_SDL_create() {
	return new SystemStub_SDL();
}
