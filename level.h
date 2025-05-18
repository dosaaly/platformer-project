#pragma once
#include <vector>
#include <cstddef>
#include <raylib.h>
#include "globals.h"
#include "assets.h"

class Level {
public:
    bool load(std::size_t index, const level& src);

    bool  inside(int row, int col)              const;
    bool  isSolid(int row, int col)             const;
    bool  colliding(Vector2 pos, char lookFor)  const;
    char& colliderRef(Vector2 pos, char lookFor);

    Vector2 playerSpawn()   const { return _playerSpawn; }
    const std::vector<Vector2>& enemySpawns() const { return _enemySpawns; }

    void drawTiles() const;

private:
    std::vector<std::vector<char>> grid;
    Vector2                        _playerSpawn{};
    std::vector<Vector2>           _enemySpawns;

    static bool texturesLoaded;
    static void loadTextures();
};
