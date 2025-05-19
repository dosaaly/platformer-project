#include "level.h"
#include "globals.h"
#include <raylib.h>
#include "level_legacy.h"
bool Level::texturesLoaded = false;

static constexpr char CH_WALL      = WALL;
static constexpr char CH_WALL_DARK = WALL_DARK;
static constexpr char CH_AIR       = AIR;
static constexpr char CH_SPIKE     = SPIKE;
static constexpr char CH_PLAYER    = PLAYER;
static constexpr char CH_ENEMY     = ENEMY;
static constexpr char CH_COIN      = COIN;
static constexpr char CH_EXIT      = EXIT;

static bool charSolid(char ch){ return ch==CH_WALL || ch==CH_WALL_DARK; }

static Texture2D& texFor(char ch){
    switch(ch){
        case CH_WALL:      return wall_image;
        case CH_WALL_DARK: return wall_dark_image;
        case CH_SPIKE:     return spike_image;
        case CH_EXIT:      return exit_image;
        case CH_COIN:      return coin_sprite.frames[0];
        default:           return wall_image;
    }
}

void Level::loadTextures(){
    if(texturesLoaded) return;
    texturesLoaded = true;   // textures already loaded globally in assets.h
}

bool Level::load(std::size_t, const level& src){
    loadTextures();
    grid.clear();
    _enemySpawns.clear();

    grid.resize(src.rows);
    for(size_t r=0;r<src.rows;++r){
        grid[r].resize(src.columns);
        for(size_t c=0;c<src.columns;++c){
            char ch = src.data[r*src.columns + c];
            grid[r][c] = ch;
            if(ch==CH_PLAYER){ _playerSpawn={float(c),float(r)}; grid[r][c]=CH_AIR; }
            else if(ch==CH_ENEMY){ _enemySpawns.push_back({float(c),float(r)}); grid[r][c]=CH_AIR; }
        }
    }
    return true;
}

bool Level::inside(int r,int c) const{
    return r>=0 && c>=0 && r<(int)grid.size() && c<(int)grid[0].size();
}
bool Level::isSolid(int r,int c) const{ return inside(r,c)&&charSolid(grid[r][c]); }

bool Level::colliding(Vector2 pos, char look) {
    Rectangle hit{pos.x, pos.y, 1, 1};
    for (int r = int(pos.y); r <= int(pos.y) + 1; ++r) {
        for (int c = int(pos.x); c <= int(pos.x) + 1; ++c) {
            if (inside(r, c) && grid[r][c] == look) {
                Rectangle cell{float(c), float(r), 1, 1};
                if (CheckCollisionRecs(hit, cell)) {
                    if (look == CH_COIN) {
                        grid[r][c] = CH_AIR; // Replace coin with air
                        PlaySound(coin_sound); // Play coin sound
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

char& Level::colliderRef(Vector2 pos,char look){
    Rectangle hit{pos.x,pos.y,1,1};
    for(int r=int(pos.y)-1;r<=int(pos.y)+1;++r)
        for(int c=int(pos.x)-1;c<=int(pos.x)+1;++c)
            if(inside(r,c)&&grid[r][c]==look){
                Rectangle cell{float(c),float(r),1,1};
                if(CheckCollisionRecs(hit,cell)) return grid[r][c];
            }
    return grid[int(pos.y)][int(pos.x)];
}

void Level::drawTiles() const {
    for (int r = 0; r < (int)grid.size(); ++r) {
        for (int c = 0; c < (int)grid[r].size(); ++c) {
            char ch = grid[r][c];
            if (ch == CH_AIR) continue; // Skip empty tiles
            DrawTexture(texFor(ch), c * TILE_SIZE, r * TILE_SIZE, WHITE);
        }
    }
}