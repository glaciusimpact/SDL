#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480

#define SPRITE_STATUS_IDLE 0
#define SPRITE_STATUS_WALKING 1

#define SPRITE_FACE_DIRECTION_UP 0
#define SPRITE_FACE_DIRECTION_LEFT 1
#define SPRITE_FACE_DIRECTION_DOWN 2
#define SPRITE_FACE_DIRECTION_RIGHT 3


// We will use this renderer to draw into this window every frame
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture_background = NULL;
static SDL_FRect background;


Uint64 lastTime = 0, currentTime, fps = 0, lastFrameDisplayed;
// Number of frame per second in ms
Uint32 FrameDelayInMs = 1000 / 60;

typedef struct {
    // Y position of the first image in a texture
    unsigned int first_image_y_position[4];
    // Number of images in the animation (1 is for static image)
    unsigned int max_number_of_images;
    // Time each image of the sprite is displayed on the screen (in ms)
    Uint64 timeASpriteIsOnTheScreen;
} List_of_Animations;


typedef struct Sprite_status {
    char *path;
    SDL_FRect image;
    SDL_Texture *texture;
    SDL_FRect sprite_position;
    unsigned char current_frame;
    Uint64 lastTime;
    unsigned char face_direction;
    int moving_x;
    int moving_y;
    int speed;
    unsigned char status;
    List_of_Animations animation[2];
} Sprite;

Sprite sprite001 = {
    .path = "%ssprite_sheet.png",
    // Position and size of the whole picture (texture)
    .image.x = 0,
    .image.y = 0,
    .image.w = 0,
    .image.h = 0,
    // Position and size of the sprite (texture)
    .sprite_position.x = 250,
    .sprite_position.y = 280,
    .sprite_position.w = 64,
    .sprite_position.h = 64,
    // Default current frame
    .current_frame = 0,
    // Default time for the sprite
    .lastTime = 0,
    // Direction of the face of the sprite
    .face_direction = SPRITE_FACE_DIRECTION_DOWN,
    // Movement of the sprite (X)
    .moving_x = 0,
    // Movement of the sprite (Y)
    .moving_y = 0,
    // Speed (number of pixels the sprite move while walking)
    .speed = 4,
    // Default status
    .status = SPRITE_STATUS_IDLE,
    // Animations
    /* IDLE */
    .animation[SPRITE_STATUS_IDLE].first_image_y_position[SPRITE_FACE_DIRECTION_UP] = 22*64,
    .animation[SPRITE_STATUS_IDLE].first_image_y_position[SPRITE_FACE_DIRECTION_LEFT] = 23*64,
    .animation[SPRITE_STATUS_IDLE].first_image_y_position[SPRITE_FACE_DIRECTION_DOWN] = 24*64,
    .animation[SPRITE_STATUS_IDLE].first_image_y_position[SPRITE_FACE_DIRECTION_RIGHT] = 25*64,
    .animation[SPRITE_STATUS_IDLE].max_number_of_images = 2,
    .animation[SPRITE_STATUS_IDLE].timeASpriteIsOnTheScreen = 1000,
    /* WALKING */
    .animation[SPRITE_STATUS_WALKING].first_image_y_position[SPRITE_FACE_DIRECTION_UP] = 8*64,
    .animation[SPRITE_STATUS_WALKING].first_image_y_position[SPRITE_FACE_DIRECTION_LEFT] = 9*64,
    .animation[SPRITE_STATUS_WALKING].first_image_y_position[SPRITE_FACE_DIRECTION_DOWN] = 10*64,
    .animation[SPRITE_STATUS_WALKING].first_image_y_position[SPRITE_FACE_DIRECTION_RIGHT] = 11*64,
    .animation[SPRITE_STATUS_WALKING].max_number_of_images = 9,
    .animation[SPRITE_STATUS_WALKING].timeASpriteIsOnTheScreen = 100
};


/* Function declaration */
void DisplaySprite(SDL_Renderer *my_renderer, Sprite *my_sprite);
void Update();
void Render();


// This function runs once at startup
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Surface *surface = NULL;
    char *png_path = NULL;

    // Program information embedded inside the executable
    SDL_SetAppMetadata("animation", "1.0", "com.example.renderer-animation");

    // SDL initialization
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        // Error message
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());

        return SDL_APP_FAILURE;
    }

    // Creation of a Window
    if (!SDL_CreateWindowAndRenderer("Sprite animation FPS: ", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer))
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
    SDL_asprintf(&png_path, sprite001.path, SDL_GetBasePath());  
    
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
    sprite001.texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Check if there is an error
    if (!sprite001.texture)
    {
        // Error message
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());

        return SDL_APP_FAILURE;
    }

    // Position and size of the image (texture)
    sprite001.image.w = surface->w;
    sprite001.image.h = surface->h;
   

    // Removing surface since we get the texture
    SDL_DestroySurface(surface);

    // Get current tick
    lastTime = SDL_GetTicks();
    lastFrameDisplayed = lastTime;

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

    // Check if a key has been pressed
    if (event->type == SDL_EVENT_KEY_DOWN)
    {

        // Up arrow key
        if (event->key.scancode == SDL_SCANCODE_UP)
        {
            // Up position sprite
            sprite001.status = SPRITE_STATUS_WALKING;

            // Direction
            sprite001.face_direction = SPRITE_FACE_DIRECTION_UP;
            
            sprite001.moving_y = -1;
        }
        // Left arrow key
        if (event->key.scancode == SDL_SCANCODE_LEFT)
        {
            // Left position sprite
            sprite001.status = SPRITE_STATUS_WALKING;

            // Direction
            sprite001.face_direction = SPRITE_FACE_DIRECTION_LEFT;
            
            sprite001.moving_x = -1;
        }
	    // Down arrow key
        if (event->key.scancode == SDL_SCANCODE_DOWN)
        {
            // Down position sprite
            sprite001.status = SPRITE_STATUS_WALKING;

            // Direction
            sprite001.face_direction = SPRITE_FACE_DIRECTION_DOWN;

            sprite001.moving_y = 1;
        }
	    // Right arrow key
        if (event->key.scancode == SDL_SCANCODE_RIGHT)
        {
            // Right position sprite
            sprite001.status = SPRITE_STATUS_WALKING;

            // Direction
            sprite001.face_direction = SPRITE_FACE_DIRECTION_RIGHT;

            sprite001.moving_x = 1;
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
    else if (event->type == SDL_EVENT_KEY_UP) {

        // Up arrow key
        if (event->key.scancode == SDL_SCANCODE_UP)
        {
            // Up position sprite

            // IDLE
            sprite001.status = SPRITE_STATUS_IDLE;

            // Direction
            sprite001.face_direction = SPRITE_FACE_DIRECTION_UP;
            
            // Sprite not moving on this axis
            sprite001.moving_y = 0;
        }
        // Left Arrow key
        if (event->key.scancode == SDL_SCANCODE_LEFT)
        {
            // Left position sprite

            // IDLE
            sprite001.status = SPRITE_STATUS_IDLE;

            // Direction
            sprite001.face_direction = SPRITE_FACE_DIRECTION_LEFT;
            
            // Sprite not moving on this axis
            sprite001.moving_x = 0;
        }
        // Down Arrow key
        if (event->key.scancode == SDL_SCANCODE_DOWN)
        {
            // Down position sprite

            // IDLE
            sprite001.status = SPRITE_STATUS_IDLE;

            // Direction
            sprite001.face_direction = SPRITE_FACE_DIRECTION_DOWN;
            
            // Sprite not moving on this axis
            sprite001.moving_y = 0;
        }
        // Right Arrow key
        if (event->key.scancode == SDL_SCANCODE_RIGHT)
        {
            // Right position sprite

            // IDLE
            sprite001.status = SPRITE_STATUS_IDLE;

            // Direction
            sprite001.face_direction = SPRITE_FACE_DIRECTION_RIGHT;
            
            // Sprite not moving on this axis
            sprite001.moving_x = 0;


        }    
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    // char *title;
    char buffer[256];

    Update();
    
    Render();

    // Increase the number of frame by 1
    fps++;

    // Get current tick
    currentTime = SDL_GetTicks();

    // 1 second or more?
    if (currentTime >= lastTime + 1000)
    {
        // Yes

        // Change the title of the windows adding the number of frames per second
        SDL_snprintf(buffer, sizeof(buffer), "Sprite animation FPS: %d", (int)fps);
        SDL_SetWindowTitle(window, buffer);
        
        // Frame counter to 0
        fps = 0;

        // Last tick is current tick
        lastTime = currentTime;
    }
    
    // Frame rate limiting?
    if ((currentTime - lastFrameDisplayed) < FrameDelayInMs)
    {
        // Yes

        // Delay
        SDL_Delay(FrameDelayInMs - (currentTime - lastFrameDisplayed));
        // SDL_Delay(16);

    }
    
    // Last tick is current tick
    lastFrameDisplayed = SDL_GetTicks();

    // SDL_Log("> %d, %d", (int)sprite001.sprite_position.x, (int)sprite001.sprite_position.y);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
 
    /* SDL will clean up the window/renderer for us. */
    SDL_DestroyTexture(sprite001.texture);
    SDL_DestroyTexture(texture_background);
    
}

void Update()
{

    // Sprite moving?
    if (sprite001.status == SPRITE_STATUS_WALKING)
    {
        // Yes

        // Adding new position
        sprite001.sprite_position.x += sprite001.moving_x * sprite001.speed;
        sprite001.sprite_position.y += sprite001.moving_y * sprite001.speed;

        // Limiting the position of the sprite in a rectange in the the map
        // X
        if (sprite001.sprite_position.x < 10)
            sprite001.sprite_position.x = 10;
        else if (sprite001.sprite_position.x > 565)
            sprite001.sprite_position.x = 565;
        // Y
        if (sprite001.sprite_position.y < 123)
            sprite001.sprite_position.y = 123;
        else if (sprite001.sprite_position.y > 310)
            sprite001.sprite_position.y = 310;
    }

}

void Render()
{

    /* Clear the window to the draw color. */
    SDL_RenderClear(renderer);

    /* Draw background */
    SDL_RenderTexture(renderer, texture_background, NULL, &background);

    /* Draw sprite */
    DisplaySprite(renderer, &sprite001);

    /* Put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);
}

void DisplaySprite(SDL_Renderer *my_renderer, Sprite *my_sprite)
{
    SDL_FRect selected_sprite;

    // Sprite selection
    selected_sprite.x = my_sprite->current_frame * my_sprite->sprite_position.w;
    selected_sprite.y = my_sprite->animation[my_sprite->status].first_image_y_position[my_sprite->face_direction];
    selected_sprite.w = my_sprite->sprite_position.w;
    selected_sprite.h = my_sprite->sprite_position.h;

    // Draw sprite
    SDL_RenderTexture(my_renderer, my_sprite->texture, &selected_sprite, &my_sprite->sprite_position);

    // Next image of the sprite animation?
    if ((currentTime - my_sprite->lastTime) >= my_sprite->animation[my_sprite->status].timeASpriteIsOnTheScreen / my_sprite->speed)
    {
        // Yes time to update the sprite with a new picture
        my_sprite->current_frame++;

        // Save current time
        my_sprite->lastTime = currentTime;
    }

    // Back to first image?
    if (my_sprite->current_frame >= my_sprite->animation[my_sprite->status].max_number_of_images)
        my_sprite->current_frame = 0;

}