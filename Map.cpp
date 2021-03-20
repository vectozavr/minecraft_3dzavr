//
// Created by Иван Ильин on 15.03.2021.
//

#include "Map.h"

void Map::addCube(Cube::Type t, int posX, int posY, int posZ) {
    world.addMesh(Cube(t, posX, posY, posZ), "cube_X_" + std::to_string(posX) + "_Y_" + std::to_string(posY) + "_Z_" + std::to_string(posZ));
}

void Map::removeCube(int posX, int posY, int posZ) {
    world.removeMesh("cube_X_" + std::to_string(posX) + "_Y_" + std::to_string(posY) + "_Z_" + std::to_string(posZ));
}
