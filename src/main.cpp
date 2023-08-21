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
        // draw ball
        DrawCircle(screen_width / 2, screen_height / 2, 20, RED);
        // Draw paddle
        const int paddle_height = 120;
        DrawRectangle(10, screen_height / 2 - paddle_height / 2, 25, paddle_height, WHITE);
        // left
        DrawRectangle(screen_width - 35, screen_height / 2 - paddle_height / 2, 25, paddle_height, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}