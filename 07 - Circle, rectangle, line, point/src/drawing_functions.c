#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

void DrawFilledCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius, Uint8 r, Uint8 g, Uint8 b);
void DrawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius, Uint8 r, Uint8 g, Uint8 b);


/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Drawing Functions", "1.0", "com.example.renderer-drawing-functions");

    // SDL init function
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        // Error message
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Drawing Functions", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        // Error message
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());

        return SDL_APP_FAILURE;
    }

    // Set a device-independent resolution and presentation mode for rendering
    SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    // Carry on with the program!
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    // Quit event (like closing the window or Alt+F4)?
    if (event->type == SDL_EVENT_QUIT)
    {
        // Yes

        // End the program, reporting success to the OS.
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_FRect rect;
    SDL_FPoint points[3];

    /* Select black color */
    SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

    /* Clear the window to the draw color. */
    SDL_RenderClear(renderer);

    /* Filled circle */
    DrawFilledCircle(renderer,640 >> 1,480 >> 1,50,0,128,128);

    /* Unfilled circle */
    DrawCircle(renderer, 123, 80, 17, 25, 198, 245);

    /* Filled rectangle */
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);  /* blue, full alpha */
    rect.x = rect.y = 100;
    rect.w = 150;
    rect.h = 80;
    SDL_RenderFillRect(renderer, &rect);

    /* Draw an unfilled rectangle in-set a little bit. */
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);  /* green, full alpha */
    rect.x = 300;
    rect.y = 30;
    rect.w = 90;
    rect.h = 20;
    SDL_RenderRect(renderer, &rect);

    /* 1 line */
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);  /* yellow, full alpha */
    SDL_RenderLine(renderer, 360, 400, 490, 450);

    /* 1 point */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
    SDL_RenderPoint(renderer, 500, 100);

    /* 3 points with the same color */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
    points[0].x = 101;
    points[0].y = 301;
    points[1].x = 103;
    points[1].y = 302;
    points[2].x = 108;
    points[2].y = 307;
    SDL_RenderPoints(renderer, points, SDL_arraysize(points));

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}

/********************************************************************************************************* */

/* Draw a filled circle */
void DrawFilledCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius, Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    for (int y = centerY - radius; y <= centerY + radius; y++) {
        for (int x = centerX - radius; x <= centerX + radius; x++) {
            if ((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius) {
                SDL_RenderPoint(renderer, x, y);
            }
        }
    }
}

/* Draw an unfilled circle */
void DrawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius, Uint8 r, Uint8 g, Uint8 b) {
    int x = radius - 1;
    int y = 0;
    int error = 2 - 2 * radius;

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    while (x >= y) {
        // Draw all 8 octants
        SDL_RenderPoint(renderer, centerX + x, centerY - y);
        SDL_RenderPoint(renderer, centerX + x, centerY + y);
        SDL_RenderPoint(renderer, centerX - x, centerY - y);
        SDL_RenderPoint(renderer, centerX - x, centerY + y);
        SDL_RenderPoint(renderer, centerX + y, centerY - x);
        SDL_RenderPoint(renderer, centerX + y, centerY + x);
        SDL_RenderPoint(renderer, centerX - y, centerY - x);
        SDL_RenderPoint(renderer, centerX - y, centerY + x);

        if (error <= 0) {
            ++y;
            error += 2 * y + 1;
        }
        if (error > 0) {
            --x;
            error -= 2 * x + 1;
        }
    }
}