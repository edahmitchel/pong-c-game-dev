#include <iostream>
#include <raylib.h>

using namespace std;
class Ball
{
public:
    float x, y;
    int speed_x, speed_y;
    int raduis;
    void Draw()
    {

        // draw ball
        DrawCircle(x, y, raduis, RED);
    };
    void update_position()
    {
        x += speed_x;
        y += speed_y;
        // Checking collision with edges of game window and changing direction of ball
        if (y + raduis >= GetScreenHeight() || y - raduis <= 0)
        {
            speed_y *= -1;
        };
        if (x + raduis >= GetScreenWidth() || x - raduis <= 0)
        {
            speed_x *= -1;
        };
    };
};
class Paddle
{
};
Ball ball;

int main()
{

    cout << "check start" << endl;
    // game window dimensions
    const int screen_width = 1200;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "pong game");
    // speed.frame rate
    SetTargetFPS(60);
    ball.raduis = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;
    // game loop
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ball.update_position();
        // middl2 line
        ClearBackground(BLACK);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
        // draww ball
        ball.Draw();
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