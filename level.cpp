#include "level.h"
#include "globals.h"
#include "assets.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>

Level::Level() {}

bool Level::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) throw std::runtime_error("Cannot open " + path);

    std::string content;
    std::getline(file, content);
    std::stringstream ss(content);
    std::string rowStr;
    map.clear();
    enemySpawns.clear();

    int row = 0;
    while (std::getline(ss, rowStr, '|')) {
        parseRLELine(rowStr, row++);
    }
    return true;
}

void Level::parseRLELine(const std::string& line, int row) {
    int col = 0;
    for (size_t i = 0; i < line.size();) {
        int count = 0;
        while (i < line.size() && std::isdigit(line[i])) {
            count = count * 10 + (line[i++] - '0');
        }
        if (i >= line.size()) throw std::runtime_error("Bad RLE at row " + std::to_string(row));
        char ch = line[i++];
        TileType t = charToTile(ch);
        if (t == TileType::Unknown) throw std::runtime_error(std::string("Unknown char ") + ch);
        count = count ? count : 1;

        for (int k = 0; k < count; ++k, ++col) {
            if (t == TileType::PlayerStart)
                playerStart = Vector2{ static_cast<float>(col), static_cast<float>(row) };
            if (t == TileType::Enemy)
                enemySpawns.push_back(Vector2{ static_cast<float>(col), static_cast<float>(row) });
            if (map.size() <= row) map.emplace_back();
            map[row].push_back(t);
        }
    }
}

void Level::update() {
    // e.g. animate coins or spikes—empty for now
}

void Level::draw() {
    const int tileSize = TILE_SIZE;
    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            TileType t = map[y][x];
            Texture2D tex = Assets::GetTexture(t);
            DrawTexture(tex, x * tileSize, y * tileSize, WHITE);
        }
    }
}


Vector2 Level::getPlayerStart() const {
    return Vector2{ playerStart.x * TILE_SIZE, playerStart.y * TILE_SIZE };
}

const std::vector<Vector2>& Level::getEnemySpawns() const {
    return enemySpawns;
}

bool Level::isSolidTile(int tx, int ty) const {
    if (ty < 0 || ty >= map.size() || tx < 0 || tx >= map[ty].size()) return false;
    auto t = map[ty][tx];
    return t == TileType::Wall || t == TileType::DarkWall;
}

TileType Level::charToTile(char ch) const {
    switch (ch) {
        case '#': return TileType::Wall;
        case '=': return TileType::DarkWall;
        case '-': return TileType::Air;
        case '@': return TileType::PlayerStart;
        case '*': return TileType::Coin;
        case '^': return TileType::Spike;
        case '&': return TileType::Enemy;
        default:  return TileType::Unknown;
    }
}
