#include "../lib/sdl-events.h"
#include <SDL2/SDL_events.h>

const int camZoom = 10;

int handle_Window_Events(SDL_Event win_Event, SDL_Rect* camS, int screenW, int screenH)
{
    // 1 if window open, 0 if closed
    int windowOpen = 1;

/*---------------------------- Basic Win Events -------------------------------*/
    switch(win_Event.type)
    {
        case SDL_QUIT:
            return windowOpen = 0; 
            break;
    }

    switch(win_Event.key.keysym.sym)
    {
        case SDLK_ESCAPE:
            return windowOpen = 0;
            break;
    }

/*--------------------- WASD Events -----------------------------------------*/
//TODO: Fix A+W && D+W && A+S && D+S
    
    switch (win_Event.type) 
    {
        case SDL_KEYDOWN:         
            break;
    }

    //Right && Up/Down
    /*
    if((win_Event.key.keysym.sym == SDLK_d) && (win_Event.key.keysym.sym == SDLK_w))
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

    if((win_Event.key.keysym.sym == SDLK_d) && (win_Event.key.keysym.sym == SDLK_s))
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
    */
    //Up || Down
    if(win_Event.key.keysym.sym == SDLK_w)
    {
        if(camS->y > 0)
        {
            camS->y -= 1;
        }
    }
    else if(win_Event.key.keysym.sym == SDLK_s)
    {
        if(camS->y < screenH)
        {
            camS->y += 1;
        }
    }

    // Left || Right
    if(win_Event.key.keysym.sym == SDLK_a)
    {
        if(camS->x > 0)
        {
            camS->x -= 1;
        }
    }
    /*Streches the texture a little bit, I don't know why for certain.
     *I'm going to assume it has to do with the cameraPresent offset
     *Not bad enought to warrant a fix for now
     */
    else if(win_Event.key.keysym.sym == SDLK_d)
    {
        if(camS->x < screenW)
        {
            camS->x += 1;
        }
    }

    // Left && Up/Down
    /*
    if((win_Event.key.keysym.sym == SDLK_a) && (win_Event.key.keysym.sym == SDLK_w))
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
    

    if((win_Event.key.keysym.sym == SDLK_a) && (win_Event.key.keysym.sym == SDLK_s))
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
    */


/*---------------- Camera Keybindings ------------------------*/
//TODO: Fix camera zoom in and out
    
    // Camera zoom; 1 to zoom in, 2 for out
    if(win_Event.key.keysym.sym == SDLK_1)
    {
        if((camS->w > screenW/32) && (camS->h > screenH/32))
        {
            camS->w /= 1.1;
            camS->h /= 1.1;
            //if(camS->h > screenH/32)
        }
    }

    if(win_Event.key.keysym.sym == SDLK_2)
    {
        if(camS->w * camS->h < screenW * screenH)
        {
            camS->w *= 1.1;
            camS->h *= 1.1;
        }
        /*
        if(camS->w  < screenW)
        {
            if(camS->h < screenH)
            {
                camS->h *= 1.1;
                camS->w *= 1.1;
            }
        }
        */
    }

    //Camera reset keybind
    if((win_Event.key.keysym.sym == SDLK_LSHIFT) && SDLK_r) 
    {
        camS->x = screenW/2;
        camS->y = screenH/2;
        camS->h = camZoom;
        camS->w = camZoom;
    }


    return windowOpen;
}

/*
//TODO, change to if statments
switch(windowEvent.type)
{
case SDL_QUIT:
windowOpen = 0;
break;
}

switch(windowEvent.key.keysym.sym)
{
case SDLK_ESCAPE:
windowOpen = 0;
break;
//To move Camera
//W is up, S is down, A is left, and D is right
case SDLK_w:
if(camSource.y > 0)
{
camSource.y -= 1;
}
break;
case SDLK_s:
if(camSource.y < CANVAS_H)
{
camSource.y += 1;
}
break;
case SDLK_a:
if(camSource.x > 0)
{
camSource.x -= 1;
}
break;
case SDLK_d:
if(camSource.x < CANVAS_W)
{
camSource.x += 1;
}
break;
//To reset camera
case SDLK_LSHIFT + SDLK_r:
camSource.x = SCREEN_W/2;
camSource.y = SCREEN_H/2;
camSource.w = (double)SCREEN_W/32;
camSource.h = (double)SCREEN_H/32;
break;
//To zoom in and out
// 1 is zoom out
// 2 is zoom in
case SDLK_1:
if(camSource.w * camSource.h < CANVAS_W * CANVAS_H)
{
camSource.w *= 1.1;
camSource.h *= 1.1;
}
break;
//TODO: WHY CAMERA BREAK???
case SDLK_2:
camSource.w /= 1.1;
camSource.h /= 1.1;
break;
}
*/
