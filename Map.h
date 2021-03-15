//
// Created by Иван Ильин on 15.03.2021.
//

#ifndef MINECRAFT_3DZAVR_MAP_H
#define MINECRAFT_3DZAVR_MAP_H

#include "tdzavrlib/World.h"
#include "Cube.h"
#include <vector>

class Map {
private:
    World& world;
    std::vector<std::string> cubes;


public:
    explicit Map(World& world): world(world) {}

    void addCube(Cube::Type t, int posX = 0, int posY = 0, int posZ = 0);
};


#endif //MINECRAFT_3DZAVR_MAP_H
