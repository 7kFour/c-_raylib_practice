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
    const int window_dimensions[2]{512, 380};
    const char *win_title = "Dapper Dasher";

    // initialize game window
    InitWindow(window_dimensions[0], window_dimensions[1], win_title);

    // AnimationData for nebula hazards
    // a better way of declaring structs and initializing struct members
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    // array for hazards
    const int size_of_nebulae{6};
    AnimationData nebulae[size_of_nebulae]{};

    // initializing the nebulae in the array
    for (int i = 0; i < size_of_nebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width / 8.0;
        nebulae[i].rec.height = nebula.height / 8.0;
        // not doing in loop pos.x because neb1/2 have different values
        nebulae[i].pos.y = window_dimensions[1] - nebula.height / 8.0;
        nebulae[i].frame = 0;
        nebulae[i].update_time = 0.0;
        nebulae[i].running_time = 0.0;
        nebulae[i].pos.x = window_dimensions[0] + (i * 300);
    }

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
    scarfy_data.pos.x = window_dimensions[0] / 2 - scarfy_data.rec.width / 2;
    scarfy_data.pos.y = window_dimensions[1] - scarfy_data.rec.height;
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

        // Start Game Logic

        // check rectangle position and apply gravity if it's not on the ground
        if (scarfy_data.pos.y >= window_dimensions[1] - scarfy_data.rec.height)
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

        // update nebulae positions
        for (int i = 0; i < size_of_nebulae; i++)
        {
            nebulae[i].pos.x += neb_vel * dT;
        }

        // update scarfy position
        scarfy_data.pos.y += velocity * dT;

        // running time to keep track of time since last update of animation frame
        // updating running time each frame
        scarfy_data.running_time += dT;
        for (int i = 0; i < size_of_nebulae; i++)
        {
            nebulae[i].running_time += dT;
        }

        // update scarfy animation frame - freeze while jumping
        if (scarfy_data.running_time >= scarfy_data.update_time && !is_in_air)
        {
            // reset run time because it has reached 1/12th of a second or greater
            scarfy_data.running_time = 0;

            // iterate through the spritesheet
            scarfy_data.rec.x = scarfy_data.frame * scarfy_data.rec.width;
            scarfy_data.frame++;

            // set frame back to beginning of spritesheet if it reaches the end
            if (scarfy_data.frame > 5)
            {
                scarfy_data.frame = 0;
            }
        }

        // update nebula animation frame
        for (int i = 0; i < size_of_nebulae; i++)
        {
            if (nebulae[i].running_time >= nebulae[i].update_time)
            {
                nebulae[i].running_time = 0;
                nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
                nebulae[i].frame++;

                if (nebulae[i].frame > 7)
                {
                    nebulae[i].frame = 0;
                }
            }
        }

        // draw nebulae hazards
        for (int i = 0; i < size_of_nebulae; i++)
        {
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
        }

        // drawing scarfy
        // using color white because we don't want to tint scarfy
        DrawTextureRec(scarfy, scarfy_data.rec, scarfy_data.pos, WHITE);

        // stop drawing

        // End Game Logic

        EndDrawing();
    }
    // must unload texture and close window to shut program down properly
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}