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
public:
    float x, y;
    float width, height;
    int speed;
    void Draw()
    {
        DrawRectangle(x, y, width, height, WHITE);
    }
    void update_position()
    {
        if (IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        };
        RestrictMovement();
    };

protected:
    void RestrictMovement()
    {
        if (y <= 0)
        {
            y = 0;
        };
        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        };
    };
};
// using inheritance
class NpcPaddle : public Paddle
{
public:
    void update_position(int ball_y)
    {
        // algorithm for ai
        if (y + height / 2 > ball_y)
        {

            y = y - speed;
        }
        if (y + height / 2 <= ball_y)
        {
            y = y + speed;
        };
        RestrictMovement();
    };
};
Ball ball;
Paddle player;
NpcPaddle npc;

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

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;
    npc.height = 120;
    npc.width = 25;
    npc.x = 10;
    npc.y = screen_height / 2 - npc.height / 2;
    npc.speed = 6;
    // Draw paddle
    // game loop
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ball.update_position();
        player.update_position();
        npc.update_position(ball.y);
        // middl2 line
        ClearBackground(BLACK);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
        // draww ball
        ball.Draw();
        npc.Draw();
        // const int paddle_height = 120;
        // DrawRectangle(10, screen_height / 2 - paddle_height / 2, 25, paddle_height, WHITE);
        // left
        player.Draw();
        // DrawRectangle(screen_width - 35, screen_height / 2 - paddle_height / 2, 25, paddle_height, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}