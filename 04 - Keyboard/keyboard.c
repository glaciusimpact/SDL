#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480


/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static SDL_Texture *texture = NULL;
static SDL_Texture *texture_background = NULL;
static SDL_FRect image001;
static SDL_FRect background;


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Surface *surface = NULL;
    char *png_path = NULL;

    /* Program information embedded inside the executable  */
    SDL_SetAppMetadata("keyboard", "1.0", "com.example.renderer-keyboard");

    /* SDL initialization */
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Creation of a Window */
    if (!SDL_CreateWindowAndRenderer("Title", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);


    /* BACKGROUND */

    /* SDL_Surface is pixel data the CPU can access. SDL_Texture is pixel data the GPU can access.
       Load a .png into a surface, move it to a texture from there. */
    SDL_asprintf(&png_path, "%sbackground.png", SDL_GetBasePath());  /* allocate a string of the full file path */
    surface = SDL_LoadPNG(png_path);
    if (!surface) {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_free(png_path);  /* done with this, the file is loaded. */

    // Store surface into a texture (GPU)
    texture_background = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture_background) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Position and size of the background (texture)
    background.x = 0;
    background.y = 0;
    background.w = surface->w;
    background.h = surface->h;

    SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */


    /* SPRITE */

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

    // Store surface into a texture (GPU)
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Position and size of the image (texture)
    image001.x = 0;
    image001.y = 310;
    image001.w = surface->w;
    image001.h = surface->h;

    SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    /* Check if a key has been pressed */
    if (event->type == SDL_EVENT_KEY_DOWN) {

        // Left arrow key
        if (event->key.scancode == SDL_SCANCODE_LEFT)
        {
            if (image001.x > 10)
                image001.x -= 10;
        }
	// Right arrow key
        else if (event->key.scancode == SDL_SCANCODE_RIGHT)
        {
            if (image001.x <= 560)
                image001.x += 10;
        }
        // Esc key
        else if (event->key.scancode == SDL_SCANCODE_ESCAPE)
        {
            return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
        }
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    
    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);

    /* Draw background */
    SDL_RenderTexture(renderer, texture_background, NULL, &background);

    /* Draw sprite */
    SDL_RenderTexture(renderer, texture, NULL, &image001);

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
