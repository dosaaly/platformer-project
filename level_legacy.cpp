#include "globals.h"
#include "level_adapter.h"
#include "player_manager.h"


void reset_level_index() { level_index = 0; }

void load_level(int offset /* =0 */)
{
    level_index += offset;
    if (level_index >= LEVEL_COUNT) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        level_index = 0;
        return;
    }

    std::size_t rows = LEVELS[level_index].rows;
    std::size_t cols = LEVELS[level_index].columns;

    delete[] current_level_data;
    current_level_data = new char[rows * cols];

    for (std::size_t r=0; r<rows; ++r)
        for (std::size_t c=0; c<cols; ++c)
            current_level_data[r*cols + c] =
                LEVELS[level_index].data[r*cols + c];

    current_level = { rows, cols, current_level_data };

    PlayerManager::get_instance().spawn_player();
    EnemiesController::get_instance().spawn_enemies();
    derive_graphics_metrics_from_loaded_level();

    gLevel.load(level_index, current_level);   // hand to OOP Level

    timer = MAX_LEVEL_TIME;
}

void unload_level()            { delete[] current_level_data; }
char& get_level_cell(std::size_t r,std::size_t c) { return current_level.data[r*current_level.columns+c]; }
void  set_level_cell(std::size_t r,std::size_t c,char ch){ get_level_cell(r,c)=ch; }
