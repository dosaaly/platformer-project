#include "player_manager.h"
#include "globals.h"
#include "level_adapter.h"
#include "enemies_controller.h"
#include "raylib.h"

PlayerManager& PlayerManager::get_instance() {
    static PlayerManager instance;
    return instance;
}

void PlayerManager::reset_stats() {
    lives = MAX_PLAYER_LIVES;
    for (int &score : level_scores)
        score = 0;
}

void PlayerManager::spawn_player() {
    player.set_y_velocity(0.0f);

    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t col = 0; col < current_level.columns; ++col) {
            if (get_level_cell(row, col) == PLAYER) {
                player.set_pos({static_cast<float>(col), static_cast<float>(row)});
                set_level_cell(row, col, AIR);
                return;
            }
        }
    }
}

void PlayerManager::kill_player() {
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    lives--;
    level_scores[level_index] = 0;
}

void PlayerManager::move_horizontally(float speed) {
    float delta = 0;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        delta = speed;
        player.set_looking_forward(true);
        player.set_moving(true);
    }
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        delta = -speed;
        player.set_looking_forward(false);
        player.set_moving(true);
    }
    else {
        player.set_moving(false);
        return;
    }

    Vector2 pos = player.get_pos();
    float next_x = pos.x + delta;

    if (!is_colliding({next_x, pos.y}, WALL)) {
        pos.x = next_x;
        player.set_pos(pos);
    } else {
        pos.x = roundf(pos.x);
        player.set_pos(pos);
    }
}

void PlayerManager::jump() {
    if (player.is_on_ground())
        player.set_y_velocity(-JUMP_STRENGTH);
}

void PlayerManager::update_gravity() {
    Vector2 pos = player.get_pos();
    float vy = player.get_y_velocity();

    if (is_colliding({pos.x, pos.y - 0.1f}, WALL) && vy < 0) {
        vy = CEILING_BOUNCE_OFF;
    }

    pos.y += vy;
    vy += GRAVITY_FORCE;

    bool on_ground = is_colliding({pos.x, pos.y + 0.1f}, WALL);
    if (on_ground) {
        vy = 0;
        pos.y = roundf(pos.y);
    }

    player.set_pos(pos);
    player.set_y_velocity(vy);
    player.set_on_ground(on_ground);
}

void PlayerManager::update() {
    update_gravity();
    Vector2 pos = player.get_pos();

    // Collect all coins under the player (2×2 tile area)
    int start_row = static_cast<int>(floorf(pos.y));
    int end_row   = start_row + 1;
    int start_col = static_cast<int>(floorf(pos.x));
    int end_col   = start_col + 1;

    for (int row = start_row; row <= end_row; ++row) {
        for (int col = start_col; col <= end_col; ++col) {
            if (row < 0 || row >= current_level.rows
             || col < 0 || col >= current_level.columns)
                continue;

            if (get_level_cell(row, col) == COIN) {
                set_level_cell(row, col, AIR);
                PlaySound(coin_sound);
                level_scores[level_index]++;
            }
        }
    }

    // Exit logic
    if (is_colliding(pos, EXIT)) {
        if (timer > 0) {
            timer -= 25;
            time_to_coin_counter += 5;
            if (time_to_coin_counter / 60 > 1) {
                level_scores[level_index]++;
                time_to_coin_counter = 0;
            }
        } else {
            load_level(1);
            PlaySound(exit_sound);
        }
    } else if (timer >= 0) {
        timer--;
    }

    // Spike or fall
    if (is_colliding(pos, SPIKE) || pos.y > current_level.rows) {
        kill_player();
    }

    // Enemy collisions
    auto& enemies = EnemiesController::get_instance();
    if (enemies.is_colliding_with_enemies(pos)) {
        if (player.get_y_velocity() > 0) {
            enemies.remove_colliding_enemy(pos);
            PlaySound(kill_enemy_sound);
            level_scores[level_index]++;
            player.set_y_velocity(-BOUNCE_OFF_ENEMY);
        } else {
            kill_player();
        }
    }
}

int PlayerManager::get_total_score() const {
    int total = 0;
    for (int score : level_scores)
        total += score;
    return total;
}
