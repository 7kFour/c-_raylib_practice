#include "raylib.h"

struct AnimationData {
    Rectangle rec;
    Vector2 pos;
    int frame;
    float update_time;
    float running_time;
};

bool isOnGround(AnimationData data, int windowHeight) {
    // perform ground check - return bool for ground check loop
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimationData updateAnimData(AnimationData data, float deltaTime, int maxFrame) {
    // update running time withe delta time
    data.running_time += deltaTime;

    if (data.running_time >= data.update_time) {
        data.running_time = 0.0;

        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame) {
            data.frame = 0;
        }
    }

    return data;
}

int main() {
    // main window dimensions
    const int window_dimensions[2]{512, 380};
    const char *win_title = "Dapper Dasher";

    // initialize game window
    InitWindow(window_dimensions[0], window_dimensions[1], win_title);

    // AnimationData for nebula hazards
    // a better way of declaring structs and initializing struct members
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    // array for hazards
    const int size_of_nebulae{10};
    AnimationData nebulae[size_of_nebulae]{};

    // initializing the nebulae in the array
    for (int i = 0; i < size_of_nebulae; i++) {
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

    // win condition - x position of invisible finish line
    float finish_line{nebulae[size_of_nebulae - 1].pos.x};

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
    scarfy_data.update_time = (1.0 / 6.0);

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

    // background texture
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bg_x{};
    float bg_scale{2.0};
    float bg_rotation{};

    // midground texture
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mg_x{};
    float mg_scale{2.0};
    float mg_rotation{};

    // foreground texture
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fg_x{};
    float fg_scale{2.0};
    float fg_rotation{};

    // collision between scarfy and nebula
    bool collision{};

    // set FPS
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // delta time (time since last frame)
        const float dT{GetFrameTime()};

        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // scroll background
        bg_x -= 20 * dT;
        if (bg_x <= -background.width * 2) {
            bg_x = 0;
        }

        // scroll midground
        mg_x -= 40 * dT;
        if (mg_x <= -midground.width * 2) {
            mg_x = 0;
        }

        // scroll foreground
        fg_x -= 80 * dT;
        if (fg_x <= -foreground.width * 2) {
            fg_x = 0;
        }

        // draw background
        // *2 because we are scaling by 2 when drawing
        Vector2 bg1Pos{bg_x, 0.0};
        DrawTextureEx(background, bg1Pos, bg_rotation, bg_scale, WHITE);
        Vector2 bg2Pos{bg_x + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, bg_rotation, bg_scale, WHITE);

        // // draw midground
        Vector2 mg1Pos{mg_x, 0.0};
        DrawTextureEx(midground, mg1Pos, mg_rotation, mg_scale, WHITE);
        Vector2 mg2Pos{mg_x + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg2Pos, mg_rotation, mg_scale, WHITE);

        // draw foreground
        Vector2 fg1Pos{fg_x, 0.0};
        DrawTextureEx(foreground, fg1Pos, fg_rotation, fg_scale, WHITE);
        Vector2 fg2Pos{fg_x + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg2Pos, fg_rotation, fg_scale, WHITE);

        // Start Game Logic

        // check rectangle position and apply gravity if it's not on the ground (ground check)
        if (isOnGround(scarfy_data, window_dimensions[1])) {
            // rectangle on ground
            velocity = 0;
            is_in_air = false;
        } else {
            // rectangle is in the air
            velocity += gravity * dT;
            is_in_air = true;
        }

        // allow user to jump with space bar
        if (IsKeyPressed(KEY_SPACE) && !is_in_air) {
            velocity += jump_vel;
        }

        // update nebulae positions
        for (int i = 0; i < size_of_nebulae; i++) {
            nebulae[i].pos.x += neb_vel * dT;
        }

        // update scarfy position
        scarfy_data.pos.y += velocity * dT;

        // running time to keep track of time since last update of animation frame
        // updating running time each frame
        scarfy_data.running_time += dT;
        for (int i = 0; i < size_of_nebulae; i++) {
            nebulae[i].running_time += dT;
        }

        // update scarfy animation frame - freeze while jumping
        if (!is_in_air) {
            scarfy_data = updateAnimData(scarfy_data, dT, 5);
        }

        // update nebula animation frame
        for (int i = 0; i < size_of_nebulae; i++) {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

        // scroll finish line at same speed as nebulae
        // placed at position of final nebula in array
        finish_line += neb_vel * dT;

        for (AnimationData nebula : nebulae) {
            // making the collision rectangle more accurate by making it tighter to the hazard
            float pad{50};
            Rectangle neb_rec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2 * pad,
                nebula.rec.height - 2 * pad};

            // don't need a pad for scarfy because the sprite fills up its location on the spritesheet better
            Rectangle scarfy_rec{
                scarfy_data.pos.x,
                scarfy_data.pos.y,
                scarfy_data.rec.width,
                scarfy_data.rec.height};

            if (CheckCollisionRecs(neb_rec, scarfy_rec)) {
                collision = true;
            }
        }

        // win/lose conditions
        if (collision) {
            // lose the game
            DrawText("u ded reeee", 120, window_dimensions[1] / 2, 48, RED);

        } else if (scarfy_data.pos.x >= finish_line) {
            // win the game
            DrawText("u one yyayyaaa", 75, window_dimensions[1] / 2, 48, BLUE);

        } else {
            // draw nebulae hazards
            for (int i = 0; i < size_of_nebulae; i++) {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }

            // drawing scarfy
            // using color white because we don't want to tint scarfy
            DrawTextureRec(scarfy, scarfy_data.rec, scarfy_data.pos, WHITE);
        }

        // End Game Logic
        EndDrawing();
    }

    // must unload texture and close window to shut program down properly
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}
