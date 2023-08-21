#include <iostream>
#include <raylib.h>

using namespace std;

int main()
{

    cout << "check start" << endl;
    // game window dimensions
    const int screen_width = 1200;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "pong game");
    // speed.frame rate
    SetTargetFPS(60);
    // game loop
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}