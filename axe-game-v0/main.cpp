#include "raylib.h"

int main()
{
    // window dimensions
    int width{800};
    int height{450};
    // initialize game window
    InitWindow(width, height, "Axe Game");

    // circle coordinates
    // circles position is calculated from the center - so + or - the radius to find the edges
    int circle_x{200};
    int circle_y{200};
    // circle size
    float circle_radius{25.0f};
    // circle edges
    int l_circle_x{circle_x - circle_radius};
    int r_circle_x{circle_x + circle_radius};
    int u_circle_y{circle_y - circle_radius};
    int b_circle_y{circle_y + circle_radius};

    // rectangle coordinates
    // rectangles position is calculated from upper left corner so rectangle_X
    int rectangle_x{400};
    int rectangle_y{0};
    // rectangle size
    int rect_height{50};
    int rect_width{50};
    // rectangle movement direction
    int direction{10};
    // rectangle edges
    int l_rect_x{rectangle_x};
    int r_rect_x{rectangle_x + rect_width};
    int u_rect_y{rectangle_y};
    int b_rect_y{rectangle_y + rect_height};

    // collision detection
    bool collision_with_axe =
        (b_rect_y >= u_circle_y) &&
        (u_rect_y <= b_circle_y) &&
        (l_rect_x < r_circle_x) &&
        (r_rect_x >= l_circle_x);

    // set application fps
    SetTargetFPS(60);

    // game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        if (collision_with_axe)
        {
            DrawText("Game Over Man!", 400, 200, 20, RED);
        }
        else
        {
            // game logic begins

            // update edges
            l_circle_x = circle_x - circle_radius;
            r_circle_x = circle_x + circle_radius;
            u_circle_y = circle_y - circle_radius;
            b_circle_y = circle_y + circle_radius;
            l_rect_x = rectangle_x;
            r_rect_x = rectangle_x + rect_width;
            u_rect_y = rectangle_y;
            b_rect_y = rectangle_y + rect_height;

            // update collision with axe
            // performing a collision check each frame
            collision_with_axe =
                (b_rect_y >= u_circle_y) &&
                (u_rect_y <= b_circle_y) &&
                (l_rect_x < r_circle_x) &&
                (r_rect_x >= l_circle_x);

            DrawCircle(circle_x, circle_y, circle_radius, BLUE);
            DrawRectangle(rectangle_x, rectangle_y, rect_height, rect_width, RED);

            // move the axe (rectangle)
            rectangle_y += direction;
            // reverse the movement of the axe so it moves up and down
            // direction is adding 10 when moving moving down and adding -10 when moving up
            if (rectangle_y > (height - rect_height) || rectangle_y < 0)
            {
                // - makes it the opposite of what it is -- a negative number would return as positive
                direction = -direction;
            }

            if (IsKeyDown(KEY_D) && circle_x < (width - 26.0))
            {
                circle_x += 5;
            }
            if (IsKeyDown(KEY_A) && circle_x > 26)
            {
                circle_x -= 5;
            }

            // game logic ends
        }

        EndDrawing();
    }
}