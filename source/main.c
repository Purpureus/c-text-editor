
/*

  What is a text editor?

  input -> buffer(cursor location)
  buffer -> screen

*/

#include <assert.h>
#include "SDL2/SDL.h"
#include <windows.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define ARRLEN(arr) ((sizeof(arr))/(sizeof(arr[0])))

#define BOOL uint32
#define TRUE 1
#define FALSE 0

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define SCREEN_FONT_WIDTH 5
#define SCREEN_FONT_HEIGHT 7
#define SRC_FONT_WIDTH 7
#define SRC_FONT_HEIGHT 9
#define FONT_SCALE 3

#define BUFFER_CAPACITY 1024
#define CURSOR_BLINK 0

typedef Uint32 uint32;
typedef Uint64 uint64;
typedef Sint32 int32;
typedef Sint64 int64;

typedef struct {
    int32 x;
    int32 y;
} vec2i;

typedef struct {
    float x;
    float y;
} vec2f;


SDL_Surface *surface_from_file(const char *filePath)
{
    int32 width, height, n;
    unsigned char *pixels = stbi_load(filePath, &width, &height, &n, STBI_rgb_alpha);
    if(pixels == NULL) {
        fprintf(stderr, "ERROR: Couldn't load file %s: %s\n", filePath, stbi_failure_reason());
    }

    uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else // little endian, like x86
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    int32 depth = 32;
    int32 pitch = 4*width;

    return(SDL_CreateRGBSurfaceFrom((void*)pixels,
                                    width, height,
                                    depth, pitch,
                                    rmask, gmask, bmask, amask));
}

void renderChar(SDL_Renderer *renderer,
                SDL_Texture *fontTexture,
                char text,
                vec2i position,
                float scale) {

    uint32 fontx = 0;
    uint32 fonty = 0;
    switch(text)
    {
        case ' ': fontx = 0; fonty = 0; break;
        case '!': fontx = 1; fonty = 0; break;
        case '"': fontx = 2; fonty = 0; break;
        case '#': fontx = 3; fonty = 0; break;
        case '$': fontx = 4; fonty = 0; break;
        case '%': fontx = 5; fonty = 0; break;
        case '&': fontx = 6; fonty = 0; break;
        case '\'': fontx = 7; fonty = 0; break;
        case '(': fontx = 8; fonty = 0; break;
        case ')': fontx = 9; fonty = 0; break;
        case '*': fontx = 10; fonty = 0; break;
        case '+': fontx = 11; fonty = 0; break;
        case ',': fontx = 12; fonty = 0; break;
        case '-': fontx = 13; fonty = 0; break;
        case '.': fontx = 14; fonty = 0; break;
        case '/': fontx = 15; fonty = 0; break;
        case '0': fontx = 16; fonty = 0; break;
        case '1': fontx = 17; fonty = 0; break;

        case '2': fontx = 0; fonty = 1; break;
        case '3': fontx = 1; fonty = 1; break;
        case '4': fontx = 2; fonty = 1; break;
        case '5': fontx = 3; fonty = 1; break;
        case '6': fontx = 4; fonty = 1; break;
        case '7': fontx = 5; fonty = 1; break;
        case '8': fontx = 6; fonty = 1; break;
        case '9': fontx = 7; fonty = 1; break;
        case ':': fontx = 8; fonty = 1; break;
        case ';': fontx = 9; fonty = 1; break;
        case '<': fontx = 10; fonty = 1; break;
        case '=': fontx = 11; fonty = 1; break;
        case '>': fontx = 12; fonty = 1; break;
        case '?': fontx = 13; fonty = 1; break;
        case '@': fontx = 14; fonty = 1; break;
        case 'A': fontx = 15; fonty = 1; break;
        case 'B': fontx = 16; fonty = 1; break;
        case 'C': fontx = 17; fonty = 1; break;

        case 'D': fontx = 0; fonty = 2; break;
        case 'E': fontx = 1; fonty = 2; break;
        case 'F': fontx = 2; fonty = 2; break;
        case 'G': fontx = 3; fonty = 2; break;
        case 'H': fontx = 4; fonty = 2; break;
        case 'I': fontx = 5; fonty = 2; break;
        case 'J': fontx = 6; fonty = 2; break;
        case 'K': fontx = 7; fonty = 2; break;
        case 'L': fontx = 8; fonty = 2; break;
        case 'M': fontx = 9; fonty = 2; break;
        case 'N': fontx = 10; fonty = 2; break;
        case 'O': fontx = 11; fonty = 2; break;
        case 'P': fontx = 12; fonty = 2; break;
        case 'Q': fontx = 13; fonty = 2; break;
        case 'R': fontx = 14; fonty = 2; break;
        case 'S': fontx = 15; fonty = 2; break;
        case 'T': fontx = 16; fonty = 2; break;
        case 'U': fontx = 17; fonty = 2; break;

        case 'V': fontx = 0; fonty = 3; break;
        case 'W': fontx = 1; fonty = 3; break;
        case 'X': fontx = 2; fonty = 3; break;
        case 'Y': fontx = 3; fonty = 3; break;
        case 'Z': fontx = 4; fonty = 3; break;
        case '[': fontx = 5; fonty = 3; break;
        case '\\': fontx = 6; fonty = 3; break;
        case ']': fontx = 7; fonty = 3; break;
        case '^': fontx = 8; fonty = 3; break;
        case '_': fontx = 9; fonty = 3; break;
        case '`': fontx = 10; fonty = 3; break;
        case 'a': fontx = 11; fonty = 3; break;
        case 'b': fontx = 12; fonty = 3; break;
        case 'c': fontx = 13; fonty = 3; break;
        case 'd': fontx = 14; fonty = 3; break;
        case 'e': fontx = 15; fonty = 3; break;
        case 'f': fontx = 16; fonty = 3; break;
        case 'g': fontx = 17; fonty = 3; break;

        case 'h': fontx = 0; fonty = 4; break;
        case 'i': fontx = 1; fonty = 4; break;
        case 'j': fontx = 2; fonty = 4; break;
        case 'k': fontx = 3; fonty = 4; break;
        case 'l': fontx = 4; fonty = 4; break;
        case 'm': fontx = 5; fonty = 4; break;
        case 'n': fontx = 6; fonty = 4; break;
        case 'o': fontx = 7; fonty = 4; break;
        case 'p': fontx = 8; fonty = 4; break;
        case 'q': fontx = 9; fonty = 4; break;
        case 'r': fontx = 10; fonty = 4; break;
        case 's': fontx = 11; fonty = 4; break;
        case 't': fontx = 12; fonty = 4; break;
        case 'u': fontx = 13; fonty = 4; break;
        case 'v': fontx = 14; fonty = 4; break;
        case 'w': fontx = 15; fonty = 4; break;
        case 'x': fontx = 16; fonty = 4; break;
        case 'y': fontx = 17; fonty = 4; break;

        case 'z': fontx = 0; fonty = 5; break;
        case '{': fontx = 1; fonty = 5; break;
        case '|': fontx = 2; fonty = 5; break;
        case '}': fontx = 3; fonty = 5; break;
        case '~': fontx = 4; fonty = 5; break;

        default: fontx = 0; fonty = 0; break;
    }

    SDL_Rect srcRect = {
        .x = fontx * SRC_FONT_WIDTH,
        .y = fonty * SRC_FONT_HEIGHT,
        .w = SRC_FONT_WIDTH,
        .h = SRC_FONT_HEIGHT
    };

    SDL_Rect destRect = {
        .x = position.x * SCREEN_FONT_WIDTH * scale,
        .y = position.y * SCREEN_FONT_HEIGHT * scale,
        .w = SCREEN_FONT_WIDTH * scale,
        .h = SCREEN_FONT_HEIGHT * scale
    };

    SDL_RenderCopy(renderer,
                   fontTexture,
                   &srcRect,
                   &destRect);
}

void renderText(SDL_Renderer *renderer,
                SDL_Texture *fontTexture,
                char *text,
                uint32 textLength,
                vec2i textPos,
                float scale) {

    for(uint32 i = 0; i < textLength; i++)
    {
        if(text[i] == 0) continue;
        vec2i charPos = {
            textPos.x + i,
            textPos.y
        };
        renderChar(renderer, fontTexture, text[i], charPos, scale);
    }
}

void readFile(char *path, char *buffer, uint32 *bufferSize, uint32 bufferCapacity)
{
    FILE *file = fopen(path, "r");
    if(file == NULL) printf("File doesn't exist\n");
    
    fseek(file, 0, SEEK_END);
    int numBytes = ftell(file);
    if(numBytes > bufferCapacity) {
        printf("File is too big!");
    }

    fseek(file, 0, SEEK_SET);
    size_t elementsRead = fread(buffer, sizeof(char), numBytes, file);
    (*bufferSize) = elementsRead;
    printf("%d bytes read. ", elementsRead);

    fclose(file);
}

void saveFile(char *path, char *input, uint32 bufferSize)
{
    FILE *file = fopen(path, "w+");
    if(file == NULL) printf("File doesn't exist\n");
    printf("Saving file...");
    fwrite(input, sizeof(char), bufferSize, file);

    fclose(file);
}

void printBuffer(char *buffer, uint32 bufferSize)
{
    for(uint32 bufferIndex = 0;
        bufferIndex < bufferSize;
        bufferIndex++)
    {
        if(buffer[bufferIndex] == 0) {
            continue;
        }
        printf("%c", buffer[bufferIndex]);
    }
    printf("\n");
}

void bufferWriteChar(char *buffer,
                     size_t *bufferSize,
                     uint32 *cursor,
                     char *text,
                     size_t _textSize,
                     size_t bufferCapacity)
{
    size_t freeSpace = bufferCapacity - *bufferSize;
    size_t textSize = _textSize;
    if(textSize > freeSpace) {
        textSize = freeSpace;
    }
    memmove(buffer + *cursor + textSize,
            buffer + *cursor,
            bufferCapacity - *cursor);
    memcpy(buffer + *cursor, text, textSize);
    *bufferSize += textSize;
    *cursor += strlen(text);
}

char bufferDeleteChar(char *buffer, uint32 *bufferSize, uint32 *cursor)
{
    char output = buffer[*cursor-1];

    if(*cursor > 0) {
        // buffer[*cursor-1] = 0;
        memmove(buffer + *cursor - 1,
                buffer + *cursor,
                *bufferSize - *cursor);
        (*cursor) -= 1;
        (*bufferSize)--;
    }

    return output;
}

void bufferDeleteWord(char *buffer, uint32 *bufferSize, uint32 *cursor)
{
    if(*cursor <= 0) return;

    while(bufferDeleteChar(buffer, bufferSize, cursor) == ' ')
    { }

    if(buffer[*cursor-1] != ' ')
        bufferDeleteWord(buffer, bufferSize, cursor);
}

void cursorLeft(uint32 *cursor, size_t bufferSize, uint32 amount)
{
    for(uint32 i = 0; i < amount; i++) {
        if(*cursor == 0) return;
        (*cursor)--;
    }
}

void cursorRight(uint32 *cursor, size_t bufferSize, uint32 amount)
{
    for(uint32 i = 0; i < amount; i++) {
        if(*cursor == bufferSize) return;
        (*cursor)++;
    }
}

void endOfLine(uint32 *cursor, char *buffer, size_t bufferSize)
{
    while((*cursor) < bufferSize && buffer[*cursor] != '\n')
    {
        (*cursor)++;
    }
}

uint32 beginningOfLine(uint32 *cursor, char *buffer, size_t bufferSize)
{
    uint32 charCount = 0;
    while((*cursor) > 0 && buffer[*cursor-1] != '\n')
    {
        (*cursor)--;
        charCount++;
    }
    return charCount;
}


void cursorUp(uint32 *cursor, char *buffer, size_t bufferSize)
{
    uint32 currentColumn = beginningOfLine(cursor, buffer, bufferSize);
    cursorLeft(cursor, bufferSize, 1);
    beginningOfLine(cursor, buffer, bufferSize);

    for(uint32 newColumn = 0;
        newColumn < currentColumn;
        newColumn++)
    {
        if(buffer[*cursor] == '\n') break;
        cursorRight(cursor, bufferSize, 1);
    }
}

void cursorDown(uint32 *cursor, char *buffer, size_t bufferSize)
{
    uint32 currentColumn = beginningOfLine(cursor, buffer, bufferSize);
    endOfLine(cursor, buffer, bufferSize);
    cursorRight(cursor, bufferSize, 1);

    for(uint32 newColumn = 0;
        newColumn < currentColumn;
        newColumn++)
    {
        if(buffer[*cursor] == '\n') break;
        cursorRight(cursor, bufferSize, 1);
    }
}

void cursorLeftWord(uint32 *cursor, char *buffer, size_t bufferSize)
{
    while(*cursor > 0 && buffer[*cursor-1] == ' ')
        cursorLeft(cursor, bufferSize, 1);

    while(*cursor > 0 && buffer[*cursor-1] != ' ')
        cursorLeft(cursor, bufferSize, 1);
}

void cursorRightWord(uint32 *cursor, char *buffer, size_t bufferSize)
{
    while(*cursor < bufferSize && buffer[*cursor+1] == ' ')
        cursorRight(cursor, bufferSize, 1);

    while(*cursor < bufferSize && buffer[*cursor+1] != ' ')
        cursorRight(cursor, bufferSize, 1);

    cursorRight(cursor, bufferSize, 1);
}

int main(int argc, char *argv[]) {

    // Initialization //
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Text editor test",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if(window == NULL)
        printf("Window couldn't be created! %s\n", SDL_GetError());

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                0,
                                                SDL_RENDERER_SOFTWARE|SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
        printf("Renderer couldn't be created! %s\n", SDL_GetError());

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Load fonts //
    SDL_Surface *mySurface = surface_from_file("font.png");
    SDL_Texture *myTexture = SDL_CreateTextureFromSurface(renderer, mySurface);

    char buffer[BUFFER_CAPACITY] = "";
    size_t bufferSize = 0;

    uint32 cursor = 0;
    BOOL lctrl = FALSE;

    BOOL _doOnce_ = TRUE;

    BOOL running = TRUE;
    while(running)
    {
        // uint32 ticks = SDL_GetTicks();

        //// Input handling ////
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                {
                    running = FALSE;
                    break;
                } break;

                case SDL_TEXTINPUT:
                {
                    size_t textSize = strlen(event.text.text);
                    bufferWriteChar(buffer, &bufferSize, &cursor,
                                    event.text.text, textSize,
                                    BUFFER_CAPACITY);
                } break;

                case SDL_KEYDOWN:
                {
                    SDL_Keycode keycode = event.key.keysym.sym;

                    switch(keycode)
                    {
                        case SDLK_LCTRL:
                            lctrl = TRUE;
                            break;

                        case SDLK_ESCAPE:
                            running = FALSE;
                            break;

                        case SDLK_F7:
                            readFile("myfile.txt", buffer, &bufferSize, BUFFER_CAPACITY);
                            break;
                            
                        case SDLK_F8:
                            saveFile("myfile.txt", buffer, bufferSize);
                            break;

                        case SDLK_LEFT:
                            if(cursor == 0) break;
                            if(lctrl)
                                cursorLeftWord(&cursor, buffer, bufferSize);
                            else
                                cursorLeft(&cursor, bufferSize, 1);
                            break;

                        case SDLK_RIGHT:
                            if(cursor == bufferSize) break;
                            if(lctrl)
                                cursorRightWord(&cursor, buffer, bufferSize);
                            else
                                cursorRight(&cursor, bufferSize, 1);
                            break;

                        case SDLK_UP:
                            cursorUp(&cursor, buffer, bufferSize);
                            break;

                        case SDLK_DOWN:
                            cursorDown(&cursor, buffer, bufferSize);
                            break;

                        case SDLK_END:
                            endOfLine(&cursor, buffer, bufferSize);
                            break;

                        case SDLK_HOME:
                            beginningOfLine(&cursor, buffer, bufferSize);
                            break;

                        case SDLK_BACKSPACE:
                            if(lctrl) {
                                bufferDeleteWord(buffer, &bufferSize, &cursor);
                            }
                            else {
                                bufferDeleteChar(buffer, &bufferSize, &cursor);
                            }
                            break;

                        case SDLK_RETURN: {
                            char eol[2] = "\n";
                            bufferWriteChar(buffer, &bufferSize, &cursor,
                                            eol, strlen(eol),
                                            BUFFER_CAPACITY);
                        } break;

                        default: break;
                    }
                } break;

                case SDL_KEYUP:
                {
                    SDL_Keycode keycode = event.key.keysym.sym;

                    switch(keycode)
                    {
                        case SDLK_LCTRL:
                            lctrl = FALSE;
                    }
                } break;
            }
        }

        //// Rendering ////
        SDL_SetRenderDrawColor(renderer, 32, 0, 0, 255);
        SDL_RenderClear(renderer);

        vec2i renderPos = { 0, 0 };
        vec2i cursorScreenPos = { 0, 0 };
        for(int bufferIndex = 0;
            bufferIndex < bufferSize;
            bufferIndex++)
        {
            if(_doOnce_) {
                printf("%d ", bufferIndex);
            }

            char currentChar = buffer[bufferIndex];
            if(currentChar == 0) continue;

            renderChar(renderer, myTexture, currentChar, renderPos, FONT_SCALE);

            if(currentChar == '\n') {
                renderPos.y++;
                renderPos.x = 0;
            }
            else {
                renderPos.x++;
            }

            if(bufferIndex == cursor-1) {
                cursorScreenPos.x = renderPos.x;
                cursorScreenPos.y = renderPos.y;
            }
        }

        // Render cursor
        SDL_Rect cursorRect = {
            .x = cursorScreenPos.x * SCREEN_FONT_WIDTH * FONT_SCALE,
            .y = cursorScreenPos.y * SCREEN_FONT_HEIGHT * FONT_SCALE,
            .w = 5,
            .h = SCREEN_FONT_HEIGHT * FONT_SCALE
        };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &cursorRect);

        // Render logger
        vec2i loggerPos = { 0, 27 };
        char logText[64] = "";
        
        sprintf(logText, "cur %d at %d, bufferSize %d",
                buffer[cursor],
                cursor,
                bufferSize);
        renderText(renderer, myTexture, logText, ARRLEN(logText), loggerPos, 3);

        SDL_RenderPresent(renderer);

        _doOnce_ = FALSE;
    }

    return(0);
}
