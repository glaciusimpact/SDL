# SDL
SDL3 codes to display a sprite, to use keyboard or to play music on Windows.

C code using SDL3 is provided as is.

- [01 - Display a window](01%20-%20Display%20a%20window)
- [02 - Display a picture](02%20-%20Display%20a%20picture)
- [03 - Rotate a picture](03%20-%20Rotate%20a%20picture)
- [04 - Keyboard](04%20-%20Keyboard)
- [05 - Music](05%20-%20Music)
- [06 - Picture, keyboard and music](06%20-%20Picture,%20keyboard%20and%20music)
- [07 - Circle, rectangle, line, point](07%20-%20Circle,%20rectangle,%20line,%20point)
- [08 - Sprite transparency](08%20-%20Sprite%20transparency)
- [09 - Animated sprite](09%20-%20Animated%20sprite)
- [10 - Icon](10%20-%20Icon)

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

As for the icon added to an executable there are 3 steps to do so:
1. Creating a .rc file

res.rc
```
id ICON "icon.ico"
```

2. Building a resource file using windres

```
windres res.rc -O coff -o my_res.res
```

3. Adding the .res file during compilation time (here "my_res.res")

```
gcc.exe -fdiagnostics-color=always -masm=intel -fno-ident -mwindows -s -Os -Wall src\icon.c -o icon.exe -I C:\SDL\x86_64-w64-mingw32\include -L C:\SDL\x86_64-w64-mingw32\lib -lSDL3 res\my_res.res
```




