#include "raylib.h"

int main()
{

    // main window dimensions
    const int win_height{380};
    const int win_width{512};
    const char *win_title = "Dapper Dasher";

    // initialize game window
    InitWindow(win_width, win_height, win_title);

    // adding the scarfy spritesheet
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    // what rectangle we are getting off the sprite sheet
    Rectangle scarfy_rec;
    scarfy_rec.width = scarfy.width / 6;
    scarfy_rec.height = scarfy.height;
    scarfy_rec.x = 0;
    scarfy_rec.y = 0;
    // where we want to draw scarfy
    Vector2 scarfy_pos;
    // centering and placing on ground
    scarfy_pos.x = win_width / 2 - scarfy_rec.width / 2;
    scarfy_pos.y = win_height - scarfy_rec.height;

    // animation frame
    int frame{};

    // time between animation frames
    // updating animation 12 times per second - so this is 1/12th of a second
    const float update_time{1.0 / 12.0};

    // time since last update frame
    float running_time{};

    // acceleration due to gravity (pixels/sec)/sec
    const int gravity{1000};

    // ground check
    bool is_in_airAir{false};

    // jump velocity in pixels per second
    // negative value because negative is up
    const int jump_vel{-600};

    // velocity
    int velocity{0};

    // set FPS
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // delta time (time since last frame)
        const float dT{GetFrameTime()};

        // check rectangle position and apply gravity if it's not on the ground
        if (scarfy_pos.y >= win_height - scarfy_rec.height)
        {
            // rectangle on ground
            velocity = 0;
            is_in_airAir = false;
        }
        else
        {
            // rectangle is in the air
            velocity += gravity * dT;
            is_in_airAir = true;
        }

        // allow user to jump with space bar
        if (IsKeyPressed(KEY_SPACE) && !is_in_airAir)
        {
            velocity += jump_vel;
        }

        // update position
        scarfy_pos.y += velocity * dT;

        // running time to keep track of time since last update of animation frame
        // updating running time
        running_time += dT;

        // update animation frame
        if (running_time >= update_time)
        {
            // reset run time for next iteration of check
            running_time = 0;

            // iterate through the spritesheet
            scarfy_rec.x = frame * scarfy_rec.width;
            frame++;

            // set frame back to beginning of spritesheet if it reaches the end
            if (frame > 5)
            {
                frame = 0;
            }
        }

        // drawing scarfy
        // using color white because we don't want to tint scarfy
        DrawTextureRec(scarfy, scarfy_rec, scarfy_pos, WHITE);

        // stop drawing

        // Start Game Logic

        // End Game Logic

        EndDrawing();
    }
    // must unload texture and close window to shut program down properly
    UnloadTexture(scarfy);
    CloseWindow();
}