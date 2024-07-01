#include "raylib.h"
#include "raymath.h"
#include "stdio.h"

bool reached = false;
bool throw_hook = false;
float range = 500;

typedef struct Hook
{
    Vector2 pos;
    Vector2 target;
} Hook;

typedef struct Player
{
    Vector2 pos;
    Vector2 size;
    Vector2 target;
    bool should_move;
    float dir;
    Vector2 hook_loc;
    Hook hook;
} Player;

Player InitPlayer() {
    Player player = {0};
    player.pos = (Vector2){200, 200};
    player.size = (Vector2){20, 20};
    player.target = (Vector2){0, 0};
    player.should_move = false;
    player.dir = 0.0;
    player.hook_loc = (Vector2){0, 0};
    player.hook.pos = player.pos;
    player.hook.target = player.hook_loc;
    return player;
}

void DrawHook(Player *p) {
    Vector2 start = p->pos;
    Vector2 end = p->hook_loc;

    float distance = Vector2Distance(start, end); 

    Vector2 target_pos = Vector2Lerp(start, end, GetFrameTime());

    if(throw_hook) {
        if(p->hook.pos.x < end.x && !reached){
            p->hook.pos.x += 2;
        }
        if(p->hook.pos.x >= end.x){
            reached = true;
        }
        
        if(reached) {
            p->hook.pos.x -= 2;
        }

        if(p->hook.pos.x == p->pos.x){
            p->hook.pos = p->pos;
            reached = false;
            throw_hook = false;
        }

        DrawRectangleV(p->hook.pos, (Vector2){5, 5}, BLACK);
    }
}

void DrawPlayer(Player *p) {
    DrawRectangleV(p->pos, p->size, RED); 
}

void UpdatePlayer(Player *p) {
    // Get mouse position on right click
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        Vector2 mouse_pos = GetMousePosition();
        p->target = mouse_pos;
        p->should_move = true;

        float distance = Vector2Distance(p->pos, p->target); 
        p->dir = Vector2Angle(p->pos, p->target);
        printf("mouse_x: %f\n", distance);

    }

    if(IsKeyPressed(KEY_Q)){
        Vector2 mouse_pos = GetMousePosition();
        p->hook_loc = mouse_pos;
        throw_hook = true;
    }
    

    if(throw_hook) {
        DrawHook(p);
    }
    // Very basic move to click
    if(p->should_move && p->pos.x < p->target.x) {
        p->pos.x += 5;
    }
    if(p->should_move && p->pos.x > p->target.x) {
        p->pos.x -= 5;
    }
    if(p->should_move && p->pos.y < p->target.y) {
        p->pos.y += 5;
    }
    if(p->should_move && p->pos.y > p->target.y) {
        p->pos.y -= 5;
    }

    // set abilities to qwer keys
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