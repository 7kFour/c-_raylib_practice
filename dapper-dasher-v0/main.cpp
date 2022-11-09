#include "raylib.h"

struct AnimationData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float update_time;
    float running_time;
};

main()
{

    // main window dimensions
    const int win_height{380};
    const int win_width{512};
    const char *win_title = "Dapper Dasher";

    // initialize game window
    InitWindow(win_width, win_height, win_title);

    // AnimationData for nebula hazards
    // a better way of declaring structs and initializing struct members
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    AnimationData neb_data{
        {0.0, 0.0, nebula.width / 8.0, nebula.height / 8.0}, // rectangle rec
        {win_width, win_height - nebula.height / 8.0},       // Vector2 pos
        0,                                                   // int frame
        (1.0 / 12.0),                                        // float update_time
        0.0                                                  // float running_time
    };

    AnimationData neb2_data{
        {0.0, 0.0, nebula.width / 8.0, nebula.height / 8.0}, // rectangle rec
        {win_width + 300, win_height - nebula.height / 8.0}, // Vector2 pos
        0,                                                   // int frame
        (1.0 / 16.0),                                        // float update_time
        0.0                                                  // float running_time
    };

    // nebula x velocity (pixels per second)
    int neb_vel{-200};

    // showing one way of creating and initializing struct members
    // adding the scarfy spritesheet
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    // what rectangle we are getting off the sprite sheet
    // leaving this is in to illustrate that it's possible but prefer the above way of single line initialization
    AnimationData scarfy_data;
    scarfy_data.rec.width = scarfy.width / 6;
    scarfy_data.rec.height = scarfy.height;
    scarfy_data.rec.x = 0;
    scarfy_data.rec.y = 0;
    // where we want to draw scarfy
    // centering and placing on ground
    scarfy_data.pos.x = win_width / 2 - scarfy_data.rec.width / 2;
    scarfy_data.pos.y = win_height - scarfy_data.rec.height;
    // animation frame
    scarfy_data.frame = 0;
    // time between animation frames
    // updating animation 12 times per second - so this is 1/12th of a second
    scarfy_data.update_time = (1.0 / 12.0);
    // time since last update frame
    scarfy_data.running_time = 0.0;

    // acceleration due to gravity (pixels/sec)/sec
    const int gravity{1000};

    // ground check
    bool is_in_air{false};

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
            is_in_air = false;
        }
        else
        {
            // rectangle is in the air
            velocity += gravity * dT;
            is_in_air = true;
        }

        // allow user to jump with space bar
        if (IsKeyPressed(KEY_SPACE) && !is_in_air)
        {
            velocity += jump_vel;
        }
        // update nebula position
        neb_pos.x += neb_vel * dT;
        // update secondd nebula position
        neb2_pos.x += neb_vel * dT;
        // update scarfy position
        scarfy_pos.y += velocity * dT;

        // running time to keep track of time since last update of animation frame
        // updating running time each frame
        running_time += dT;
        neb_running_time += dT;

        // update scarfy animation frame - freeze while jumping
        if (running_time >= update_time && !is_in_air)
        {
            // reset run time because it has reached 1/12th of a second or greater
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

        // update nebula animation frame
        if (neb_running_time >= neb_update_time)
        {
            neb_running_time = 0;
            neb_rec.x = neb_frame * neb_rec.width;
            neb_frame++;

            if (neb_frame > 7)
            {
                neb_frame = 0;
            }
        }

        // update nebula animation frame
        if (neb2_running_time >= neb2_update_time)
        {
            neb2_running_time = 0;
            neb2_rec.x = neb2_frame * neb2_rec.width;
            neb2_frame++;

            if (neb2_frame > 7)
            {
                neb2_frame = 0;
            }
        }

        // draw nebula hazard
        DrawTextureRec(nebula, neb_rec, neb_pos, WHITE);
        // draw second nebula hazard
        DrawTextureRec(nebula, neb2_rec, neb2_pos, RED);

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
    UnloadTexture(nebula);
    CloseWindow();
}