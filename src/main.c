#include <SDL2/SDL.h>
#include <unistd.h>
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
#include "../lib/sdl-events.h"

//Setting defualt window values
const char WINDOW_NAME[] = "A Study in Cellular Automata";
//The canvas is where the game(s) will take place, and is the size of the texture
const size_t CANVAS_W = 2500;
const size_t CANVAS_H = 2500;
//The screen width and height are for the camera; what is to be shown to the user
const size_t SCREEN_W = 1240;
const size_t SCREEN_H = 1080;
//An array that holds relevant RGBA colors
const size_t BACKGROUND_COLOR[] = {41, 41, 48, 1};
const size_t CELL_COLOR[] = {239, 107, 31, 1};
//Creating pointers for SDL
SDL_Window* m_Window;
SDL_Renderer* m_Renderer;
SDL_Texture* m_Texture;

//SDL cleanup
void SDL_Exit(SDL_Window* main_Window, SDL_Renderer*, SDL_Texture*);
//Used to prepare the camera for drawing
int SDL_Render_Camera(SDL_Rect*, SDL_FRect*, SDL_Renderer*, SDL_Texture*);

int internal_SDL_Render_Camera_Resize(SDL_Rect* camS, SDL_FRect* camPresF, int screenW, int screenH, SDL_Window*);

int internal_SDL_Init(SDL_Window* m_Window, SDL_Renderer* m_Renderer, SDL_Texture* m_Texture);

void Conways_Game_Of_Life_Running(Automata** Cell_Matrix, int canvas_W, int canvas_H, SDL_Renderer* m_renderer, SDL_Texture* m_texture);

int main(int argv, char *argc[])
{

    /*
     
     
     
    //internal_SDL_Init(m_Window, m_Renderer, m_Texture);

    //TODO: Window resizaeability 
    m_Window = SDL_CreateWindow(
            WINDOW_NAME, 
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_W, SCREEN_H, 
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_ALLOW_HIGHDPI);

    m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);

    m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, CANVAS_W, CANVAS_H);

    //TODO: Make the camera it's own structure/object. Should have both the source and present, xVel yVel, zoomVel
    //Divide screen by 32 to zoom(stretch) texture 
    SDL_Rect camSource = {SCREEN_W/2, SCREEN_H/2, (double)SCREEN_W/10, (double)SCREEN_H/10};
    //The offsets create a border around the window
    SDL_FRect camPresentF = { 10, 10, SCREEN_W-20, SCREEN_H-20};



    */

    Automata** ConwayPixels = conway_Automata_Matrix_Init(CANVAS_W, CANVAS_H, CELL_COLOR[0], CELL_COLOR[1], CELL_COLOR[2], 255);
    Automata** ConwayPixels_B = conway_Automata_Matrix_Init(CANVAS_W, CANVAS_H, CELL_COLOR[0], CELL_COLOR[1], CELL_COLOR[2], 255);
    //Randomize if pixels should be alive or not
    conway_Automata_Matrix_Seed(ConwayPixels, CANVAS_W, CANVAS_H, (SCREEN_W*SCREEN_H)/6);

    //Main loop
    int windowOpen = 1;
    while(windowOpen != 0)
    {
        /*
         *
        SDL_Event windowEvent;
        while(SDL_PollEvent(&windowEvent))
        {
            windowOpen = handle_Window_Events(windowEvent, &camSource, SCREEN_W, SCREEN_H);

            handle_Keyboard_Events(windowEvent, &camSource, SCREEN_W, SCREEN_H);

            handle_Mouse_Events(windowEvent, &camSource, SCREEN_W, SCREEN_H);
        }
        //Wipe the texture and renderer white
        SDL_SetRenderTarget(m_Renderer, m_Texture);
        SDL_SetRenderDrawColor(m_Renderer, BACKGROUND_COLOR[0],BACKGROUND_COLOR[1],BACKGROUND_COLOR[2], SDL_ALPHA_OPAQUE);
        SDL_RenderClear(m_Renderer);

        //Draw the Automata Matrix
        SDL_Render_Emplace_Automata_Matrix(ConwayPixels, CANVAS_W, CANVAS_H, m_Renderer, m_Texture);


        */

        //conway_Generation_Next(CANVAS_W, CANVAS_H, ConwayPixels, ConwayPixels_B);
        conway_Generation_Next_Threaded(CANVAS_W, CANVAS_H, ConwayPixels, ConwayPixels_B);

        //Conways_Game_Of_Life_Running(ConwayPixels, CANVAS_W, CANVAS_H, m_Renderer, m_Texture);

        //Prepare camera
        //This breaks the camera movement
        //internal_SDL_Render_Camera_Resize(&camSource, &camPresentF, SCREEN_W, SCREEN_H, m_Window);
        
        /*
        SDL_Render_Camera(&camSource, &camPresentF, m_Renderer, m_Texture);

        //Final draw call
        SDL_RenderPresent(m_Renderer);

        //Hacked 120fps
        SDL_Delay(8);
        */
        sleep(3);
        windowOpen = 0;
    }

    //Destroys the matricies
    conway_Automata_Matrix_Destroy(ConwayPixels, CANVAS_W, CANVAS_H);
    conway_Automata_Matrix_Destroy(ConwayPixels_B, CANVAS_W, CANVAS_H);

    /*
    SDL_Exit(m_Window, m_Renderer, m_Texture);
    */
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

int SDL_Render_Camera(SDL_Rect* camSource, SDL_FRect* camPresentF, SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopyF(renderer, texture, camSource, camPresentF);

    return 0;
}

/*
 * \Not a perfect solution, but will work for now
 * \Doesn't resize the window width for an unkown reason, TBF
 * \Breaks window movement, will fix later
int internal_SDL_Render_Camera_Resize(SDL_Rect* camS, SDL_FRect* camPresF, int screenW, int screenH, SDL_Window* window)
{
    int* screnW = NULL;
    screnW = &screenW;

    int* screnH = NULL;
    screnH = &screenW;

    *
    float* screnW_F = NULL;
    screnW_F = &screenW;

    float* screnH_F = NULL;
    screnH_F = &screenW;
    *

    SDL_GetWindowSize(window, screnW, screnH);

    if((*screnW != SCREEN_W))
    {
        printf("Window Width being resized");
        camS->w = *screnW/12;
        camPresF->w = *screnW-20;

    }
    if((*screnH != SCREEN_H))
    {
        printf("Window Height being resized");
        camS->h = *screnH/12;
        camPresF->h = *screnH-20;

    }

    printf("WINDOW_W: %d | WINDOW_H %d\n", *screnW, *screnH);
    printf("CAMERA_W: %02f | CAMERA_H: %02f \n", (camPresF->w), (camPresF->h));

    return 0;
}
*/

void Conways_Game_Of_Life_Running(Automata** Cell_Matrix, int canvas_W, int canvas_H, SDL_Renderer* m_renderer, SDL_Texture* m_texture)
{
    SDL_Render_Emplace_Automata_Matrix(Cell_Matrix, canvas_W, canvas_H, m_renderer, m_texture);

    //conway_Generation_Next(canvas_W, canvas_H, Cell_Matrix);
}
