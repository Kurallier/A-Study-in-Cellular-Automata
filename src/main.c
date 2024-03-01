#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

//Setting defualt window values
const char WINDOW_NAME[] = "A Study in Cellular Automata";
//The canvas is where the game(s) will take place, and is the size of the texture
const size_t CANVAS_W = 2000;
const size_t CANVAS_H = 2000;
//The screen width and height are for the camera; what is to be shown to the user
const size_t SCREEN_W = 640;
const size_t SCREEN_H = 480;
//Creating pointers for SDL
SDL_Window* m_Window;
SDL_Renderer* m_Renderer;
SDL_Texture* m_Texture;

//SDL cleanup
void func_SDL_Exit(SDL_Window* main_Window, SDL_Renderer*, SDL_Texture*);
//Used to prepare the camera for drawing
int func_SDL_Render_Camera(SDL_Rect, SDL_FRect, SDL_Renderer*, SDL_Texture*);


int main(int argv, char *argc[])
{
    m_Window = SDL_CreateWindow(
            WINDOW_NAME, 
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_W, SCREEN_H, 
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_ALLOW_HIGHDPI);

    m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);

    m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, CANVAS_W, CANVAS_H);

    //Divide screen by 32 to zoom(stretch) texture 
    SDL_Rect camSource = { 0, 0, SCREEN_W/32, SCREEN_H/32};
    //The offsets create a border around the window
    SDL_FRect camPresentF = { 10, 10, SCREEN_W-20, SCREEN_H-20};


    
    //Main loop
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
                //To move Camera
                //W is up, S is down, A is left, and D is right
                case SDLK_w:
                    camSource.y -= 3;
                    break;
                case SDLK_s:
                    camSource.y += 3;
                    break;
                case SDLK_a:
                    camSource.x -= 3;
                    break;
                case SDLK_d:
                    camSource.x += 3;
                    break;
                //To zoom in and out
                // 1 is zoom in
                // 2 is zoom out
                case SDLK_1:
                    camSource.w *= 2;
                    camSource.h *= 2;
                    break;
                case SDLK_2:
                    camSource.w /= 2;
                    camSource.h /= 2;
                    break;
            }
        }

        //Wipe the texture and renderer white
        SDL_SetRenderTarget(m_Renderer, m_Texture);
        SDL_SetRenderDrawColor(m_Renderer, 255,255,255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(m_Renderer);
        //Prepare camera
        func_SDL_Render_Camera(camSource, camPresentF, m_Renderer, m_Texture);

        //Final draw call
        SDL_RenderPresent(m_Renderer);

    }

    func_SDL_Exit(m_Window, m_Renderer, m_Texture);
    return 0;
}


void func_SDL_Exit(SDL_Window* main_Window, SDL_Renderer* main_Renderer, SDL_Texture* main_Texture)
{
    SDL_DestroyWindow(main_Window);
    SDL_DestroyRenderer(main_Renderer);
    SDL_DestroyTexture(main_Texture);
    SDL_Quit();
}

int func_SDL_Render_Camera(SDL_Rect camSource, SDL_FRect camPresentF, SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopyF(renderer, texture, &camSource, &camPresentF);

    return 0;
}

