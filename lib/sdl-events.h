#ifndef SDL_EVENTS_H
#define SDL_EVENTS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>
#include "conwayEngine.h"

int handle_Window_Events(SDL_Event win_Event, SDL_Rect* camS, int screenW, int screenH);

int handle_Keyboard_Events(SDL_Event win_Event, SDL_Rect* camS, int screenW, int screenH);

int handle_Mouse_Events(SDL_Event win_Event, SDL_Rect* camS, int screenW, int screenH, Automata** matrix);
#endif
