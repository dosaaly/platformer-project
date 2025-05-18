#ifndef LEVEL_LEGACY_H
#define LEVEL_LEGACY_H

#include "globals.h"

// declarations only – no function bodies here
void reset_level_index();
void load_level(int offset);
void unload_level();

char& get_level_cell(std::size_t row, std::size_t col);
void  set_level_cell(std::size_t row, std::size_t col, char ch);

#endif  // LEVEL_LEGACY_H
