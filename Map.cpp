//
// Created by Иван Ильин on 15.03.2021.
//

#include "Map.h"
#include <iostream>
#include <fstream>

void Map::addCube(Cube::Type t, int posX, int posY, int posZ) {
    world.addMesh(Cube(t, posX, posY, posZ), "cube_X_" + std::to_string(posX) + "_Y_" + std::to_string(posY) + "_Z_" + std::to_string(posZ));

    cubes.emplace_back(t, Pos{posX, posY, posZ});
}

void Map::removeCube(int posX, int posY, int posZ) {
    world.removeMesh("cube_X_" + std::to_string(posX) + "_Y_" + std::to_string(posY) + "_Z_" + std::to_string(posZ));

    // delete cube. Yes, this is not very effective way to do this but it is 3am and I dont know what I am doing
    auto it = cubes.begin();
    while (it != cubes.end())
        if(it->second.x == posX && it->second.y == posY && it->second.z == posZ)
            cubes.erase(it);
        else
            it++;
}

void Map::saveMap(const std::string& mapName) {
    std::ofstream outfile (mapName, std::fstream::trunc);

    for(auto& cube : cubes)
        outfile << cube.first << "\t" << cube.second.x << "\t" << cube.second.y << "\t" << cube.second.z << std::endl;

    outfile.close();
}

void Map::loadMap(const std::string &mapName) {
    std::ifstream infile (mapName, std::fstream::out);

    while (!infile.eof()) {
        int t, posX, posY, posZ;
        infile >> t >> posX >> posY >> posZ;
        addCube(static_cast<Cube::Type>(t), posX, posY, posZ);
    }

    infile.close();
}
