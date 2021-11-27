### **Text editor (C/SDL2)**

**Building (Windows, MinGW/GCC)**

* Download the SDL2 development libraries (https://www.libsdl.org/download-2.0.php) and install them (check https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/mingw/index.php)
* Place zlib.dll next to `textEditor.exe`
* You can compile from the CMD with something like `gcc main.c -o textEditor.exe -Wall -g -IC:/MinGW/include/SDL2 -LC:/MinGW/lib -lmingw32 -lSDL2main -lSDL2` (you may tweak the flags and paths depending on your preferences)



### **Credits:**

STB_Image library by Sean Barrett (https://github.com/nothings/stb/blob/master/stb_image.h)
Font by Domsson (https://opengameart.org/content/ascii-bitmap-font-oldschool)



### **TODO**

* Command line
* Clipboard
* Visual selection
* Automatic indentation
* Word guessing
* Hotkey mapping
* Hardware acceleration (OpenGL?)
* ...