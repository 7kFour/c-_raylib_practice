#include "raylib.h"

class Character {
   public:
    // Constructor
    Character();

    // functions can be defined inside the class or outside of it
    // this is defined inside albeit with a single line of code
    Vector2 get_world_pos() { return world_pos; }

    // function prototype - notice it has a ; and no {}
    // will be defined outside of the class to save space
    void set_screen_pos(int win_width, int win_height);

    void tick(float delta_time);

    void undo_movement();

   private:
    Texture2D texture{LoadTexture("assets/characters/knight_idle_spritesheet.png")};
    Texture2D idle{LoadTexture("assets/characters/knight_idle_spritesheet.png")};
    Texture2D run{LoadTexture("assets/characters/knight_run_spritesheet.png")};
    Vector2 screen_pos{};
    Vector2 world_pos{};
    Vector2 world_pos_last_frame{};

    // 1 : facing right, -1 : facing left
    // this will be multiplied against the source rectangle width
    float right_left{1.0f};
    // animation variables
    float running_time{};
    int frame{};
    int max_frames{6};
    float update_time{1.0f / 12.0f};
    float speed{4.0};
    float width{};
    float height{};
};