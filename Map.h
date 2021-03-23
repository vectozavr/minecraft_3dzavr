//
// Created by Иван Ильин on 15.03.2021.
//

#ifndef MINECRAFT_3DZAVR_MAP_H
#define MINECRAFT_3DZAVR_MAP_H

#include "tdzavrlib/World.h"
#include "Cube.h"
#include <vector>

struct Pos {
    int x = 0;
    int y = 0;
    int z = 0;
};

class Map {
private:
    World& world;

    std::vector<std::pair<Cube::Type, Pos>> cubes;
public:
    explicit Map(World& world): world(world) {}

    void addCube(Cube::Type t, int posX = 0, int posY = 0, int posZ = 0);
    Cube::Type removeCube(int posX, int posY, int posZ);

    void loadMap(const std::string& mapName);
    void saveMap(const std::string& mapName);

    void init();
};


#endif //MINECRAFT_3DZAVR_MAP_H
