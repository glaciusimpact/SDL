#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480


/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static SDL_Texture *texture = NULL;
static int texture_width = 0;
static int texture_height = 0;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Surface *surface = NULL;
    char *png_path = NULL;

    SDL_SetAppMetadata("display_image", "1.0", "com.example.renderer-display-image");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Title", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);



    /* Textures are pixel data that we upload to the video hardware for fast drawing. Lots of 2D
       engines refer to these as "sprites." We'll do a static texture (upload once, draw many
       times) with data from a bitmap file. */

    /* SDL_Surface is pixel data the CPU can access. SDL_Texture is pixel data the GPU can access.
       Load a .png into a surface, move it to a texture from there. */
    SDL_asprintf(&png_path, "%ssprite.png", SDL_GetBasePath());  /* allocate a string of the full file path */
    surface = SDL_LoadPNG(png_path);
    if (!surface) {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_free(png_path);  /* done with this, the file is loaded. */

    texture_width = surface->w;
    texture_height = surface->h;

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */


    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_FPoint center;
    SDL_FRect dst_rect;
    const Uint64 now = SDL_GetTicks();

    /* we'll have a texture rotate around over 2 seconds (2000 milliseconds). 360 degrees in a circle! */
    const float rotation = (((float) ((int) (now % 2000))) / 2000.0f) * 360.0f;

    /* select a color */
    SDL_SetRenderDrawColorFloat(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);


    /* Center this one, and draw it with some rotation so it spins! */
    dst_rect.x = ((float) (DEFAULT_WINDOW_WIDTH - texture_width)) / 2.0f;
    dst_rect.y = ((float) (DEFAULT_WINDOW_HEIGHT - texture_height)) / 2.0f;
    dst_rect.w = (float) texture_width;
    dst_rect.h = (float) texture_height;
    /* rotate it around the center of the texture; you can rotate it from a different point, too! */
    center.x = texture_width / 2.0f;
    center.y = texture_height / 2.0f;
    SDL_RenderTextureRotated(renderer, texture, NULL, &dst_rect, rotation, &center, SDL_FLIP_NONE);

  
    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyTexture(texture);

    /* SDL will clean up the window/renderer for us. */
}
