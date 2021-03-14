//
// Created by Иван Ильин on 15.03.2021.
//

#include "Map.h"

void Map::addCube(Cube::Type t, int posX, int posY, int posZ) {
    cubes.push_back("cube_" + std::to_string(t) + "_" + std::to_string(cubes.size()));
    world.addMesh(Cube(t, posX, posY, posZ), cubes.back());
}
