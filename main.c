#include "raylib.h"
#include "stdio.h"
typedef struct Player
{
    Vector2 pos;
    Vector2 size;
} Player;

Player InitPlayer() {
    Player player = {0};
    player.pos = (Vector2){200, 200};
    player.size = (Vector2){20, 20};

    return player;
}

void DrawPlayer(Player *p) {
   DrawRectangleV(p->pos, p->size, RED); 
}

void UpdatePlayer(Player *p) {
    // Get mouse position on right click
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        Vector2 mouse_pos = GetMousePosition();

        printf("mouse_x: %f\nmouse_y: %f\n", mouse_pos.x, mouse_pos.y);
    }
    // if player x/y != mouse pos go to mouse
    // set abilities to qwer keys


    if(IsKeyDown(KEY_W)) {
        p->pos.y -= 5;
    }
    if(IsKeyDown(KEY_S)) {
        p->pos.y += 5;
    }
    if(IsKeyDown(KEY_D)) {
        p->pos.x += 5;
    }
    if(IsKeyDown(KEY_A)) {
        p->pos.x -= 5;
    }
}

int main(){
    InitWindow(800, 600, "Test Linking");
    SetTargetFPS(60);

    Player player = InitPlayer();

    while (!WindowShouldClose())
    {
        UpdatePlayer(&player);
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawPlayer(&player);

        EndDrawing();

    }

}