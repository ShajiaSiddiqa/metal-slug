#include <iostream>
#include "raylib.h"
#include <cstdlib> 

using namespace std;

const int SCREEN_WIDTH = 1150;
const int SCREEN_HEIGHT = 500;
const int MAX_ROCKETS = 3;

// ------------------------------------------------------------- {STRUCTS}

//struct Player
//{
//    Texture2D texture;
//    Vector2 position;
//    float speed;
//};

struct Player
{
    Texture2D textureRight;
    Texture2D textureLeft;
    Texture2D currentTexture;                // --->  Active texture based on direction
    Vector2 position;
    float speed;
    bool isJumping;
    bool isFacingRight;
};
struct Tank
{
    Texture2D texture;
    Vector2 position;
    float speed;
};
struct Rocket
{
    Texture2D texture;
    Vector2 position;
    float speed;
    Vector2 direction;
};

// ------------------------------------------------------------------ {INIT FUNCTIONS}

//  i added some changes in player, left right dir , changes images with the pressed key 

void InitPlayer(Player& player, int floor)
{
    player.textureRight = LoadTexture("player.png");  // Facing right
    player.textureLeft = LoadTexture("player2.png");  // Facing left

    if (player.textureRight.id == 0 || player.textureLeft.id == 0)
    {
        cout << "Failed to load player textures!\n";
        exit(EXIT_FAILURE);
    }

    player.textureRight.height = 100;
    player.textureRight.width = 100;
    player.textureLeft.width = 100;
    player.textureLeft.height = 100;
    player.currentTexture.width = 100;
    player.currentTexture.height = 100;

    player.currentTexture = player.textureRight;
    player.position = { 100, (float)floor };
    player.speed = 200.0f;
    player.isFacingRight = true;
    player.isJumping = false;

}
void InitTank(Tank& tank)
{
    tank.texture = LoadTexture("tank_enemy.png");

    if (tank.texture.id == 0)
    {
        cout << "Failed to load tank texture!\n";
        exit(EXIT_FAILURE);
    }

    tank.texture.height = 180;
    tank.texture.width = 180;

    tank.position = { SCREEN_WIDTH, float(SCREEN_HEIGHT - tank.texture.height - 10) };
    tank.speed = 100.0f;
}
void InitRocket(Rocket rocket[], Texture2D& rocketTexture, Player& player)
{
    if (rocketTexture.id == 0)
    {
        cout << "Failed to load tank texture!\n";
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MAX_ROCKETS; i++)
    {
        rocket[i].texture = rocketTexture;
        rocket[i].texture.height = 40;
        rocket[i].texture.width = 40;
        rocket[i].position = { (float)(rand() % (SCREEN_WIDTH - 40)), (float)(-(rand() % 300)) };

        rocket[i].speed = 100.0f;

        Vector2 direction;
        direction.x = player.position.x - rocket[i].position.x;              // Pythagorus theorem use kia ha player or rocket ka distance compute kernay k liay  
        direction.y = player.position.y - rocket[i].position.y;

        float length = sqrt(direction.x * direction.x + direction.y * direction.y);

        rocket[i].direction = { direction.x / length, direction.y / length };                   // ya constant speed k liay kia ha 
        // this is also the way to initialize, instead of seperately using dir.x = xyz... and dir.y = abc...
    }
}

// ------------------------------------------------------------------ {MOVE FUNCTIONS}

void MoveTank(Tank& tank)
{
    tank.position.x -= tank.speed * GetFrameTime();

    if (tank.position.x < -tank.texture.width)
    {
        tank.position.x = SCREEN_WIDTH;
    }
}
void MoveRocket(Rocket rocket[], Player& player)
{
    for (int i = 0; i < MAX_ROCKETS; i++)
    {
        rocket[i].position.x += rocket[i].direction.x * rocket[i].speed * GetFrameTime();
        rocket[i].position.y += rocket[i].direction.y * rocket[i].speed * GetFrameTime();

        if (rocket[i].position.y > SCREEN_HEIGHT)
        {
            rocket[i].position.x = rand() % (SCREEN_WIDTH - 40);
            rocket[i].position.y = -40;

            //  i copied it from initRocket function

            Vector2 direction;
            direction.x = player.position.x - rocket[i].position.x;
            direction.y = player.position.y - rocket[i].position.y;

            float length = sqrt(direction.x * direction.x + direction.y * direction.y);

            rocket[i].direction = { direction.x / length, direction.y / length };
        }
    }
}

// ------------------------------------------------------------------ {DRAW FUNCTIONS}

void DrawTank(Tank& tank)
{
    DrawTexture(tank.texture, tank.position.x, tank.position.y, WHITE);
}
void DrawRocket(Rocket rocket[])
{
    for (int i = 0; i < MAX_ROCKETS; i++)
    {
        DrawTexture(rocket[i].texture, rocket[i].position.x, rocket[i].position.y, WHITE);
    }
}
void DrawPlayer(Player& player)
{
    DrawTexture(player.currentTexture, player.position.x, player.position.y, WHITE);
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Metal Slug - Starter");
    InitAudioDevice();

    SetTargetFPS(60);

    Texture2D b1 = LoadTexture("b1_metal_slug.png");
    Texture2D b2 = LoadTexture("b2_metal_slug.png");
    Texture2D title = LoadTexture("title.png");
    Texture2D startWall = LoadTexture("StartWall.png");

    Texture2D rocketTexture = LoadTexture("crab_enemy.png");

    Sound shoot = LoadSound("gun_shoot.wav");

    /*if (shoot.stream == nullptr)                     // mujhay is ka check samajh nahi araha
    {
        cout << "Failed to load sound!\n";              // " == " per error ha, don't know Y?
        exit(EXIT_FAILURE);
    }*/



    Player player;
    int floor = 375;
    InitPlayer(player, floor);

    Player player2;
    Tank tank;
    InitTank(tank);
    Rocket rocket[MAX_ROCKETS];
    InitRocket(rocket, rocketTexture, player);
    b1.height = SCREEN_HEIGHT;
    b2.height = SCREEN_HEIGHT;
    b1.width = SCREEN_WIDTH;
    b2.width = SCREEN_WIDTH;

    int b1x = 0;
    int b2x = SCREEN_WIDTH;

    bool gamestarted = false;
    bool gameover = false;

    int jumpMax = 301;

    title.width = SCREEN_WIDTH;
    title.height = SCREEN_HEIGHT;
    startWall.width = SCREEN_WIDTH;
    startWall.height = SCREEN_HEIGHT;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        if (!gamestarted)
        {
            if (!gameover)
            {
                DrawTexture(title, SCREEN_WIDTH / 2 - title.width / 2, SCREEN_HEIGHT / 2 - title.height / 2, WHITE);
                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                {
                    gameover = true;
                }
            }
            else
            {
                DrawTexture(startWall, SCREEN_WIDTH / 2 - startWall.width / 2, SCREEN_HEIGHT / 2 - startWall.height / 2, WHITE);
                if (IsKeyPressed(KEY_ENTER))
                {
                    gamestarted = true;
                    gameover = false;

                }
            }
        }

        EndDrawing();



        if (gamestarted && !gameover)
        {
            while (!WindowShouldClose())
            {
                float deltaTime = GetFrameTime();
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawTexture(b1, b1x, 0, WHITE);
                DrawTexture(b2, b2x, 0, WHITE);


                if (IsKeyDown(KEY_RIGHT) && player.position.x + player.currentTexture.width < SCREEN_WIDTH)
                {
                    player.position.x += player.speed * deltaTime;
                    player.currentTexture = player.textureRight;    // Face right
                    player.isFacingRight = true;
                    b1x -= 2;
                    b2x -= 2;
                }

                if (IsKeyDown(KEY_LEFT) && player.position.x > 0)
                {
                    player.position.x -= player.speed * deltaTime;
                    player.currentTexture = player.textureLeft;    // Face left

                    PlaySound(shoot);                                   // for test 

                    player.isFacingRight = false;
                }

                if (IsKeyDown(KEY_UP)
                    && player.position.y == floor)
                {
                    player.isJumping = true;
                }
                if (player.isJumping)
                {
                    player.position.y -= 2;
                    if (player.position.y <= jumpMax)
                    {
                        player.position.y = jumpMax;
                        player.isJumping = false;
                    }
                }
                if (player.isJumping == false && player.position.y < floor and player.position.y >= jumpMax)
                {
                    player.position.y += 2;
                    if (player.position.y >= floor)
                    {
                        player.position.y = floor;
                    }
                }

                if (b1x <= -SCREEN_WIDTH)
                    b1x = SCREEN_WIDTH;

                if (b2x <= -SCREEN_WIDTH)
                    b2x = SCREEN_WIDTH;

                DrawTank(tank);
                MoveTank(tank);

                DrawRocket(rocket);
                MoveRocket(rocket, player);

                DrawPlayer(player);
                DrawPlayer(player2);

                EndDrawing();
            }

        }

    }

    UnloadTexture(player.textureRight);
    UnloadTexture(player.textureLeft);
    UnloadTexture(tank.texture);
    UnloadTexture(title);
    UnloadTexture(b1);
    UnloadTexture(b2);
    UnloadSound(shoot);

    CloseWindow();


    return 0;
}


