#include "raylib.h"
#include "raymath.h"
#include "stdio.h"

#include "utils.c"

bool reached = false;
bool throw_hook = false;
float range = 500;

typedef struct Hook
{
    Rectangle coll;
    Vector2 pos;
    float speed;
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
    Rectangle hook_target;
} Player;

Player InitPlayer() {
    Player player = {0};

    player.pos = (Vector2){200, 200};
    player.size = (Vector2){20, 20};
    player.target = (Vector2){0, 0};
    player.should_move = false;
    player.dir = 0.0;
    player.hook_loc = player.pos; 
    player.hook.pos = player.pos;
    player.hook.speed = 10;
    player.hook_target = (Rectangle){.height=5, .width=5, .x=player.pos.x, .y=player.pos.y}; 
    player.hook.coll = (Rectangle){.height=5, .width=5, .x=player.hook_target.x, player.hook_target.y};
    return player;
}

void DrawHook(Player *p) {
    DrawRectangleV(p->hook.pos, (Vector2){5, 5}, BLACK);
}

void DrawPlayer(Player *p) {
    DrawRectangleV(p->pos, p->size, RED); 
}

void UpdateHook(Player *p) {
    Vector2 start = p->pos; 
    Vector2 end = p->hook_loc;
    Vector2 direction = Vector2Normalize(Vector2Subtract(start, end));
    
    // Moves hook towards mouse click
    if(!Vector2Equals(p->hook.pos, p->hook_loc) && !reached){
        p->hook.pos = Vector2Add(p->hook.pos, Vector2Negate(direction));

        p->hook.coll.x = p->hook.pos.x;
        p->hook.coll.y = p->hook.pos.y;
    }
    
    if(CheckCollisionRecs(p->hook.coll, p->hook_target)) {
        reached = true;
    }

    // Debug draw the target loc of hook
    DrawRectangle(p->hook_target.x, p->hook_target.y, p->hook_target.width, p->hook_target.height, BLUE);
}

void UpdatePlayer(Player *p) {
    // Get mouse position on right click
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        Vector2 mouse_pos = GetMousePosition();
        p->target = mouse_pos;
        p->should_move = true;

        float distance = Vector2Distance(p->pos, p->target); 
        p->dir = Vector2Angle(p->pos, p->target);

    }
    
    if(IsKeyPressed(KEY_Q)){
        Vector2 mouse_pos = GetMousePosition();
        p->hook_loc = mouse_pos;
        p->hook.pos = p->pos;
        p->hook_target.x = p->hook_loc.x;
        p->hook_target.y = p->hook_loc.y;

        // spawn hook collider
        p->hook.coll.x = p->hook.pos.x;
        p->hook.coll.y = p->hook.pos.y;

        throw_hook = true;
    }

    if(throw_hook) {
        UpdateHook(p);
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