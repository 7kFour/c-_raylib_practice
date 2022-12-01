#include "Character.h"
#include "raylib.h"
#include "raymath.h"

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

        // checking map bounds
        if (knight.get_world_pos().x < 0.0f ||
            knight.get_world_pos().y < 0.0f ||
            knight.get_world_pos().x + window_width > map.width * map_scale ||
            knight.get_world_pos().y + window_height > map.height * map_scale) {
            // stop character from running off the map
            knight.undo_movement();
        }

        EndDrawing();
    }

    // shutting down gracefully
    UnloadTexture(map);
    // UnloadTexture(knight.idle);
    CloseWindow();
}