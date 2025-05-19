

#ifndef ENEMIES_CONTROLLER_H
#define ENEMIES_CONTROLLER_H
#include <vector>
#include "raylib.h"
#include "enemy.h"


class EnemiesController {
public:
    static EnemiesController &get_instance() {
        static EnemiesController instance;
        return instance;
    }

    EnemiesController(const EnemiesController&) = delete;
    EnemiesController& operator=(const EnemiesController&) = delete;
    EnemiesController(EnemiesController&&) = delete;
    EnemiesController& operator=(EnemiesController&&) = delete;

    void spawn_enemies();
    void update_enemies();
    bool is_colliding_with_enemies(Vector2 pos) const;
    void remove_colliding_enemy(Vector2 pos);

    [[nodiscard]] const std::vector<Enemy> & get_enemies() const {
        return enemies;
    }

private:
    EnemiesController() = default;
    ~EnemiesController() = default;

    std::vector<Enemy> enemies{};
};



#endif //ENEMIES_CONTROLLER_H
