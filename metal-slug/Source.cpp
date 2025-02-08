#include <iostream>
#include "raylib.h"
#include <cstdlib> 
#include <cmath>
using namespace std;

const int SCREEN_WIDTH = 1150;
const int SCREEN_HEIGHT = 500;
const int MAX_ROCKETS = 3;
const int MAX_BULLETS = 1;
const int MAX_ENEMIES = 3;
const int MAX_HELICOPTER = 2;

// ------------------------------------------------------------- {STRUCTS}
struct Tank
{
    Texture2D texture;
    Vector2 position;
    float speed;
    Rectangle collider;
};
struct Barrel
{
    Texture2D texture;
    Vector2 position;
    float speed;
    Rectangle collider;
};
struct Enemy
{
    Texture2D texture;                // --->  Active texture based on direction
    Vector2 position;
    float speed;
    Rectangle collider;
    bool active;
};
struct Player
{
    Texture2D textureRight;
    Texture2D textureLeft;
    Texture2D textureUp;
    Texture2D currentTexture;                // --->  Active texture based on direction
    Vector2 position;
    float speed;
    bool isJumping;
    bool isFacingRight;
    Rectangle collider;
};
struct Rocket
{
    Texture2D texture;
    Vector2 position;
    float speed;
    Rectangle collider;
    Vector2 direction;
};
struct Bullet
{
    Texture2D texture;
    Vector2 position;
    float speed;
    Vector2 direction;
    Rectangle collider;
    bool active;
};
struct Helicopter
{
    Texture2D texture;
    Vector2 position;
    float speed;
    Rectangle collider;
    Vector2 direction;
};
// ------------------------------------------------------------------ {INIT FUNCTIONS}

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
void InitPlayer(Player& player, int floor)
{
    player.textureRight = LoadTexture("player.png");  // Facing right
    player.textureLeft = LoadTexture("player2.png");  // Facing left
    player.textureUp = LoadTexture("playerUp.png");
    if (player.textureRight.id == 0 || player.textureLeft.id == 0)
    {
        cout << "Failed to load player textures!\n";
        exit(EXIT_FAILURE);
    }

    player.textureRight.height = 100;
    player.textureRight.width = 100;
    player.textureLeft.width = 100;
    player.textureLeft.height = 100;
    player.textureUp.width = 100;
    player.textureUp.height = 100;
    player.currentTexture.width = 100;
    player.currentTexture.height = 100;

    player.currentTexture = player.textureRight;
    player.position = { 100, (float)floor };
    player.speed = 200.0f;
    player.isFacingRight = true;
    player.isJumping = false;

}
void InitBullets(Bullet bullets[], Texture2D& bulletTexture)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].texture = bulletTexture;
        bullets[i].texture.height = 12;
        bullets[i].texture.width = 12;
        bullets[i].active = false;
        bullets[i].position = { -100, -120 }; 
        bullets[i].collider = { bullets[i].position.x, bullets[i].position.y , 12,12};
    }
}
void InitEnemy(Enemy enemy[], Texture2D enemyTexture, int floor)
{

    if (enemyTexture.id == 0)
    {
        cout << "Failed to load player textures!\n";
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        enemy[i].texture = enemyTexture;
        enemy[i].texture.height = 100;
        enemy[i].texture.width = 100;
        enemy[i].position = { SCREEN_WIDTH - 10, (float)floor };
        enemy[i].collider = { enemy[i].position.x, enemy[i].position.y , 100 ,100 };
        enemy[i].speed = 200.0f;
        enemy[i].active = true;
    }
}
void InitRocket(Rocket rocket[], Texture2D& rocketTexture, Player& player)
{
    if (rocketTexture.id == 0)
    {
        cout << "Failed to load Rocket texture!\n";
        exit(EXIT_FAILURE);
    }
    float rocketPosX = (rand() % (SCREEN_WIDTH - 40));
    float rocketPosY = -(rand() % 300);

    for (int i = 0; i < MAX_ROCKETS; i++)
    {
        rocket[i].texture = rocketTexture;
        rocket[i].texture.height = 40;
        rocket[i].texture.width = 40;
        rocket[i].position = { rocketPosX, rocketPosY };
        rocket[i].collider = { rocket[i].position.x,rocket[i].position.y , 40 ,40 };

        rocket[i].speed = 100.0f;

        Vector2 direction;
        direction.x = player.position.x - rocket[i].position.x;              // Pythagorus theorem use kia ha player or rocket ka distance compute kernay k liay  
        direction.y = player.position.y - rocket[i].position.y;

        float length = sqrt(direction.x * direction.x + direction.y * direction.y);

        rocket[i].direction = { direction.x / length, direction.y / length };                   // ya constant speed k liay kia ha 
    }
}
void InitHelicopter(Helicopter helicopter[], Texture2D& helicopterTexture, Player& player)
{
    if (helicopterTexture.id == 0)
    {
        cout << "Failed to load Helicopter texture!\n";
        exit(EXIT_FAILURE);
    }
    float rocketPosX = (rand() % (SCREEN_WIDTH - 40));
    float rocketPosY = -(rand() % 300);

    for (int i = 0; i < MAX_ROCKETS; i++)
    {
        helicopter[i].texture = helicopterTexture;
        helicopter[i].texture.height = 40;
        helicopter[i].texture.width = 40;
        helicopter[i].position = { rocketPosX, rocketPosY };
        helicopter[i].collider = { helicopter[i].position.x,helicopter[i].position.y , 200 ,200 };

        helicopter[i].speed = 100.0f;

        Vector2 direction;
        direction.x = player.position.x - helicopter[i].position.x;              // Pythagorus theorem use kia ha player or rocket ka distance compute kernay k liay  
        direction.y = player.position.y - helicopter[i].position.y;

        float length = sqrt(direction.x * direction.x + direction.y * direction.y);

        helicopter[i].direction = { direction.x / length, direction.y / length };                   // ya constant speed k liay kia ha 
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
void MoveEnemy(Enemy enemy[])
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        enemy[i].position.x -= enemy[i].speed * GetFrameTime();
        enemy[i].collider.x = enemy[i].position.x;
        enemy[i].collider.y = enemy[i].position.y;

        if (enemy[i].position.x < -enemy[i].texture.width)
        {
            enemy[i].position.x = SCREEN_WIDTH;
        }
    }
}
void MoveBarrel(Barrel& barrel)
{
    barrel.position.x -= barrel.speed * GetFrameTime();
    if (barrel.position.x < -barrel.texture.width)
    {
        barrel.position.x = SCREEN_WIDTH;
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
            bullets[i].collider.x = bullets[i].position.x;
            bullets[i].collider.y = bullets[i].position.y;
                
            if (bullets[i].position.x < -50 || bullets[i].position.x > SCREEN_WIDTH + 50 ||
                bullets[i].position.y < -50 || bullets[i].position.y > SCREEN_HEIGHT + 50)
            {
                bullets[i].active = false;
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
        rocket[i].collider.x = rocket[i].position.x;
        rocket[i].collider.y = rocket[i].position.y;

        if (rocket[i].position.y > SCREEN_HEIGHT)
        {
            rocket[i].position.x = rand() % (SCREEN_WIDTH - 40);
            rocket[i].position.y = -40;

            Vector2 direction;
            direction.x = player.position.x - rocket[i].position.x;
            direction.y = player.position.y - rocket[i].position.y;

            float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length != 0)
            {
                direction.x /= length;
                direction.y /= length;
            }

            rocket[i].direction = direction;
        }
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
void MoveHelicopter(Helicopter helicopter[], Player& player)
{
    for (int i = 0; i < MAX_ROCKETS; i++)
    {
        helicopter[i].position.y += helicopter[i].direction.y * helicopter[i].speed * GetFrameTime();
        helicopter[i].position.x += helicopter[i].direction.x * helicopter[i].speed * GetFrameTime();
        helicopter[i].collider.x = helicopter[i].position.x;
        helicopter[i].collider.y = helicopter[i].position.y;

        if (helicopter[i].position.y > SCREEN_HEIGHT)
        {
            helicopter[i].position.x = rand() % (SCREEN_WIDTH - 40);
            helicopter[i].position.y = -40;

            Vector2 direction;
            direction.x = player.position.x - helicopter[i].position.x;
            direction.y = player.position.y - helicopter[i].position.y;

            float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length != 0)
            {
                direction.x /= length;
                direction.y /= length;
            }

            helicopter[i].direction = direction;
        }
    }
}

// ------------------------------------------------------------------ {COLLISION}

void CheckCollision(Bullet bullet[], Rocket rocket[], int &score)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullet[i].active)
        {
            for (int j = 0; j < MAX_ROCKETS; j++)
            {
                if (CheckCollisionRecs(bullet[i].collider, rocket[j].collider))
                {
                    bullet[i].active = false;
                    float rocketPosX = ((SCREEN_WIDTH - 40));
                    float rocketPosY = -(rand() % 300);
                    score++;
                    rocket[j].position = { rocketPosX,rocketPosY };
                    rocket[j].collider = { rocket[j].position.x ,rocket[j].position.y ,40,40};
                }
            }
        }
    }
}
void CheckCollisionWithEnemy(Bullet bullet[], Enemy enemy[], int& score)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullet[i].active)
        {
            for (int j = 0; j < MAX_ENEMIES; j++)
            {
                if (CheckCollisionRecs(bullet[i].collider, enemy[j].collider))
                {
                    bullet[i].active = false;
                    float enemyPosX = SCREEN_WIDTH;
                    float enemyPosY = 375;
                    enemy[j].active = false;
                    enemy[j].position = { enemyPosX,enemyPosY };
                    enemy[j].collider = { enemy[j].position.x ,enemy[j].position.y ,100, 100 };
                    score++;
                    cout << "Enemy killed! Score: " << score << endl;
                    DrawRectangle(enemy[j].collider.x, enemy[j].collider.y, 100, 100, RED);
                }
            }
        }
    }
}
void CheckCollisionWithHelicopter(Bullet bullet[], Helicopter helicopter[], int& score)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullet[i].active)
        {
            for (int j = 0; j < MAX_ROCKETS; j++)
            {
                if (CheckCollisionRecs(bullet[i].collider, helicopter[j].collider))
                {
                    bullet[i].active = false;
                    float heliPosX = ((SCREEN_WIDTH - 40));
                    float heliPosY = -(rand() % 300);
                    score++;
                    helicopter[j].position = { heliPosX,heliPosY };
                    helicopter[j].collider = { helicopter[j].position.x ,helicopter[j].position.y ,250,250 };
                }
            }
        }
    }
}

// ------------------------------------------------------------------ {DRAW FUNCTIONS}

void DrawTank(Tank& tank)
{
    DrawTexture(tank.texture, tank.position.x, tank.position.y, WHITE);
}
void DrawEnemy(Enemy enemy[])
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        DrawTexture(enemy[i].texture, enemy[i].position.x, enemy[i].position.y, WHITE);
    }
}
void DrawBarrel(Barrel& barrel)
{
    DrawTexture(barrel.texture, barrel.position.x, barrel.position.y, WHITE);
}
void DrawPlayer(Player& player)
{
    DrawTexture(player.currentTexture, player.position.x, player.position.y, WHITE);
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
            DrawTexturePro(
                bullets[i].texture,
                { 0, 0, (float)bullets[i].texture.width, (float)bullets[i].texture.height },
                { bullets[i].position.x, bullets[i].position.y, 20, 20 },
                { 0, 0 }, 0, WHITE);
        }
    }
}
void DrawHelicopter(Helicopter helicopter[])
{
    for (int i = 0; i < MAX_HELICOPTER; i++)
    {
        DrawTexture(helicopter[i].texture, helicopter[i].position.x, helicopter[i].position.y, WHITE);
    }
}

// ------------------------------------------------------------------ {SHOOT FUNCTION}

void ShootBullet(Bullet bullets[], Texture2D& bulletTextureLeft, Texture2D& bulletTextureRight, Player& player, Vector2 direction)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active)
        {
            float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length != 0)
            {
                direction.x /= length;
                direction.y /= length;
            }
            if (player.isFacingRight)
            {
                bullets[i].position.x = player.position.x + 60;
                bullets[i].texture = bulletTextureRight;
            }
            else
            {
                bullets[i].position.x = player.position.x - 20;
                bullets[i].texture = bulletTextureLeft;
            }
            bullets[i].position.y = player.position.y + 20;
            bullets[i].speed = 600.0f;
            bullets[i].direction = direction;
            bullets[i].active = true;
            break;
        }
    }
}
void SwitchPlayerDirection(Player& player, float deltaTime, int& b1x, int& b2x)
{
    if (IsKeyDown(KEY_RIGHT) && player.position.x + player.currentTexture.width < SCREEN_WIDTH)
    {
        player.position.x += player.speed * deltaTime;
        player.currentTexture = player.textureRight;        // Face right
        player.isFacingRight = true;
        b1x -= 2;
        b2x -= 2;
    }
    if (IsKeyDown(KEY_LEFT) && player.position.x > 0)
    {
        player.position.x -= player.speed * deltaTime;
        player.currentTexture = player.textureLeft;         // Face left
        player.isFacingRight = false;
    }
}
void ShootBulletAction(void (*Shoot)(Bullet[], Texture2D&, Player&, Vector2), Bullet bullets[],
    Texture2D& bulletTexture, Player& player, Vector2 direction, Sound shoot)
{
    Shoot(bullets, bulletTexture, player, direction);
    PlaySound(shoot);
}
void ShootBasedOnKeyPress(Bullet bullets[], Texture2D& bulletTextureLeft, Texture2D& bulletTextureRight, Player& player,
    Sound shoot, void (*Shoot)(Bullet[], Texture2D&, Texture2D&, Player&, Vector2))
{
    if (IsKeyDown(KEY_A))
    {
        player.currentTexture = player.textureLeft;
        DrawPlayer(player);
        Shoot(bullets, bulletTextureLeft, bulletTextureRight, player, { -1.0f, 0.0f });
        PlaySound(shoot);
    }
    if (IsKeyDown(KEY_D))
    {
        player.currentTexture = player.textureRight;
        DrawPlayer(player);
        Shoot(bullets, bulletTextureLeft, bulletTextureRight, player, { 1.0f, 0.0f });
        PlaySound(shoot);
    }
    if (IsKeyDown(KEY_W))
    {
        player.currentTexture = player.textureUp;
        DrawPlayer(player);
        Shoot(bullets, bulletTextureLeft, bulletTextureRight, player, { 0.0f, -1.0f });
        PlaySound(shoot);
    }
    if (IsKeyDown(KEY_S))
    {
        Shoot(bullets, bulletTextureLeft, bulletTextureRight, player, { 0.0f, 1.0f });
        PlaySound(shoot);
    }
}

// ----------------------------------------------------------------------- (MAIN)

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Metal Slug - Starter");
    SetTargetFPS(83);
    InitAudioDevice();

    Sound shoot = LoadSound("gun_shoot.wav");
    Texture2D title = LoadTexture("title.png");
    Texture2D enemy = LoadTexture("enemy.png");
    Texture2D barrel1 = LoadTexture("barrel.png");
    Texture2D level2 = LoadTexture("level2.png");
    Texture2D b1 = LoadTexture("b1_metal_slug.png");
    Texture2D b2 = LoadTexture("b2_metal_slug.png");
    Texture2D b3 = LoadTexture("b3_metal_slug.png");
    Texture2D b4 = LoadTexture("b4_metal_slug.png");
    Texture2D startWall = LoadTexture("StartWall.png");
    Texture2D helicopter1 = LoadTexture("helicopter.png");
    Texture2D rocketTexture = LoadTexture("crab_enemy.png");
    Texture2D bulletTextureLeft = LoadTexture("bulletLeft.png");
    Texture2D bulletTextureRight = LoadTexture("bulletRight.png");

    Bullet bullets[MAX_BULLETS];
    Rocket rocket[MAX_ROCKETS];
    Enemy Enemy[MAX_ENEMIES];

    Tank tank;
    Player player;
    Player player2;
    Barrel barrel;
    Helicopter helicopter[MAX_HELICOPTER];

    int b1x = 0;
    int b2x = SCREEN_WIDTH;
    int b3x = 0;
    int b4x = SCREEN_WIDTH;
    int floor = 375;
    int score = 0;
    int health = 20;
    int jumpMax = 301;

    bool gameover = false;
    bool gamestarted = false;
    bool startLevel2 = false;

    float healthBarWidth = 300;
    float healthBarHeight = 30;
    float healthPercentage = (float)health / 20;

    b1.width = SCREEN_WIDTH;
    b2.width = SCREEN_WIDTH;
    b3.width = SCREEN_WIDTH;
    b4.width = SCREEN_WIDTH;
    b1.height = SCREEN_HEIGHT;
    b2.height = SCREEN_HEIGHT;
    b3.height = SCREEN_HEIGHT;
    b4.height = SCREEN_HEIGHT;
    title.width = SCREEN_WIDTH;
    title.height = SCREEN_HEIGHT;
    level2.width = SCREEN_WIDTH;
    level2.height = SCREEN_HEIGHT;
    startWall.width = SCREEN_WIDTH;
    startWall.height = SCREEN_HEIGHT;

    void (*Shoot)(Bullet[], Texture2D&, Texture2D&, Player&, Vector2) = ShootBullet;
    Shoot = ShootBullet;

    InitTank(tank);
    InitPlayer(player, floor);
    InitEnemy(Enemy, enemy, floor);
    InitRocket(rocket, rocketTexture, player);
    InitBullets(bullets, bulletTextureRight);
    InitHelicopter(helicopter,helicopter1, player);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        if (gameover)
        {
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100, 50, RED);
            DrawText(TextFormat("Final Score: %d", score), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 30, DARKGRAY);
            DrawText("Press ENTER to Restart or ESC to Quit", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 50, 20, DARKGRAY);
        }

        if (!gamestarted)
        {
            if (!gameover)
            {
                DrawTexture(title, SCREEN_WIDTH / 2 - title.width / 2, SCREEN_HEIGHT / 2
                    - title.height / 2, WHITE);
                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                {
                    gamestarted = true;
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

                MoveTank(tank);
                MoveEnemy(Enemy);
                MoveBullets(bullets);
                MoveRocket(rocket, player);
                PlayerJump(player, floor, jumpMax);
                MoveHelicopter(helicopter, player);
                CheckCollision(bullets, rocket, score);
                CheckCollisionWithEnemy(bullets, Enemy, score);
                CheckCollisionWithHelicopter(bullets, helicopter, score);
                ShootBasedOnKeyPress(bullets, bulletTextureRight, bulletTextureRight, player, shoot, Shoot);

                // .:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.
                //                      LEVEL # 01
                // .:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.

                if (score >= 0 && score <= 10)
                {
                    DrawTexture(b1, b1x, 0, WHITE);
                    DrawTexture(b2, b2x, 0, WHITE);
                    SwitchPlayerDirection(player, deltaTime, b1x, b2x);

                    if (b1x <= -SCREEN_WIDTH)
                        b1x = SCREEN_WIDTH;

                    if (b2x <= -SCREEN_WIDTH)
                        b2x = SCREEN_WIDTH;

                    DrawBullets(bullets);
                    DrawRocket(rocket);
                    DrawPlayer(player);
                    DrawPlayer(player2);
                    DrawEnemy(Enemy);
                }

                // .:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.
                //                      LEVEL # 02
                // .:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.

                if (score >= 10 && !startLevel2) 
                {
                    DrawTexture(level2, SCREEN_WIDTH / 2 - level2.width / 2, SCREEN_HEIGHT / 2 - level2.height / 2, WHITE);
                    DrawText("Press ENTER to Start Level 2", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 + 50, 20, GREEN);

                    if (IsKeyPressed(KEY_ENTER)) 
                    {
                        startLevel2 = true;
                    }
                }
                if (startLevel2)
                {
                    DrawTexture(b3, b3x, 0, WHITE);
                    DrawTexture(b4, b4x, 0, WHITE);
                    SwitchPlayerDirection(player, deltaTime, b3x, b4x);

                    DrawPlayer(player);
                    DrawPlayer(player2);
                    DrawBullets(bullets);
                    DrawHelicopter(helicopter);

                    if (b3x <= -SCREEN_WIDTH)
                        b3x = SCREEN_WIDTH;

                    if (b4x <= -SCREEN_WIDTH)
                        b4x = SCREEN_WIDTH;

                    if (score >= 15)
                    {
                        DrawBarrel(barrel);
                        MoveBarrel(barrel);
                    }
                }
                
                DrawText(TextFormat("SCORE : %d", score), 7, 7, 30, GREEN);
                DrawRectangle(SCREEN_WIDTH - healthBarWidth - 10, 10, healthBarWidth, healthBarHeight, DARKGRAY);                  // Background
                DrawRectangle(SCREEN_WIDTH - healthBarWidth - 10, 10, healthBarWidth * healthPercentage, healthBarHeight, GREEN);  // Health Bar
                DrawText(TextFormat("HEALTH: %d", health), SCREEN_WIDTH - 250, 7, 40, RED);
                // health functions are to be added yet

                if (health <= 0)
                {
                    gameover = true;
                    break;
                }

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
    UnloadTexture(b3);
    UnloadTexture(b4);
    UnloadSound  (shoot);

    CloseWindow();

    return 0;
}

