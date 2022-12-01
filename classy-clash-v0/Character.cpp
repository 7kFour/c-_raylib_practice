#include "Character.h"

#include "raymath.h"

// Constructor
Character::Character() {
    width = texture.width / max_frames;
    height = texture.height;
}

// defining function from inside Character Class
// fully qualifying name by adding Character:: -- so <ClassName>::<function_name)
void Character::set_screen_pos(int win_width, int win_height) {
    screen_pos = {
        (float)win_width / 2.0f - 4.0f * (0.5f * width),
        (float)win_height / 2.0f - 4.0f * (0.5f * height)};
}

void Character::tick(float delta_time) {
    world_pos_last_frame = world_pos;

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
    Rectangle source{frame * width / 6.0f, 0.0f, right_left * width, height};
    Rectangle dest{screen_pos.x, screen_pos.y, 4.0f * width, 4.0f * height};
    DrawTexturePro(texture, source, dest, Vector2{}, 0.0f, WHITE);
}

void Character::undo_movement() {
    world_pos = world_pos_last_frame;
}