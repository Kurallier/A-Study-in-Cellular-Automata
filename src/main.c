#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <stdint.h>
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
#include <SDL2/SDL_ttf.h>

#include "../lib/conwayEngine.h"
#include "../lib/sdl-events.h"

//Setting defualt window values
const char WINDOW_NAME[] = "A Study in Cellular Automata";
//The canvas is where the game(s) will take place, and is the size of the texture
const size_t CANVAS_W = 1240;
const size_t CANVAS_H = 1080;
//The screen width and height are for the camera; what is to be shown to the user
const size_t SCREEN_W = 1240;
const size_t SCREEN_H = 1080;
//Some cam stuff
size_t CAM_X = CANVAS_W / 2;
size_t CAM_Y = CANVAS_H / 2;
const size_t CAM_MAGNI = 4;
//An array that holds relevant RGBA colors
const size_t BACKGROUND_COLOR[] = {41, 41, 48, 1};
const size_t CELL_COLOR[] = {239, 107, 31, 1};
//Creating pointers for SDL
SDL_Window* m_Window;
SDL_Renderer* m_Renderer;
SDL_Texture* m_Texture;
SDL_Surface* m_Surface;
TTF_Font* m_Font;

const char FONT_PATH[] = "../assets/KarmaSuture.ttf";
SDL_Color FontColor = {255,255,255,255};
SDL_Rect rectText = {10, 10, 150, 25};


//SDL cleanup
void SDL_Exit();
//Used to prepare the camera for drawing
int SDL_Render_Camera(SDL_Rect*, SDL_FRect*, SDL_Renderer*, SDL_Texture*);

int internal_TTF_Cell_Counter(Automata**);

int internal_SDL_Render_Camera_Resize(SDL_Rect* camS, SDL_FRect* camPresF, int screenW, int screenH, SDL_Window*);

int internal_SDL_Init();

void Conways_Game_Of_Life_Running(Automata** Cell_Matrix, int canvas_W, int canvas_H, SDL_Renderer* m_renderer, SDL_Texture* m_texture);

int main(int argv, char *argc[])
{
    internal_SDL_Init();


/*--------------------------------- Font Texure --------------------------------*/



    //TODO: Window resizaeability 

    //TODO: Make the camera it's own structure/object. Should have both the source and present, xVel yVel, zoomVel
    
    //Divide screen by 32 to zoom(stretch) texture 
    //Camera PlayerCam;
    /*
    PlayerCam.screenW = SCREEN_W;
    PlayerCam.screenH = SCREEN_H;

    PlayerCam.camS.x = SCREEN_W/2;
    PlayerCam.camS.y = SCREEN_H/2;
    PlayerCam.camS.w = (double)SCREEN_W/camMagni;
    PlayerCam.camS.h = (double)SCREEN_H/camMagni;

    //The offsets create a border around the window
    PlayerCam.camS.x = 10;
    PlayerCam.camS.y = 10;
    PlayerCam.camS.w = SCREEN_W - 20;
    PlayerCam.camS.h = SCREEN_H - 20;
    */

    SDL_Rect camSource = {CAM_X, CAM_Y, (double)SCREEN_W/CAM_MAGNI, (double)SCREEN_H/CAM_MAGNI};
    //The offsets create a border around the window
    SDL_FRect camPresentF = { 5, 5, SCREEN_W-10, SCREEN_H-10};



    Automata** ConwayPixels = conway_Automata_Matrix_Init(CANVAS_W, CANVAS_H, CELL_COLOR[0], CELL_COLOR[1], CELL_COLOR[2], 255);
    Automata** ConwayPixels_B = conway_Automata_Matrix_Init(CANVAS_W, CANVAS_H, CELL_COLOR[0], CELL_COLOR[1], CELL_COLOR[2], 255);
    //Randomize if pixels should be alive or not
    conway_Automata_Matrix_Seed(ConwayPixels, CANVAS_W, CANVAS_H, (SCREEN_W*SCREEN_H)/18);

    //Main loop
    int windowOpen = 1;
    while(windowOpen != 0)
    {
        SDL_Event windowEvent;
        while(SDL_PollEvent(&windowEvent))
        {
            windowOpen = handle_Window_Events(windowEvent, &camSource, SCREEN_W, SCREEN_H);

            //handle_Keyboard_Events(windowEvent, &camSource, SCREEN_W, SCREEN_H, CANVAS_W, CANVAS_H);

            if(windowEvent.key.keysym.sym == SDLK_r)
            {
                for(size_t i = 0; i < CANVAS_W; i++)
                {
                    for(size_t j = 0; j < CANVAS_H; j++)
                    {
                        ConwayPixels[i][j].state = CELL_DEAD;
                    }
                }
                conway_Automata_Matrix_Seed(ConwayPixels, CANVAS_W, CANVAS_H, (SCREEN_W*SCREEN_H)/20);
                SDL_Delay(150);
            }

            /*
            printf("camS X:%d Y:%d\n", camSource.x, camSource.y);
            printf("Boudry X:%d ",(int)(CANVAS_W));
            printf("Boudry Y:%d\n",(int)(CANVAS_H));
            if (CAM_X < 0) {
                CAM_X = 0;
            }
            if (CAM_Y < 0) {
                CAM_Y = 0;
            }
            if (CAM_X > CANVAS_W - SCREEN_W) {
                CAM_X = CANVAS_W - SCREEN_W;
            }
            if (CAM_Y > CANVAS_H - SCREEN_H) {
                CAM_Y = CANVAS_H - SCREEN_H;
            }

            // Update the source rectangle
            camSource.x = CAM_X + (CANVAS_W - SCREEN_W)/2;
            camSource.y = CAM_Y + (CANVAS_H - SCREEN_H)/2;
            */

            handle_Mouse_Events(windowEvent, &camSource, SCREEN_W, SCREEN_H, CANVAS_W, CANVAS_H);
        }
        //Wipe the texture and renderer white
        SDL_SetRenderTarget(m_Renderer, m_Texture);
        SDL_SetRenderDrawColor(m_Renderer, BACKGROUND_COLOR[0],BACKGROUND_COLOR[1],BACKGROUND_COLOR[2], SDL_ALPHA_OPAQUE);
        SDL_RenderClear(m_Renderer);

        conway_Generation_Next(CANVAS_W, CANVAS_H, ConwayPixels, ConwayPixels_B);

        //Draw the Automata Matrix
        SDL_Render_Emplace_Automata_Matrix(ConwayPixels, CANVAS_W, CANVAS_H, m_Renderer, m_Texture);

        //conway_Generation_Next_Threaded(CANVAS_W, CANVAS_H, ConwayPixels, ConwayPixels_B);

        //Conways_Game_Of_Life_Running(ConwayPixels, CANVAS_W, CANVAS_H, m_Renderer, m_Texture);

        //Prepare camera
        //This breaks the camera movement
        //internal_SDL_Render_Camera_Resize(&camSource, &camPresentF, SCREEN_W, SCREEN_H, m_Window);
        

        SDL_Render_Camera(&camSource, &camPresentF, m_Renderer, m_Texture);

        internal_TTF_Cell_Counter(ConwayPixels);
        

        //Final draw call
        SDL_RenderPresent(m_Renderer);

        //Hacked 60fps
        SDL_Delay(64);
    }

    //Destroys the matricies
    conway_Automata_Matrix_Destroy(ConwayPixels, CANVAS_W, CANVAS_H);
    conway_Automata_Matrix_Destroy(ConwayPixels_B, CANVAS_W, CANVAS_H);

    SDL_Exit();
    return 0;
}

int internal_TTF_Cell_Counter(Automata** matrix)
{
    int count = 0;

    for(size_t i = 0; i < CANVAS_W; i++)
    {
        for(size_t j = 0; j < CANVAS_H; j++)
        {
            if(matrix[i][j].state == CELL_ALIVE)
            {
                count++;
            }
        }
    }

    char intStr[12];
    char text[100] = "Alive Cells: ";

    SDL_itoa(count, intStr, 10);
    SDL_strlcat(text, intStr, 100);

    SDL_Surface* surfaceText = TTF_RenderText_Solid(m_Font, text, FontColor);

    SDL_Texture* textureText = SDL_CreateTextureFromSurface(m_Renderer, surfaceText);

    SDL_FreeSurface(surfaceText);

    SDL_RenderCopy(m_Renderer, textureText, NULL, &rectText);

    return 0;
}


int internal_SDL_Init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("FAILED TO INIT: %s\n", SDL_GetError());
        exit(1);
    }

    if(TTF_Init() != 0)
    {
        printf("FAILED TO INIT TTF: %s\n", TTF_GetError());
        exit(1);
    }

    m_Window = SDL_CreateWindow(
            WINDOW_NAME, 
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_W, SCREEN_H, 
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_ALLOW_HIGHDPI);

    m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);

    m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, CANVAS_W, CANVAS_H);


    m_Font = TTF_OpenFont(FONT_PATH, 32);
    if(!m_Font)
    {
        printf("Failed to load font");
        exit(1);
    }

    return 0;
}

void SDL_Exit()
{
    SDL_DestroyWindow(m_Window);
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyTexture(m_Texture);
    for(size_t i = 0; i <= TTF_WasInit(); i++)
        TTF_Quit();
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
