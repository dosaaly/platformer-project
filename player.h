
#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "globals.h"

class Player {
public:
    Player()
        : pos{0, 0},
          y_velocity(0.0f),
          on_ground(false),
          looking_forward(true),
          moving(false),
          lives(MAX_PLAYER_LIVES),
          level_scores(LEVEL_COUNT, 0) {}

    // Accessors
    [[nodiscard]] Vector2 get_pos() const { return pos; }
    [[nodiscard]] float get_y_velocity() const { return y_velocity; }
    [[nodiscard]] bool is_on_ground() const { return on_ground; }
    [[nodiscard]] bool is_looking_forward() const { return looking_forward; }
    [[nodiscard]] bool is_moving() const { return moving; }
    [[nodiscard]] int get_lives() const { return lives; }
    [[nodiscard]] const std::vector<int>& get_level_scores() const { return level_scores; }

    // Mutators
    void set_pos(const Vector2& p) { pos = p; }
    void set_y_velocity(float v) { y_velocity = v; }
    void set_on_ground(bool value) { on_ground = value; }
    void set_looking_forward(bool value) { looking_forward = value; }
    void set_moving(bool value) { moving = value; }
    void set_lives(int value) { lives = value; }

    int& score_for_level(int index) { return level_scores[index]; }

private:
    Vector2 pos;
    float y_velocity;
    bool on_ground;
    bool looking_forward;
    bool moving;
    int lives;
    std::vector<int> level_scores;
};

#endif // PLAYER_H

