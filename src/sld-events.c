#include "../lib/sdl-events.h"

const int camZoom = 10;

int handle_Window_Events(SDL_Event* win_Event, SDL_Rect* camS, int screenW, int screenH)
{
    // 1 if window open, 0 if closed
    int windowOpen = 1;

/*---------------------------- Basic Win Events -------------------------------*/
    switch(win_Event->type)
    {
        case SDL_QUIT:
            return windowOpen = 0; 
            break;
    }

    switch(win_Event->key.keysym.sym)
    {
        case SDLK_ESCAPE:
            return windowOpen = 0;
            break;
    }

/*--------------------- WASD Events -----------------------------------------*/
//TODO: Fix A+W && D+W && A+S && D+S

    //Right && Up/Down
    if((win_Event->key.keysym.sym == SDLK_d) && (win_Event->key.keysym.sym == SDLK_w))
    {
        if (camS->x < screenW) 
        {
            camS->x += 1; 
        }

        if(camS->y > 0)
        {
            camS->y -= 1;
        }
    }

    if((win_Event->key.keysym.sym == SDLK_d) && (win_Event->key.keysym.sym == SDLK_s))
    {
        if (camS->x < screenW) 
        {
            camS->x += 1; 
        }

        if(camS->y < screenH)
        {
            camS->y += 1;
        }
    }
    //Up || Down
    if(win_Event->key.keysym.sym == SDLK_w)
    {
        if(camS->y > 0)
        {
            camS->y -= 1;
        }
    }
    else if(win_Event->key.keysym.sym == SDLK_s)
    {
        if(camS->y < screenH)
        {
            camS->y += 1;
        }
    }

    // Left || Right
    if(win_Event->key.keysym.sym == SDLK_a)
    {
        if(camS->x > 0)
        {
            camS->x -= 1;
        }
    }
    else if(win_Event->key.keysym.sym == SDLK_d)
    {
        if(camS->x < screenW)
        {
            camS->x += 1;
        }
    }

    // Left && Up/Down
    if((win_Event->key.keysym.sym == SDLK_a) && (win_Event->key.keysym.sym == SDLK_w))
    {
        if (camS->x > 0) 
        {
            camS->x -= 1; 
        }

        if(camS->y > 0)
        {
            camS->y -= 1;
        }
    }

    if((win_Event->key.keysym.sym == SDLK_a) && (win_Event->key.keysym.sym == SDLK_s))
    {
        if (camS->x > 0) 
        {
            camS->x -= 1; 
        }

        if(camS->y < screenH)
        {
            camS->y += 1;
        }
    }


/*---------------- Camera Keybindings ------------------------*/
//TODO: Fix camera zoom in and out
    
    // Camera zoom; 1 to zoom in, 2 for out
    if(win_Event->key.keysym.sym == SDLK_1)
    {
        if(/*(camS->w && camS->h) >=*/ 1)
        {
            camS->h /= camZoom;
            camS->w /= camZoom;
        }
    }

    if(win_Event->key.keysym.sym == SDLK_2)
    {
        if((camS->w && camS->h) < (screenW && screenH))
        {
            camS->h *= camZoom;
            camS->w *= camZoom;
        }
    }

    //Camera reset keybind
    if((win_Event->key.keysym.sym == SDLK_LSHIFT) && SDLK_r) 
    {
        camS->x = screenW/2;
        camS->y = screenH/2;
        camS->h = camZoom;
        camS->w = camZoom;
    }


    return windowOpen;
}
