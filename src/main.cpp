#include <iostream>
#include <raylib.h>
#include <cstdlib>

using namespace std;
// scores
int npc_score = 0;
int player_score = 0;
enum PowerUpType
{
    FASTER_BALL,
    SLOWER_BALL,
    FASTER_PADDLE
};

enum GameState
{
    ONGOING,
    PLAYER_WON,
    NPC_WON,
    PLAYER_LOST
};

GameState currentState = ONGOING;
class PowerUp
{
public:
    Vector2 position;
    PowerUpType type;
    bool active = false;
    int radius = 15;

    void Draw()
    {
        Color color;
        switch (type)
        {
        case FASTER_BALL:
            color = GREEN;
            break;
        case SLOWER_BALL:
            color = PINK;
            break;
        case FASTER_PADDLE:
            color = BLUE;
            break;
        }
        DrawCircleV(position, radius, color);
    }
};
PowerUp powerUp;
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
        if (x + raduis >= GetScreenWidth())
        {
            npc_score++;
            ResetBall();
        }

        if (x - raduis <= 0)
        {
            player_score++;
            ResetBall();
            // speed_x *= -1;
        };
    };
    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        x = GetScreenWidth() / 2;
        int speed_choice[2] = {-1, 1};
        speed_x *= speed_choice[GetRandomValue(0, 1)];
        speed_y *= speed_choice[GetRandomValue(0, 1)];
    }
};
class Paddle
{
public:
    float x, y;
    float width, height;
    int speed;
    Color color; // <-- added color property

    Paddle() : color(WHITE) {} // default constructor with WHITE color

    Paddle(Color c) : color(c) {} // constructor to set the color

    void Draw()
    {
        DrawRectangle(x, y, width, height, color);
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
    bool active = true; // Add this line

    int speed_y = 6; // vertical speed for the npcShooter
    int speed_x = 6; // horizontal speed for the npcShooter
                     // Constructor for NpcPaddle that takes a Color
    NpcPaddle(Color c) : Paddle(c) {}

    void update_position_horizontal()
    {
        // If it hits the screen edges, reverse its direction
        if (x <= 0 || x + width >= GetScreenWidth())
        {
            speed_x *= -1;
        }
        x += speed_x;
    }

    void update_position_vertical()
    {
        y += speed_y;

        // If it hits the top or bottom of the screen, reverse its direction
        if (y <= 0 || y + height >= GetScreenHeight())
        {
            speed_y *= -1;
        }
    }

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

class Projectile
{
public:
    float x, y;
    int speed_x;
    int radius;
    bool active;

    void Draw()
    {
        if (active)
        {
            DrawCircle(x, y, radius, BLUE);
        }
    };

    void UpdatePosition()
    {
        if (active)
        {
            x += speed_x;
            // Deactivate projectile when it goes out of the screen
            if (x - radius > GetScreenWidth() || x + radius < 0)
            {
                active = false;
            }
        }
    };
};
void SpawnPowerUp()
{
    powerUp.position = {float(GetRandomValue(100, 1100)), float(GetRandomValue(100, 700))};
    int randomType = GetRandomValue(0, 2); // 3 types of power-ups
    powerUp.type = static_cast<PowerUpType>(randomType);
    powerUp.active = true;
}
Projectile playerProjectile;
Projectile npcProjectile;

Ball ball;
Paddle player(BLUE);
NpcPaddle npc(RED);
NpcPaddle npcShooter(BLUE);
int main()
{
    int framesSinceLastPowerUp = 0;

    cout << "check start" << endl;
    // game window dimensions
    const int screen_width = 1200;
    const int screen_height = 800;
    framesSinceLastPowerUp++;

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
    //
    npcShooter.height = 60;
    npcShooter.width = 25;
    npcShooter.x = 30;
    npcShooter.y = screen_height / 2 - npc.height / 2;
    npcShooter.speed = 9;

    playerProjectile.radius = 10;    // Adjust the value as needed
    npcProjectile.radius = 10;       // Adjust the value as needed
    int npcProjectileInterval = 120; // Interval in frames (2 seconds at 60 FPS)
    int npcProjectileTimer = npcProjectileInterval;

    // Draw paddle
    // game loop
    while (WindowShouldClose() == false)
    {
        if (currentState == ONGOING)
        {

            BeginDrawing();
            // Spawn a power-up every 300 frames as an example
            if (framesSinceLastPowerUp >= 300)
            {
                SpawnPowerUp();
                framesSinceLastPowerUp = 0;
            }
            // updating positions
            ball.update_position();
            player.update_position();
            npc.update_position(ball.y);
            npcShooter.update_position_vertical();
            // Update projectile positions...
            playerProjectile.UpdatePosition();
            npcProjectile.UpdatePosition();

            // check for collision
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, (float)ball.raduis, Rectangle{player.x, player.y, player.width, player.height}))
            {
                ball.speed_x *= -1;
            };
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, (float)ball.raduis, Rectangle{npc.x, npc.y, npc.width, npc.height}))
            {
                ball.speed_x *= -1;
            };
            // Check for collision between projectiles and paddles...
            if (CheckCollisionCircleRec(Vector2{playerProjectile.x, playerProjectile.y}, (float)playerProjectile.radius, Rectangle{npcShooter.x, npcShooter.y, npcShooter.width, npcShooter.height}) && playerProjectile.active)
            {
                // npc_score = npc_score - 1;
                npcShooter.active = false; //
                playerProjectile.active = false;
            }

            if (CheckCollisionCircleRec(Vector2{npcProjectile.x, npcProjectile.y}, (float)npcProjectile.radius, Rectangle{player.x, player.y, player.width, player.height}) && npcProjectile.active)
            {
                player_score = player_score - 1;

                npcProjectile.active = false;
            }
            if (powerUp.active)
            {
                // For ball-speed-changing power-ups
                if (CheckCollisionCircles({ball.x, ball.y}, ball.raduis, powerUp.position, powerUp.radius))
                {
                    switch (powerUp.type)
                    {
                    case FASTER_BALL:
                        ball.speed_x *= 1.5;
                        ball.speed_y *= 1.5;
                        break;
                    case SLOWER_BALL:
                        ball.speed_x *= 0.5;
                        ball.speed_y *= 0.5;
                        break;
                    default:
                        break;
                    }
                    powerUp.active = false; // deactivate after collision
                }

                // For paddle-speed-changing power-up
                if (CheckCollisionCircleRec(powerUp.position, powerUp.radius, {player.x, player.y, player.width, player.height}) || CheckCollisionCircleRec(powerUp.position, powerUp.radius, {npc.x, npc.y, npc.width, npc.height}))
                {
                    if (powerUp.type == FASTER_PADDLE)
                    {
                        player.speed *= 1.5;
                        npc.speed *= 1.5;
                    }
                    powerUp.active = false; // deactivate after collision
                }
            }

            // Draw power-up if active
            if (powerUp.active)
            {
                powerUp.Draw();
            }

            // Draw projectiles...
            playerProjectile.Draw();
            npcProjectile.Draw();

            // Check for input to shoot projectiles...
            if (IsKeyPressed(KEY_SPACE))
            {
                if (!playerProjectile.active)
                {
                    // Set the initial position of the player's projectile to the middle of the player paddle
                    playerProjectile.x = player.x + player.width;
                    playerProjectile.y = player.y + player.height / 2 - playerProjectile.radius; // Updated position
                    playerProjectile.speed_x = -8;                                               // Set the speed to a negative value
                    playerProjectile.active = true;
                }
            }
            npcProjectileTimer--;

            // Send NPC projectile at intervals
            if (npcProjectileTimer <= 0)
            {
                if (!npcProjectile.active)
                {
                    npcProjectile.x = npcShooter.x;
                    npcProjectile.y = npcShooter.y + npcShooter.height / 2 - npcProjectile.radius;
                    npcProjectile.speed_x = 4; // Set the speed for the NPC projectile
                    npcProjectile.active = true;
                    npcProjectileTimer = npcProjectileInterval; // Reset the timer
                }
            }

            ClearBackground(BLACK);
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
            // draww ball

            // render
            playerProjectile.Draw();
            npcProjectile.Draw();
            ball.Draw();
            npc.Draw();
            if (npcShooter.active) // Only draw if active
            {
                npcShooter.update_position_vertical();
                npcShooter.Draw();
            }
            player.Draw();

            DrawText(TextFormat("%i", npc_score), screen_width / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

            EndDrawing();
            if (player_score >= 7)
            {
                currentState = PLAYER_WON;
            }
            else if (npc_score >= 7)
            {
                currentState = NPC_WON;
            }
            else if (player_score < 0)
            {
                currentState = PLAYER_LOST;
            }
        }
        else
        {
            BeginDrawing();
            ClearBackground(BLACK);

            if (currentState == PLAYER_WON)
            {
                DrawText("Player Wins!", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2, 30, WHITE);
            }
            else if (currentState == NPC_WON)
            {
                DrawText("NPC Wins!", GetScreenWidth() / 2 - 90, GetScreenHeight() / 2, 30, WHITE);
            }
            else if (currentState == PLAYER_LOST)
            {
                BeginDrawing();
                ClearBackground(BLACK);
                DrawText("You Lost!", GetScreenWidth() / 2 - 50, GetScreenHeight() / 2, 30, RED);
                EndDrawing();
            }

            DrawText(TextFormat("Final Score: Player: %i, NPC: %i", player_score, npc_score), GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 + 40, 20, WHITE);

            EndDrawing();

            // Allow the user to press a key to exit or restart
            if (IsKeyPressed(KEY_ENTER))
            {
                // Reset scores and loop again if you want or just break to exit
                player_score = 0;
                npc_score = 0;
            }
        }
    }
    CloseWindow();
    return 0;
}