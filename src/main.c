#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#define WINDOW_W 640
#define WINDOW_H 480
#define WINDOW_NAME "Cellular Automata"


SDL_Window *main_Window = NULL;
SDL_Renderer *main_Renderer = NULL;
int func_SDL_Init();

void func_SDL_Exit(void);

void func_Engine_Draw_Cells(void);

int main(int argv, char *argc[])
{
    func_SDL_Init();

    //main loop
    int windowOpen = 1;
    while(windowOpen != 0)
    {
        SDL_Event windowEvent;
        while(SDL_PollEvent(&windowEvent))
        {
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
            }
        }

        //Set BG color
        SDL_SetRenderDrawColor(main_Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(main_Renderer);

        SDL_RenderPresent(main_Renderer);
    }

    func_SDL_Exit();
    return 0;
}

int func_SDL_Init()
{

    if(SDL_Init(SDL_INIT_EVERYTHING) > 0)
    {
        printf("Error initalizing SDL2. ERROR: %s", SDL_GetError());
        return 1;
    }

    main_Window = SDL_CreateWindow(
            "Window", 
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 
            640, 480, 
            SDL_WINDOW_RESIZABLE);
    if(!main_Window)
    {
        printf("Error initalizing Window. ERROR: %s", SDL_GetError());
        return 1;
    }

    main_Renderer = SDL_CreateRenderer(main_Window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(!main_Renderer)
    {
        printf("Error initalizing Renderer. ERROR: %s", SDL_GetError());
        return 1;
    }

    return 0;
}


void func_SDL_Exit(void)
{
    SDL_DestroyRenderer(main_Renderer);
    SDL_DestroyWindow(main_Window);
    SDL_Quit();
}

void func_Engine_Draw_Cells(void)
{

}
