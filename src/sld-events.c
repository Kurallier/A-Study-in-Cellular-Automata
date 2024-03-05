#include "../lib/sdl-events.h"
#include "../lib/conwayEngine.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <stdio.h>

const int camZoom = 10;
int windowOpen = 1;

int handle_Window_Events(SDL_Event win_Event, SDL_Rect* camS, int screenW, int screenH)
{
    // 1 if window open, 0 if closed

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
    return windowOpen;
}

int handle_Keyboard_Events(SDL_Event win_Event, SDL_Rect* camS, int screenW, int screenH)
{
    
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
    return 0;
}

int handle_Mouse_Events(SDL_Event win_Event, SDL_Rect* camS, int screenW, int screenH, Automata** matrix)
{
    int mouse_X = win_Event.motion.x;
    int mouse_Y = win_Event.motion.y;
    /*----------------------- Mouse Events --------------------------*/

    if(win_Event.button.state == SDL_PRESSED)
    {
        if(win_Event.button.button == SDL_BUTTON_LEFT)
        {
            printf("Mouse button left detected X:%d, Y:%d\n", mouse_X, mouse_Y);
            printf("%d\n",matrix[mouse_X][mouse_Y].state);
            matrix[mouse_X][mouse_Y].state = CELL_DEAD;
        }

    }



    return 0;
}

