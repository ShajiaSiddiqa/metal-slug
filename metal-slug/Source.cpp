// https://github.com/ShajiaSiddiqa/metal-slug/commits/master/

#include <iostream>
#include "raylib.h"
#include <cstdlib> 
#include <cmath>
using namespace std;

const int SCREEN_WIDTH = 1150;
const int SCREEN_HEIGHT = 500;
const int MAX_TANK = 1;
const int MAX_DRONES = 3;
const int MAX_ROCKETS = 3;
const int MAX_BULLETS = 3;
const int MAX_ENEMIES = 1;
const int MAX_HELICOPTER = 2;
const int MAX_DRONE_BULLETS = 3;

// ------------------------------------------------------------- {STRUCTS}

struct Tank
{
    Texture2D texture;
    Vector2 position;
    float speed;
    Rectangle collider;
};
struct Drone
{
    Texture2D texture;
    Vector2 position;
    float speed;
    Rectangle collider;
    bool active;
    float verticalDirection;  // 1 for down, -1 for up
    float timeSinceLastShot;
    float shootInterval;
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
struct Barrel
{
    Texture2D texture;
    Vector2 position;
    float speed;
    Rectangle collider;
};
struct Helicopter
{
    Texture2D texture;
    Vector2 position;
    float speed;
    Rectangle collider;
    Vector2 direction;
};
struct DroneBullet 
{
    Vector2 position;
    Vector2 direction;
    float speed;
    bool active;
    Rectangle collider;
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

    tank.texture.height = 150;
    tank.texture.width = 150;

    tank.position = { SCREEN_WIDTH, float(SCREEN_HEIGHT - tank.texture.height - 10) };
    tank.collider = { tank.position.x, tank.position.y, (float)tank.texture.width, (float)tank.texture.height };
    tank.speed = 100.0f;
}
void InitBarrel(Barrel& barrel)
{
    barrel.texture = LoadTexture("barrel.png");

    if (barrel.texture.id == 0)
    {
        cout << "Failed to load barrel texture!\n";
        exit(EXIT_FAILURE);
    }

    barrel.texture.height = 70;
    barrel.texture.width = 70;
    barrel.position = { SCREEN_WIDTH, float(SCREEN_HEIGHT - barrel.texture.height - 10) };
    barrel.collider = { barrel.position.x, barrel.position.y };
    barrel.speed =200.0f;
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

    player.textureUp.width = 80;
    player.textureUp.height = 80;
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
void InitDroneBullets(DroneBullet droneBullets[])
{
    for (int i = 0; i < MAX_DRONE_BULLETS; i++) 
    {
        droneBullets[i].position = { -100, -100 };
        droneBullets[i].speed = 300.0f;
        droneBullets[i].active = false;
        droneBullets[i].collider = { -100, -100, 10, 10 };
    }
}
void InitDrone(Drone drones[], Texture2D& droneTexture) 
{
    if (droneTexture.id == 0) 
    {
        cout << "Failed to load drone texture!\n";
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MAX_DRONES; i++) 
    {
        drones[i].texture = droneTexture;
        drones[i].texture.height = 140;
        drones[i].texture.width = 140;
        drones[i].position = { SCREEN_WIDTH + (float)(i * 200), (float)(100 + rand() % 200) };
        drones[i].collider = { drones[i].position.x, drones[i].position.y, 80, 80 };
        drones[i].speed = 150.0f;
        drones[i].active = true;
        drones[i].verticalDirection = 1;
        drones[i].timeSinceLastShot = 0;
        drones[i].shootInterval = 2.0f + (rand() % 2); // Random interval between 2-3 seconds
    }
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

    for (int i = 0; i < MAX_HELICOPTER; i++)
    {
        helicopter[i].texture = helicopterTexture;
        helicopter[i].texture.height = 150;
        helicopter[i].texture.width = 150;
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
    barrel.collider = { barrel.position.x, barrel.position.y, 90, 90 };

    if (barrel.position.x < -barrel.texture.width)
    {
        barrel.position.x = SCREEN_WIDTH + (rand() % 300);
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
void MoveDrones(Drone drones[], float deltaTime) 
{
    for (int i = 0; i < MAX_DRONES; i++) 
    {
        if (drones[i].active) {
            drones[i].position.x -= drones[i].speed * deltaTime;
            drones[i].position.y += drones[i].speed * 0.5f * deltaTime * drones[i].verticalDirection;
            if (drones[i].position.y > SCREEN_HEIGHT - 100) drones[i].verticalDirection = -1;
            if (drones[i].position.y < 50) drones[i].verticalDirection = 1;
            drones[i].collider.x = drones[i].position.x;
            drones[i].collider.y = drones[i].position.y;
            if (drones[i].position.x < -drones[i].texture.width) 
            {
                drones[i].position.x = SCREEN_WIDTH + (float)(rand() % 200);
                drones[i].position.y = (float)(100 + rand() % 200);
            }
            drones[i].timeSinceLastShot += deltaTime;
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
    Sound Jump = LoadSound("playerJumping.wav");
    if (IsKeyDown(KEY_UP) && player.position.y == floor)
    {
        player.isJumping = true;
    }
    if (player.isJumping)
    {
        PlaySound(Jump);
        player.position.y -= 2;
        if (player.position.y <= jumpMax)
        {
            player.position.y = jumpMax;
            player.isJumping = false;
            StopSound(Jump);
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
    for (int i = 0; i < MAX_HELICOPTER; i++)
    {
        helicopter[i].position.y += helicopter[i].direction.y * helicopter[i].speed * GetFrameTime();
        helicopter[i].position.x += helicopter[i].direction.x * helicopter[i].speed * GetFrameTime();
        helicopter[i].collider.x = helicopter[i].position.x;
        helicopter[i].collider.y = helicopter[i].position.y;
        if (helicopter[i].position.y > SCREEN_HEIGHT)
        {
            helicopter[i].position.x = rand() % (SCREEN_WIDTH - 100);
            helicopter[i].position.y = -100; 
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
void MoveDroneBullets(DroneBullet droneBullets[], float deltaTime)
{
    for (int i = 0; i < MAX_DRONE_BULLETS; i++) {

        if (droneBullets[i].active) 
        {
            droneBullets[i].position.x += droneBullets[i].direction.x * droneBullets[i].speed * deltaTime;
            droneBullets[i].position.y += droneBullets[i].direction.y * droneBullets[i].speed * deltaTime;
            droneBullets[i].collider.x = droneBullets[i].position.x;
            droneBullets[i].collider.y = droneBullets[i].position.y;
            if (droneBullets[i].position.x < 0 || droneBullets[i].position.x > SCREEN_WIDTH ||
                droneBullets[i].position.y < 0 || droneBullets[i].position.y > SCREEN_HEIGHT) 
            {
                droneBullets[i].active = false;
            }
        }
    }
}

// ------------------------------------------------------------------ {COLLISION}

void CheckCollision(Bullet bullet[], Rocket rocket[], int& score)
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
                    score += 1; 
                    rocket[j].position = { rocketPosX,rocketPosY };
                    rocket[j].collider = { rocket[j].position.x ,rocket[j].position.y ,40,40 };
                }
            }
        }
    }
}
void CheckCollisionWithTank(Bullet bullet[], Tank& tank, int& score)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullet[i].active)
        {
            Rectangle bulletRect =
            {
                bullet[i].position.x,
                bullet[i].position.y,
                (float)bullet[i].texture.width,
                (float)bullet[i].texture.height
            };

            Rectangle tankRect =
            {
                tank.position.x,
                tank.position.y,
                (float)tank.texture.width,
                (float)tank.texture.height
            };

            if (CheckCollisionRecs(bulletRect, tankRect))
            {
                bullet[i].active = false;
                tank.position = { SCREEN_WIDTH, 375 };
                tank.collider = { tank.position.x, tank.position.y,
                                (float)tank.texture.width,
                                (float)tank.texture.height };
                score += 2; 
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
                if (enemy[j].active && CheckCollisionRecs(bullet[i].collider, enemy[j].collider))
                {
                    bullet[i].active = false;
                    enemy[j].active = false;
                    score += 1;
                    enemy[j].position = { float(SCREEN_WIDTH + (rand() % 100)), 375 };
                    enemy[j].active = true;
                }
            }
        }
    }
}void CheckCollisionWithDrones(Bullet bullets[], Drone drones[], int& score)
{
    for (int i = 0; i < MAX_BULLETS; i++) 
    {
        if (bullets[i].active) {
            for (int j = 0; j < MAX_DRONES; j++) 
            {
                if (drones[j].active && CheckCollisionRecs(bullets[i].collider, drones[j].collider)) {
                    bullets[i].active = false;
                    drones[j].active = false;
                    score += 2; 
                    drones[j].position.x = SCREEN_WIDTH + (float)(rand() % 200);
                    drones[j].position.y = (float)(100 + rand() % 200);
                    drones[j].active = true;
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
void CheckPlayerDroneBulletCollision(Player& player, DroneBullet droneBullets[], int& health)
{
    Rectangle playerRect = { player.position.x, player.position.y, (float)player.currentTexture.width, (float)player.currentTexture.height };

    for (int i = 0; i < MAX_DRONE_BULLETS; i++)
    {
        if (droneBullets[i].active)
        {
            if (CheckCollisionRecs(playerRect, droneBullets[i].collider))
            {
                health--;
                droneBullets[i].active = false;
            }
        }
    }

    if (health <= 0)
    {
        health = 0;
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
void DrawDrones(Drone drones[]) 
{
    for (int i = 0; i < MAX_DRONES; i++) 
    {
        if (drones[i].active) 
        {
            DrawTexture(drones[i].texture, drones[i].position.x, drones[i].position.y, WHITE);
        }
    }
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
                { bullets[i].position.x, bullets[i].position.y, 10, 10 }, 
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
void DrawDroneBullets(DroneBullet droneBullets[])
{
    for (int i = 0; i < MAX_DRONE_BULLETS; i++) 
    {
        if (droneBullets[i].active) 
        {
            DrawRectangle(droneBullets[i].position.x, droneBullets[i].position.y, 10, 10, RED);
        }
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
                bullets[i].position.x = player.position.x + 85;
                bullets[i].texture = bulletTextureRight;
            }
            else
            {
                bullets[i].position.x = player.position.x - 15;
                bullets[i].texture = bulletTextureLeft; 
            }

            bullets[i].position.y = player.position.y + 50; 
            bullets[i].speed = 600.0f;
            bullets[i].direction = direction;
            bullets[i].active = true;
            bullets[i].collider = { bullets[i].position.x, bullets[i].position.y, 20, 20 }; 
            break;
        }
    }
}
void SwitchPlayerDirection(Player& player, float deltaTime, int& b1x, int& b2x)
{
    if (IsKeyDown(KEY_RIGHT) && player.position.x + player.currentTexture.width < SCREEN_WIDTH)
    {
        player.position.x += player.speed * deltaTime;
        player.currentTexture = player.textureRight;
        player.isFacingRight = true;
        b1x -= 4; 
        b2x -= 4;
    }
    if (IsKeyDown(KEY_LEFT) && player.position.x > 0)
    {
        player.position.x -= player.speed * deltaTime;
        player.currentTexture = player.textureLeft;
        player.isFacingRight = false;
    }
    if (b1x <= -SCREEN_WIDTH)
        b1x = SCREEN_WIDTH;
    if (b2x <= -SCREEN_WIDTH)
        b2x = SCREEN_WIDTH;
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
void DroneShooting(Drone drones[], DroneBullet droneBullets[], Player& player) 
{
    for (int i = 0; i < MAX_DRONES; i++) 
    {
        if (drones[i].active && drones[i].timeSinceLastShot >= drones[i].shootInterval) 
        {
            for (int j = 0; j < MAX_DRONE_BULLETS; j++)
            {
                if (!droneBullets[j].active)
                {
                    Vector2 direction;
                    direction.x = player.position.x - drones[i].position.x;
                    direction.y = player.position.y - drones[i].position.y;
                    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
                    direction.x /= length;
                    direction.y /= length;
                    droneBullets[j].position = drones[i].position;
                    droneBullets[j].direction = direction;
                    droneBullets[j].active = true;
                    droneBullets[j].collider = { droneBullets[j].position.x, droneBullets[j].position.y, 10, 10 };
                    drones[i].timeSinceLastShot = 0;
                    break;
                }
            }
        }
    }
}

// ----------------------------------------------------------------------- (MAIN)

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Metal Slug - Starter");
    SetTargetFPS(83);
    InitAudioDevice();

    Sound shoot = LoadSound("gun_shoot.wav");
    Sound GameOver = LoadSound("GameOver.wav");
    Sound backSound2 = LoadSound("BackSound2.wav");
    Sound backSound3 = LoadSound("BackSound3.wav");
    Sound backSound1 = LoadSound("BackSound1.wav");
    Sound levelComp = LoadSound("GameLevelCompleted.wav");

    Texture2D title = LoadTexture("title.png");
    Texture2D enemy = LoadTexture("enemy.png");
    Texture2D level1 = LoadTexture("LEVEL1.png");
    Texture2D level2 = LoadTexture("LEVEL2.png");
    Texture2D level3 = LoadTexture("LEVEL3.png");
    Texture2D b1 = LoadTexture("b1_metal_slug.png");
    Texture2D b2 = LoadTexture("b2_metal_slug.png");
    Texture2D b3 = LoadTexture("b3_metal_slug.png");
    Texture2D b4 = LoadTexture("b4_metal_slug.png");
    Texture2D b5 = LoadTexture("b5_metal_slug.png");
    Texture2D b6 = LoadTexture("b6_metal_slug.png");
    Texture2D droneTexture = LoadTexture("drone.png");
    Texture2D startWall = LoadTexture("StartWall.png");
    Texture2D helicopter1 = LoadTexture("helicopter.png");
    Texture2D rocketTexture = LoadTexture("crab_enemy.png");
    Texture2D bulletTextureLeft = LoadTexture("bulletLeft.png");
    Texture2D bulletTextureRight = LoadTexture("bulletRight.png");

    Bullet bullets[MAX_BULLETS];
    Rocket rocket[MAX_ROCKETS];
    Enemy Enemy[MAX_ENEMIES];
    Drone drones[MAX_DRONES];
    DroneBullet droneBullets[MAX_DRONE_BULLETS];

   
    Tank tank;
    Player player;
    Player player2;
    Barrel barrel;
    Helicopter helicopter[MAX_HELICOPTER];
    int b1x = 0;
    int b2x = SCREEN_WIDTH;
    int b3x = 0;
    int b4x = SCREEN_WIDTH;
    int b5x = 0;
    int b6x = SCREEN_WIDTH;
    int floor = 375;
    int score = 0;
    int health = 20;
    int jumpMax = 301;

    bool gameover = false;
    bool gamestarted = false;
    bool startLevel1 = false;
    bool startLevel2 = false;
    bool startLevel3 = false;

    float healthBarWidth = 300;
    float healthBarHeight = 30;
    float healthPercentage = (float)health / 20;

    b1.width = SCREEN_WIDTH;
    b2.width = SCREEN_WIDTH;
    b3.width = SCREEN_WIDTH;
    b4.width = SCREEN_WIDTH;
    b5.width = SCREEN_WIDTH;
    b6.width = SCREEN_WIDTH;
    b1.height = SCREEN_HEIGHT;
    b2.height = SCREEN_HEIGHT;
    b3.height = SCREEN_HEIGHT;
    b4.height = SCREEN_HEIGHT;
    b5.height = SCREEN_HEIGHT;
    b6.height = SCREEN_HEIGHT;
    title.width = SCREEN_WIDTH;
    title.height = SCREEN_HEIGHT;
    level2.width = SCREEN_WIDTH;
    level2.height = SCREEN_HEIGHT;
    startWall.width = SCREEN_WIDTH;
    startWall.height = SCREEN_HEIGHT;

    void (*Shoot)(Bullet[], Texture2D&, Texture2D&, Player&, Vector2) = ShootBullet;
    Shoot = ShootBullet;

    InitTank(tank);
    InitBarrel(barrel);
    InitPlayer(player, floor);
    InitDroneBullets(droneBullets);
    InitEnemy(Enemy, enemy, floor);
    InitDrone(drones, droneTexture);
    InitBullets(bullets, bulletTextureRight);
    InitRocket(rocket, rocketTexture, player);
    InitHelicopter(helicopter,helicopter1, player);
    b1x = 0;
    b2x = SCREEN_WIDTH;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        if (gameover)
        {
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100, 50, RED);
            DrawText(TextFormat("Final Score: %d", score), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 30, DARKGRAY);
            DrawText("Press ENTER to Restart or ESC to Quit", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 50, 20, DARKGRAY);
            StopSound(backSound1);
            StopSound(backSound2);
            StopSound(backSound3);
            PlaySound(GameOver);
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
                    startLevel1 = true;
                }
            }
            else if (startLevel1)
            {
                DrawTexture(startWall, SCREEN_WIDTH / 2 - startWall.width / 2,
                    SCREEN_HEIGHT / 2 - startWall.height / 2, WHITE);
                if (IsKeyPressed(KEY_ENTER))
                {
                    gamestarted = true;
                    gameover = false;
                    startLevel1 = false;

                }
            }
        }

        EndDrawing();
        b1x = 0;
        b2x = SCREEN_WIDTH;
        if (gamestarted && !gameover)
        {
            while (!WindowShouldClose())
            {
                float deltaTime = GetFrameTime();
                BeginDrawing();
                ClearBackground(RAYWHITE);

                MoveBullets(bullets);
                PlayerJump(player, floor, jumpMax);
                CheckCollision(bullets, rocket, score);
                DroneShooting(drones, droneBullets, player);
                CheckCollisionWithTank(bullets, tank, score);
                CheckCollisionWithEnemy(bullets, Enemy, score);
                CheckCollisionWithDrones(bullets, drones, score);
                CheckCollisionWithHelicopter(bullets, helicopter, score);
                ShootBasedOnKeyPress(bullets, bulletTextureRight, bulletTextureRight, player, shoot, Shoot);

                // .:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.
                //                          LEVEL # 01
                // .:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.

                if (score < 20)
                {
                    DrawTexture(level1, SCREEN_WIDTH / 2 - level1.width / 2, SCREEN_HEIGHT / 2 - level1.height / 2, WHITE);
                    DrawTexture(b1, b1x, 0, WHITE);
                    DrawTexture(b2, b2x, 0, WHITE);
                    if (!IsSoundPlaying(backSound1))
                        PlaySound(backSound1);
                    SwitchPlayerDirection(player, deltaTime, b1x, b2x);

                    if (b1x <= -SCREEN_WIDTH)
                        b1x = SCREEN_WIDTH;

                    if (b2x <= -SCREEN_WIDTH)
                        b2x = SCREEN_WIDTH;
                    CheckPlayerDroneBulletCollision(player, droneBullets, health);
                    DrawBullets(bullets);
                    DrawRocket(rocket);
                    DrawPlayer(player);
                    DrawPlayer(player2);
                    DrawEnemy(Enemy);
                    MoveEnemy(Enemy);
                    MoveRocket(rocket, player);

                }

                // .:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.
                //                          LEVEL # 02
                // .:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.

                else if (score >= 20 && !startLevel2)
                {
                    StopSound(backSound1);
                    DrawTexture(level2, SCREEN_WIDTH / 2 - level2.width / 2, SCREEN_HEIGHT / 2 - level2.height / 2, WHITE);

                    if (IsKeyPressed(KEY_ENTER))
                    {
                        startLevel2 = true;
                        PlaySound(levelComp);
                    }
                }
                else if (startLevel2)
                {
                    DrawTexture(b3, b3x, 0, WHITE);
                    DrawTexture(b4, b4x, 0, WHITE);
                    StopSound(levelComp);
                    if (!IsSoundPlaying(backSound2))
                        PlaySound(backSound2);
                    SwitchPlayerDirection(player, deltaTime, b3x, b4x);
                    CheckPlayerDroneBulletCollision(player, droneBullets, health);
                    MoveTank(tank);

                    DrawTank(tank);
                    MoveBarrel(barrel);
                    DrawBarrel(barrel);
                    DrawPlayer(player);
                    DrawPlayer(player2);
                    DrawBullets(bullets);
                    DrawHelicopter(helicopter);
                    MoveHelicopter(helicopter, player);

                    if (b3x <= -SCREEN_WIDTH)
                        b3x = SCREEN_WIDTH;

                    if (b4x <= -SCREEN_WIDTH)
                        b4x = SCREEN_WIDTH;

                }

                // .:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.
                //                          LEVEL # 03
                // .:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.

                if (score >= 40 && !startLevel3)
                {
                    StopSound(backSound2);
                    DrawTexture(level3, SCREEN_WIDTH / 2 - level3.width / 2, SCREEN_HEIGHT / 2 - level3.height / 2, WHITE);

                    if (IsKeyPressed(KEY_ENTER))
                    {
                        startLevel3 = true;
                        PlaySound(levelComp);
                        for (int i = 0; i < MAX_DRONES; i++)
                        {
                            drones[i].position.x = SCREEN_WIDTH + (float)(i * 200);
                            drones[i].position.y = (float)(100 + rand() % 200);
                            drones[i].active = true;
                        }
                    }
                }
                else if (startLevel3)
                {
                    DrawTexture(b5, b5x, 0, WHITE);
                    DrawTexture(b6, b6x, 0, WHITE);
                    StopSound(levelComp);
                    if (!IsSoundPlaying(backSound3))
                        PlaySound(backSound3);
                    SwitchPlayerDirection(player, deltaTime, b5x, b6x);

                    if (b5x <= -SCREEN_WIDTH)
                        b5x = SCREEN_WIDTH;

                    if (b6x <= -SCREEN_WIDTH)
                        b6x = SCREEN_WIDTH;
                    DrawDrones(drones);
                    DrawPlayer(player);
                    DrawBullets(bullets);
                    DrawDroneBullets(droneBullets);
                    MoveDrones(drones, deltaTime);
                    MoveDroneBullets(droneBullets, deltaTime);

                    CheckPlayerDroneBulletCollision(player, droneBullets, health);
                    if (health <= 0)
                    {
                        gameover = true;
                        break;
                    }
                }

                DrawText(TextFormat("SCORE : %d", score), 7, 7, 30, GREEN);
                DrawRectangle(SCREEN_WIDTH - healthBarWidth - 10, 10, healthBarWidth, healthBarHeight, DARKGRAY);                  // Background
                DrawRectangle(SCREEN_WIDTH - healthBarWidth - 10, 10, healthBarWidth * healthPercentage, healthBarHeight, GREEN);  // Health Bar
                DrawText(TextFormat("HEALTH: %d", health), SCREEN_WIDTH - 250, 7, 40, RED);

                if (score >= 100)
                {
                    DrawText("GAME WON", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100, 50, RED);
                    DrawText(TextFormat("Final Score: %d", score), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 30, DARKGRAY);
                    DrawText("Press ENTER to Restart or ESC to Quit", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 50, 20, DARKGRAY);
                    StopSound(backSound1);
                    StopSound(backSound2);
                    StopSound(backSound3);
                    PlaySound(levelComp);
                }

                EndDrawing();
            }
        }

        // game restarts here
        if (IsKeyPressed(KEY_ENTER))
        {
            score = 0;
            health = 20;
            gameover = false;
            gamestarted = true;
            startLevel1 = false;
            startLevel2 = false;
            startLevel3 = false;
            healthPercentage = (float)health / 20;
            InitTank(tank);
            InitBarrel(barrel);
            InitPlayer(player, floor);
            InitDroneBullets(droneBullets);
            InitEnemy(Enemy, enemy, floor);
            InitDrone(drones, droneTexture);
            InitBullets(bullets, bulletTextureRight);
            InitRocket(rocket, rocketTexture, player);
            InitHelicopter(helicopter, helicopter1, player);

            b1x = 0;
            b2x = SCREEN_WIDTH;
            b3x = 0;
            b4x = SCREEN_WIDTH;
            b5x = 0;
            b6x = SCREEN_WIDTH;

            StopSound(GameOver);
            StopSound(backSound1);
            StopSound(backSound2);
            StopSound(backSound3);
        }
    }
    // ....................UNLOAD TEXTURES
    UnloadTexture(player.textureRight);
    UnloadTexture(player.textureLeft);
    UnloadTexture(tank.texture);
    UnloadTexture(droneTexture);
    UnloadTexture(level1);
    UnloadTexture(level2);
    UnloadTexture(level3);
    UnloadTexture(title);
    UnloadTexture(b1);
    UnloadTexture(b2);
    UnloadTexture(b3);
    UnloadTexture(b4);
    UnloadTexture(b5);
    UnloadTexture(b6);
    // ....................UNLOAD SOUNDS
    UnloadSound  (shoot);
    UnloadSound(GameOver);
    UnloadSound(backSound1);
    UnloadSound(backSound2);
    UnloadSound(backSound3);
    UnloadSound(levelComp);
    CloseAudioDevice();
    CloseWindow();

    return 0;
  }

