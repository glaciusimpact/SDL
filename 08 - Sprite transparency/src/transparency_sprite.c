#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480


// We will use this renderer to draw into this window every frame
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static SDL_Texture *texture = NULL;
static SDL_Texture *texture_background = NULL;
static SDL_FRect image001;
static SDL_FRect background;

static unsigned char image001_go_to_left = 0;

// This function runs once at startup
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Surface *surface = NULL;
    char *png_path = NULL;

    // Program information embedded inside the executable
    SDL_SetAppMetadata("transparency", "1.0", "com.example.renderer-transparency");

    // SDL initialization
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        // Error message
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());

        return SDL_APP_FAILURE;
    }

    // Creation of a Window
    if (!SDL_CreateWindowAndRenderer("transparency", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        // Error message
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());

        return SDL_APP_FAILURE;
    }

    // Set a device-independent resolution and presentation mode for rendering
    SDL_SetRenderLogicalPresentation(renderer, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);


    /* BACKGROUND */

    /* SDL_Surface is pixel data the CPU can access. SDL_Texture is pixel data the GPU can access.
       Load a .png into a surface, move it to a texture from there. */
    SDL_asprintf(&png_path, "%sbackground.png", SDL_GetBasePath());  /* allocate a string of the full file path */
    
    // Loading PNG image into a surface
    surface = SDL_LoadPNG(png_path);

    // Check if there is an error
    if (!surface)
    {
        // Error message
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
    
        return SDL_APP_FAILURE;
    }

    // Free the picture file
    SDL_free(png_path);

    // Store surface into a texture (GPU)
    texture_background = SDL_CreateTextureFromSurface(renderer, surface);
    
    // Check if there is an error
    if (!texture_background)
    {
        // Error message
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
    
    // Allocate a string of the full file picture path
    SDL_asprintf(&png_path, "%ssprite.png", SDL_GetBasePath());  
    
    // Loading PNG image into a surface
    surface = SDL_LoadPNG(png_path);

    // Check if there is an error
    if (!surface)
    {
        // Error message
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
        
        return SDL_APP_FAILURE;
    }

    // Free the picture file
    SDL_free(png_path);

    // Store surface into a texture (GPU)
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Check if there is an error
    if (!texture)
    {
        // Error message
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());

        return SDL_APP_FAILURE;
    }

    // Position and size of the image (texture)
    image001.x = 250;
    image001.y = 280;
    image001.w = surface->w;
    image001.h = surface->h;

    // Removing surface since we get the texture
    SDL_DestroySurface(surface);

    // 50% of transparency on the sprite (0 = 100%, 127 = 50%, 255 = 0%)
    SDL_SetTextureAlphaMod(texture, 127);

    // Carry on with the program!
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    // Quit event (like closing the window)?
    if (event->type == SDL_EVENT_QUIT) {
        // Yes

        // End the program, reporting success to the OS.
        return SDL_APP_SUCCESS;
    }

    /* Check if a key has been pressed */
    if (event->type == SDL_EVENT_KEY_DOWN) {

        // Left arrow key
        if (event->key.scancode == SDL_SCANCODE_LEFT)
        {
            // Sprite moving to left
            image001_go_to_left = 1;

            // Moving sprite to the left
            if (image001.x > 10)
                image001.x -= 10;
        }
	    // Right arrow key
        else if (event->key.scancode == SDL_SCANCODE_RIGHT)
        {
            // Sprite moving to right
            image001_go_to_left = 0;

            // Moving sprite to the right
            if (image001.x <= 560)
                image001.x += 10;
        }
        // Toggle desktop fullscreen Alt+Enter
        else if ((event->key.scancode == SDL_SCANCODE_RETURN) && (event->key.mod & SDL_KMOD_LALT))
        {
            // Change fullscreen <-> window mode
            bool is_fullscreen = !(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN);

            // Change the window
            SDL_SetWindowFullscreen(window, is_fullscreen);

            // Wait for change to apply
            SDL_SyncWindow(window);
        }
        // Esc key
        else if (event->key.scancode == SDL_SCANCODE_ESCAPE)
        {
            // End the program, reporting success to the OS.
            return SDL_APP_SUCCESS;
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

    // Sprite going to left?
    if (image001_go_to_left == 1)
    {
        // Yes

        /* Draw sprite with horizontal flip */
        SDL_RenderTextureRotated(renderer, texture, NULL, &image001, 0.0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else
    {
        // No

        /* Draw sprite */
        SDL_RenderTexture(renderer, texture, NULL, &image001);
    }
    
    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
 
    /* SDL will clean up the window/renderer for us. */
    SDL_DestroyTexture(texture);
    
}
