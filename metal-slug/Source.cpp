#include <iostream>
#include "raylib.h"
#include <cstdlib> 

using namespace std;

const int SCREEN_WIDTH = 1150;
const int SCREEN_HEIGHT = 500;
const int MAX_ROCKETS = 3;
const int MAX_BULLETS = 1;


// ------------------------------------------------------------- {STRUCTS}

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

struct Bullet
{
    Texture2D texture;
    Vector2 position;
    float speed;
    Vector2 direction;
    //Rectangle collider;
    bool active;
};

// ------------------------------------------------------------------ {LOAD TEXTURES}


// ------------------------------------------------------------------ {INIT FUNCTIONS}

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
        cout << "Failed to load Rocket texture!\n";
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
    }
}

void InitBullets(Bullet bullets[], Texture2D& bulletTexture)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].texture = bulletTexture;
        bullets[i].texture.height = 10;
        bullets[i].texture.width = 10;
        bullets[i].active = false;
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
void MoveBullets(Bullet bullets[])
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].position.x += bullets[i].direction.x * bullets[i].speed * GetFrameTime();
            bullets[i].position.y += bullets[i].direction.y * bullets[i].speed * GetFrameTime();

            // test 

            cout << "Bullet Moving: (" << bullets[i].position.x << ", " << bullets[i].position.y << ")" << endl;

            if (bullets[i].position.x < 0 || bullets[i].position.x > SCREEN_WIDTH ||
                bullets[i].position.y < 0 || bullets[i].position.y > SCREEN_HEIGHT)
            {
                bullets[i].active = false;

                // test

                cout << "Bullet Deactivated" << endl;
            }
        }
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
void DrawBullets(Bullet bullets[])
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            DrawTexture(bullets[i].texture, bullets[i].position.x, bullets[i].position.y, WHITE);

            // test 

            cout << "Drawing Bullet at: " << bullets[i].position.x << ", " << bullets[i].position.y << endl;
        }
    }
}
void DrawPlayer(Player& player)
{
    DrawTexture(player.currentTexture, player.position.x, player.position.y, WHITE);
}

// ------------------------------------------------------------------ {SHOOT FUNCTION}

void ShootBullet(Bullet bullets[], Texture2D& bulletTexture, Player& player, Vector2 direction)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active)  
        {
            bullets[i].texture = bulletTexture;
            bullets[i].position = { player.position.x, player.position.y /*+ 30 */};
            bullets[i].speed = 400.0f;
            bullets[i].direction = direction;
            bullets[i].active = true;

            // test 

            cout << "Bullet Fired at Position: " << bullets[i].position.x << ", " << bullets[i].position.y << endl;  // Debug line
            break;
        }
    }
}

void SwitchPlayerDirection(Player& player, float deltaTime, int& b1x, int& b2x)
{

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
        player.isFacingRight = false;
    }

}

void PlayerJump(Player& player, int floor, int jumpMax) 
{
    if (IsKeyDown(KEY_UP) && player.position.y == floor)
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

    if (!player.isJumping && player.position.y < floor && player.position.y >= jumpMax) 
    {
        player.position.y += 2;
        if (player.position.y >= floor) 
        {
            player.position.y = floor;
        }
    }
}
void ShootBulletAction(void (*Shoot)(Bullet[], Texture2D&, Player&, Vector2), Bullet bullets[],
    Texture2D& bulletTexture, Player& player, Vector2 direction, Sound shoot) 
{
    Shoot(bullets, bulletTexture, player, direction);
    PlaySound(shoot);
}


void ShootBasedOnKeyPress(Bullet bullets[], Texture2D& bulletTexture, Player& player,
    Sound shoot, void (*Shoot)(Bullet[], Texture2D&, Player&, Vector2)) 
{
    if (IsKeyPressed(KEY_A) && !player.isFacingRight) 
    {
        cout << "A key pressed!" << endl;
        ShootBulletAction(Shoot, bullets, bulletTexture, player, { -1.0f, 0.0f }, shoot);
    }
    if (IsKeyPressed(KEY_D) && player.isFacingRight) 
    {
        cout << "D key pressed!" << endl;
        ShootBulletAction(Shoot, bullets, bulletTexture, player, { 1.0f, 0.0f }, shoot);
    }
    if (IsKeyPressed(KEY_W))
    {
        cout << "W key pressed!" << endl;
        ShootBulletAction(Shoot, bullets, bulletTexture, player, { 0.0f, -1.0f }, shoot);
    }
    if (IsKeyPressed(KEY_S)) 
    {
        cout << "S key pressed!" << endl;
        ShootBulletAction(Shoot, bullets, bulletTexture, player, { 0.0f, 1.0f }, shoot);
    }
}

// ----------------------------------------------------------------------- (MAIN)

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Metal Slug - Starter");
    InitAudioDevice();

    SetTargetFPS(83);

    Texture2D b1 = LoadTexture("b1_metal_slug.png");
    Texture2D b2 = LoadTexture("b2_metal_slug.png");
    Texture2D title = LoadTexture("title.png");
    Texture2D startWall = LoadTexture("StartWall.png");
    Texture2D bulletTextureLeft = LoadTexture("bulletLeft.png");
    Texture2D bulletTextureRight = LoadTexture("bulletRight.png");
    Texture2D rocketTexture = LoadTexture("crab_enemy.png");
    Sound shoot = LoadSound("gun_shoot.wav");

    Bullet bullets[MAX_BULLETS];
    Rocket rocket[MAX_ROCKETS];

    void (*Shoot)(Bullet[], Texture2D&, Player&, Vector2 );

    Player player;
    int floor = 375;
    InitPlayer(player, floor);

    Player player2;
    Tank tank;
    InitTank(tank);
    InitRocket(rocket, rocketTexture, player);

    InitBullets(bullets, bulletTextureLeft);
    Shoot = ShootBullet;

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
                DrawTexture(title, SCREEN_WIDTH / 2 - title.width / 2, SCREEN_HEIGHT / 2
                    - title.height / 2, WHITE);
                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                {
                    gameover = true;
                }
            }
            else
            {
                DrawTexture(startWall, SCREEN_WIDTH / 2 - startWall.width / 2, 
                    SCREEN_HEIGHT / 2 - startWall.height / 2, WHITE);
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

                SwitchPlayerDirection(player, deltaTime, b1x, b2x);

                PlayerJump(player, floor, jumpMax);

                if (b1x <= -SCREEN_WIDTH)
                    b1x = SCREEN_WIDTH;

                if (b2x <= -SCREEN_WIDTH)
                    b2x = SCREEN_WIDTH;

                ShootBasedOnKeyPress(bullets, bulletTexture, player, shoot, Shoot);

                MoveBullets(bullets);
                DrawBullets(bullets);

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


