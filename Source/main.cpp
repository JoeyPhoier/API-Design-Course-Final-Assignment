/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*b
*   Copyright (c) 2013-2022 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "game.h"


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{    
    Game game;
    SetTargetFPS(60);

    //TODO: Move to a RAII class that throws.
    auto sound = LoadSound("./hitHurt.ogg");
    game.Loop();

    //TODO: What is this doing here?
    std::string filename = "level.txt";

    return 0;
}