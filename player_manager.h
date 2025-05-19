#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "player.h"

class PlayerManager {
public:
    static PlayerManager& get_instance();
    Player& get_mutable_player() {
        return player;
    }
    void reset_stats();
    void spawn_player();
    void kill_player();
    void move_horizontally(float delta);
    void jump();
    void update_gravity();
    void update();

    [[nodiscard]] int get_total_score() const;
    [[nodiscard]] const Player& get_player() const { return player; }

private:
    PlayerManager() = default;

    int lives = 0;
    int level_scores[LEVEL_COUNT]{};

    Player player;

    int time_to_coin_counter = 0;

    friend class PlayerManagerAccessor;
};

#endif // PLAYER_MANAGER_H
