#include "raylib.h"
#include "raymath.h"

class Character {
   public:
    // functions can be defined inside the class or outside of it
    // this is defined inside albeit with a single line of code
    Vector2 get_world_pos() { return world_pos; }

    // function prototype - notice it has a ; and no {}
    // will be defined outside of the class to save space
    void set_screen_pos(int win_width, int win_height);

    void tick(float delta_time);

   private:
    Texture2D texture{LoadTexture("assets/characters/knight_idle_spritesheet.png")};
    Texture2D idle{LoadTexture("assets/characters/knight_idle_spritesheet.png")};
    Texture2D run{LoadTexture("assets/characters/knight_run_spritesheet.png")};
    Vector2 screen_pos{};
    Vector2 world_pos{};
    // 1 : facing right, -1 : facing left
    // this will be multiplied against the source rectangle width
    float right_left{1.0f};
    // animation variables
    float running_time{};
    int frame{};
    const int max_frames{6};
    const float update_time{1.0f / 12.0f};
    const float speed{4.0};
};

// defining function from inside Character Class
// fully qualifying name by adding Character:: -- so <ClassName>::<function_name)
void Character::set_screen_pos(int win_width, int win_height) {
    screen_pos = {
        (float)win_width / 2.0f - 4.0f * (0.5f * (float)texture.width / 6.0f),
        (float)win_height / 2.0f - 4.0f * (0.5f * (float)texture.height)};
}

void Character::tick(float delta_time) {
    // movement
    Vector2 direction{};
    if (IsKeyDown(KEY_A)) direction.x -= 1.0;
    if (IsKeyDown(KEY_D)) direction.x += 1.0;
    if (IsKeyDown(KEY_W)) direction.y -= 1.0;
    if (IsKeyDown(KEY_S)) direction.y += 1.0;
    if (Vector2Length(direction) != 0.0) {
        // normalize changes a vectors length to 1

        // set world_pos = world_pos + direction
        world_pos = Vector2Add(world_pos, Vector2Scale(Vector2Normalize(direction), speed));

        // set right left
        // if direction.x is less than 0 it means we are moving to the left
        direction.x < 0.0f ? right_left = -1.0f : right_left = 1.0f;
        texture = run;
    } else {
        texture = idle;
    }

    // update animation frame - GetFrameTime() is delta time
    running_time += delta_time;
    if (running_time >= update_time) {
        frame++;
        running_time = 0.0f;

        if (frame > max_frames) frame = 0;
    }

    // draw character
    Rectangle source{frame * (float)texture.width / 6.0f, 0.0f, right_left * (float)texture.width / 6.0f, (float)texture.height};
    Rectangle dest{screen_pos.x, screen_pos.y, 4.0f * (float)texture.width / 6.0f, 4.0f * (float)texture.height};
    DrawTexturePro(texture, source, dest, Vector2{}, 0.0f, WHITE);
}

int main() {
    // main window dimensions
    const int window_height{384};
    const int window_width{384};
    const char* win_title = "Sword of Knight";

    // initialize game window
    InitWindow(window_width, window_height, win_title);

    // background map
    Texture2D map = LoadTexture("assets/maps/my_map.png");
    float map_x{0.0f};
    float map_scale{4.0f};

    // map position
    Vector2 map_position{map_x, 0.0f};

    // create player object - set screen pos
    Character knight{};
    knight.set_screen_pos(window_width, window_height);

    // set FPS
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // basically multiplying the characters world positon by -1 to flip it so tha the map
        // moves the opposite way to simulate movement and have character remain in the center of the screen
        map_position = Vector2Scale(knight.get_world_pos(), -1.0f);

        // draw map
        DrawTextureEx(map, map_position, 0.0, map_scale, WHITE);

        // calling tick() to implement animation and movement for character
        // call after drawing map or map will be drawn on top of player
        knight.tick(GetFrameTime());

        EndDrawing();
    }

    // shutting down gracefully
    UnloadTexture(map);
    // UnloadTexture(knight.idle);
    CloseWindow();
}