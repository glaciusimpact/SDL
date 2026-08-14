# SDL
SDL3 codes to display a sprite, to use keyboard or to play music.

C code using SDL3 is provided as is.

- [01 - Display a window](01%20-%20Display%20a%20window)
- [02 - Display a picture](02%20-%20Display%20a%20picture)
- [03 - Rotate a picture](03%20-%20Rotate%20a%20picture)
- [04 - Keyboard](04%20-%20Keyboard)
- [05 - Music](05%20-%20Music)
- [06 - Picture, keyboard and music](06%20-%20Picture,%20keyboard%20and%20music)


To compile the source code the following tools are required:
- gcc from Mingw64
- `include` and `lib` files from SDL3
    - Latest SDL is available here:
        - https://github.com/libsdl-org/SDL/releases/latest
    - Latest SDL mixer for audio files:
        - https://github.com/libsdl-org/SDL_mixer/releases/latest

Here is an example of a compilation command for the program `character_music.exe` (from "06 - Picture, keyboard and music"):

``` bash
gcc.exe -fdiagnostics-color=always -masm=intel -fno-ident -mwindows -s -Os -Wall character_music.c -o character_music.exe -I C:\SDL\x86_64-w64-mingw32\include -L C:\SDL\x86_64-w64-mingw32\lib -lSDL3 -lSDL3_mixer
```
