#include "raylib.h"
#include "raymath.h"
#include "stdio.h"

#include "utils.c"

#define MAX_OBJECTS 5

bool reached = false;
bool throw_hook = false;
bool going_back = false;

Vector2 initial;
Rectangle old_pos;

float range = 500;

typedef struct Object
{
    Rectangle coll;
    Vector2 pos;
    bool hooked;
} Object;

Object CreateObject() {
    Object obj = {0};
    obj.pos=(Vector2){50, 50};
    obj.coll=(Rectangle){
        .height=20,
        .width=20,
        .x=obj.pos.x,
        .y=obj.pos.y
    };
    obj.hooked=false;

    return obj;
}



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
    Rectangle coll;
    float dir;
    Vector2 hook_loc;
    Hook hook;
    Rectangle hook_target;
} Player;

Player InitPlayer() {
    Player player = {0};

    player.pos = (Vector2){200, 200};
    player.size = (Vector2){20, 20};
    player.coll = (Rectangle){.height=20, .width=20, .x=player.pos.x, player.pos.y};
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
    DrawLine(initial.x, initial.y+10, p->hook.pos.x, p->hook.pos.y, RED);
}

void DrawPlayer(Player *p) {
    DrawRectangleV(p->pos, p->size, RED); 
}

void DrawObject(Object *obj) {
    DrawRectangleV(obj->pos, (Vector2){20, 20}, BLACK);
}

void UpdateObject(Object *obj, Player *p) {
    if(CheckCollisionRecs(obj->coll, p->hook.coll)) {
        obj->hooked = true;
    }

    // If hooked go to hook loc
    Vector2 start = obj->pos;
    Vector2 end = initial;
    Vector2 dir_to_player = Vector2Normalize(Vector2Subtract(end, start));

    if(obj->hooked) {
        Rectangle hook_rec = (Rectangle){
            .height=10,
            .width=10,
            .x=end.x,
            .y=end.y
        };
        obj->pos = Vector2Subtract(obj->pos, Vector2Negate(dir_to_player));
        if(CheckCollisionRecs(obj->coll, hook_rec)){
            obj->hooked = false;
        }
    }

    obj->coll.x = obj->pos.x;
    obj->coll.y = obj->pos.y; 
}

void UpdateHook(Player *p) {
    // Vector2 start = p->pos; 
    Vector2 start = initial;
    Vector2 end = p->hook_loc;
    Vector2 direction = Vector2Normalize(Vector2Subtract(start, end));
    Vector2 dir_to_player = Vector2Normalize(Vector2Subtract(end, start));
    
    // Moves hook towards mouse click
    if(!Vector2Equals(p->hook.pos, p->hook_loc) && !reached){
        p->hook.pos = Vector2Add(p->hook.pos, Vector2Negate(direction));
    }

    // Move hook back towards player 
    if(CheckCollisionRecs(p->hook.coll, p->hook_target)) {
        reached = true;
        going_back = true;
    }

    if(going_back){
        p->hook.pos = Vector2Add(p->hook.pos, Vector2Negate(dir_to_player));
        if(CheckCollisionRecs(p->hook.coll, old_pos)) {
            throw_hook = false;
            going_back = false;
            reached = false;
        }
    }
    
    p->hook.coll.x = p->hook.pos.x;
    p->hook.coll.y = p->hook.pos.y;
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
    
    if(IsKeyPressed(KEY_Q) && !throw_hook){
        Vector2 mouse_pos = GetMousePosition();
        p->hook_loc = mouse_pos;
        p->hook.pos = p->pos;
        p->hook_target.x = p->hook_loc.x;
        p->hook_target.y = p->hook_loc.y;

        initial = p->pos;

        Rectangle old_pos_coll = {
            .height=20,
            .width=20,
            .x=p->pos.x,
            .y=p->pos.y
        };

        old_pos = old_pos_coll;
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

    p->coll.x = p->pos.x;
    p->coll.y = p->pos.y;

    // set abilities to qwer keys
    
}

Vector2 GetPlayerPos(Player *player) {
    return player->pos;
}

int main(){
    InitWindow(800, 600, "Test Linking");
    SetTargetFPS(60);

    Player player = InitPlayer();
    Object obj = CreateObject();

    while (!WindowShouldClose())
    {
        UpdatePlayer(&player);
        UpdateObject(&obj, &player);
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawPlayer(&player);
        DrawObject(&obj);

        EndDrawing();

    }

}