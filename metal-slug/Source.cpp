#include <iostream>
#include "raylib.h"
#include <cstdlib> 

using namespace std;

const int SCREEN_WIDTH = 1150;
const int SCREEN_HEIGHT = 500;

        // ------------------------------------------------------------- {STRUCTS}

struct Player
{
    Texture2D texture;
    Vector2 position;
    float speed;
};

struct Tank
{
    Texture2D texture;
    Vector2 position;
    float speed;
};

struct Crab
{
    Texture2D texture;
    Vector2 position;
    float speed;
};

// ------------------------------------------------------------------ {INIT FUNCTIONS}

void InitPlayer(Player& player)
{
    player.texture = LoadTexture("player.png");

    if (player.texture.id == 0)                                         // player Loading main issue tha @ Miss KB;
    {
        cout << "Failed to load player texture!\n";
        exit(EXIT_FAILURE);
    }

    player.texture.height = 160;
    player.texture.width = 140;
    player.position = { 100, float(SCREEN_HEIGHT - player.texture.height - 25) };
    player.speed = 200.0f;
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

void InitCrab(Crab& crab)
{
    crab.texture = LoadTexture("crab_enemy.png");

    if (crab.texture.id == 0)
    {
        cout << "Failed to load tank texture!\n";
        exit(EXIT_FAILURE);
    }
    crab.texture.height = 40;
    crab.texture.width = 40;
    crab.position = { SCREEN_WIDTH, float(SCREEN_HEIGHT - crab.texture.height - 10) };
    crab.speed = 100.0f;
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

void MoveCrab(Crab& crab)
{
    crab.position.y += crab.speed * GetFrameTime(); 

    if (crab.position.y > SCREEN_HEIGHT)
    {
        crab.position.y = crab.texture.height;
        crab.position.x = rand() % (SCREEN_WIDTH - crab.texture.width);
    }
}

// ------------------------------------------------------------------ {DRAW FUNCTIONS}

void DrawTank(Tank& tank)
{
    DrawTexture(tank.texture, tank.position.x, tank.position.y, WHITE);
}

void DrawCrab(Crab& crab)
{
    DrawTexture(crab.texture, crab.position.x, crab.position.y, WHITE);
}

void DrawPlayer(Player& player)
{
    DrawTexture(player.texture, player.position.x, player.position.y, WHITE);
}



int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Metal Slug - Starter");
    SetTargetFPS(60);

    Player player;
    InitPlayer(player);

    Tank tank;
    InitTank(tank);

    Crab crab;
    InitCrab(crab);

    Texture2D b1 = LoadTexture("b1_metal_slug.png");
    Texture2D b2 = LoadTexture("b2_metal_slug.png");
    Texture2D title = LoadTexture("title.png");

    b1.height = SCREEN_HEIGHT;
    b2.height = SCREEN_HEIGHT;
    b1.width = SCREEN_WIDTH;
    b2.width = SCREEN_WIDTH;

    int b1x = 0;
    int b2x = SCREEN_WIDTH;

    bool gamestarted = false;
    bool gameover = false;
    
    title.width = SCREEN_WIDTH;
    title.height = SCREEN_HEIGHT;

    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);

        if (!gamestarted)
        {
            DrawTexture(title, SCREEN_WIDTH / 2 - title.width / 2, SCREEN_HEIGHT / 2 - title.height / 2, WHITE);

            if (IsKeyPressed(KEY_ENTER))
            {
                gamestarted = true;
                gameover = false;
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


                /*if (IsKeyDown(KEY_RIGHT))
                {
                    player.position.x += player.speed * deltaTime;
                    b1x -= 2;
                    b2x -= 2;
                }
                if (IsKeyDown(KEY_LEFT))
                {
                    player.position.x -= player.speed * deltaTime;

                }*/

                                                                                     // I have Added boundary checks @ Miss KB

                if (IsKeyDown(KEY_RIGHT) && player.position.x + player.texture.width < SCREEN_WIDTH)
                {
                    player.position.x += player.speed * deltaTime;
                    b1x -= 2;
                    b2x -= 2;
                }

                if (IsKeyDown(KEY_LEFT) && player.position.x > 0)
                {
                    player.position.x -= player.speed * deltaTime;
                }

                if (b1x <= -SCREEN_WIDTH)
                    b1x = SCREEN_WIDTH;

                if (b2x <= -SCREEN_WIDTH)
                    b2x = SCREEN_WIDTH;

                DrawTank(tank);  
                MoveTank(tank);  

                DrawCrab(crab);
                MoveCrab(crab);

                DrawPlayer(player);

                DrawText("Move with Arrow Keys", 10, 10, 20, DARKGRAY);

                EndDrawing();
            }

        }

    }

    UnloadTexture(player.texture);
    UnloadTexture(tank.texture);
    UnloadTexture(title);
    UnloadTexture(b1);
    UnloadTexture(b2);
    CloseWindow();

    
    return 0;
}


