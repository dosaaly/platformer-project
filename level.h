#pragma once
#include <string>
#include <vector>
#include <raylib.h>
#include "assets.h"

enum class TileType {
    Wall, DarkWall, Air,
    PlayerStart, Coin,
    Spike, Enemy, Unknown
};

class Level {
public:
    Level();
    bool loadFromFile(const std::string& path);
    void update();
    void draw();
    Vector2 getPlayerStart() const;
    const std::vector<Vector2>& getEnemySpawns() const;
    bool isSolidTile(int tx, int ty) const;

private:
    std::vector<std::vector<TileType>> map;
    Vector2 playerStart{0,0};
    std::vector<Vector2> enemySpawns;
    TileType charToTile(char ch) const;
    void parseRLELine(const std::string& line, int row);
};
