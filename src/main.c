#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../lib/conwayEngine.h"

//Setting defualt window values
const char WINDOW_NAME[] = "A Study in Cellular Automata";
//The canvas is where the game(s) will take place, and is the size of the texture
const size_t CANVAS_W = 2500;
const size_t CANVAS_H = 2500;
//The screen width and height are for the camera; what is to be shown to the user
const size_t SCREEN_W = 1240;
const size_t SCREEN_H = 1080;
//Creating pointers for SDL
SDL_Window* m_Window;
SDL_Renderer* m_Renderer;
SDL_Texture* m_Texture;

//SDL cleanup
void SDL_Exit(SDL_Window* main_Window, SDL_Renderer*, SDL_Texture*);
//Used to prepare the camera for drawing
int SDL_Render_Camera(SDL_Rect, SDL_FRect, SDL_Renderer*, SDL_Texture*);

int internal_SDL_Init(SDL_Window* m_Window, SDL_Renderer* m_Renderer, SDL_Texture* m_Texture);

int SDL_Render_Emplace_Automata_Matrix(Automata** AutomataMatrix, int canvas_w, int canvas_h, SDL_Renderer* m_renderer, SDL_Texture* m_texture);

int main(int argv, char *argc[])
{
    srand(time(NULL));

    //internal_SDL_Init(m_Window, m_Renderer, m_Texture);

    m_Window = SDL_CreateWindow(
            WINDOW_NAME, 
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_W, SCREEN_H, 
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_ALLOW_HIGHDPI);

    m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);

    m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, CANVAS_W, CANVAS_H);

    //Divide screen by 32 to zoom(stretch) texture 
    SDL_Rect camSource = {SCREEN_W/2, SCREEN_H/2, (double)SCREEN_W/32, (double)SCREEN_H/32};
    //The offsets create a border around the window
    SDL_FRect camPresentF = { 10, 10, SCREEN_W-20, SCREEN_H-20};

    Automata** ConwayPixels = conway_Automata_Matrix_Init(CANVAS_W, CANVAS_H, 255, 0 , 0, 255);
    //Randomize if pixels should be alive or not
    conway_Automata_Matrix_Seed(ConwayPixels, CANVAS_W, CANVAS_H, 1);

    //Main loop
    int windowOpen = 1;
    while(windowOpen != 0)
    {
        SDL_Event windowEvent;
        while(SDL_PollEvent(&windowEvent))
        {
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
        }
        //Wipe the texture and renderer white
        SDL_SetRenderTarget(m_Renderer, m_Texture);
        SDL_SetRenderDrawColor(m_Renderer, 255,255,255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(m_Renderer);

        //Draw the Automata Matrix
        SDL_Render_Emplace_Automata_Matrix(ConwayPixels, CANVAS_W, CANVAS_H, m_Renderer, m_Texture);

        //Prepare camera
        //func_SDL_Render_Camera(camSource, camPresentF, m_Renderer, m_Texture);
        SDL_SetRenderTarget(m_Renderer, NULL);
        SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_Renderer);
        SDL_RenderCopyF(m_Renderer, m_Texture, &camSource, &camPresentF);

        //Final draw call
        SDL_RenderPresent(m_Renderer);
        
        //Hacked 60fps
        SDL_Delay(8);
    }

    SDL_Exit(m_Window, m_Renderer, m_Texture);
    return 0;
}

int internal_SDL_Init(SDL_Window* m_Window, SDL_Renderer* m_Renderer, SDL_Texture* m_Texture)
{
    m_Window = SDL_CreateWindow(
            WINDOW_NAME, 
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_W, SCREEN_H, 
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_ALLOW_HIGHDPI);

    m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);

    m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, CANVAS_W, CANVAS_H);

    return 0;
}

void SDL_Exit(SDL_Window* main_Window, SDL_Renderer* main_Renderer, SDL_Texture* main_Texture)
{
    SDL_DestroyWindow(main_Window);
    SDL_DestroyRenderer(main_Renderer);
    SDL_DestroyTexture(main_Texture);
    SDL_Quit();
}

int SDL_Render_Camera(SDL_Rect camSource, SDL_FRect camPresentF, SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopyF(renderer, texture, &camSource, &camPresentF);

    return 0;
}


int SDL_Render_Emplace_Automata_Matrix(Automata** AutomataMatrix, int canvas_w, int canvas_h, SDL_Renderer* m_renderer, SDL_Texture* m_texture)
{
    for(size_t i = 0; i < canvas_w; i++)
    {
        for(size_t j = 0; j < canvas_h; j++)
        {
           if(AutomataMatrix[i][j].state == CELL_ALIVE)
           {
               SDL_SetRenderTarget(m_renderer, m_texture);
               SDL_SetRenderDrawColor(m_renderer, AutomataMatrix[i][j].r,AutomataMatrix[i][j].g,AutomataMatrix[i][j].b,AutomataMatrix[i][j].a);
               SDL_RenderDrawPoint(m_renderer, AutomataMatrix[i][j].pos_X, AutomataMatrix[i][j].pos_Y);
           }
        }
    }
    return 0;
}








