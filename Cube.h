//
// Created by Иван Ильин on 15.03.2021.
//

#ifndef MINECRAFT_3DZAVR_CUBE_H
#define MINECRAFT_3DZAVR_CUBE_H

#include "tdzavrlib/Mesh.h"
#include <SFML/Graphics.hpp>

class Cube : public Mesh {
public:
    enum Type {
        grass,
        sand,
        glass,
        water,
        stone,
        earth,
        wood,
        snow,

        none
    };
private:
    Type type;
public:
    static sf::Color cubeColor(Type t);

    explicit Cube(Type t, int posX = 0, int posY = 0, int posZ = 0) : type(t){
        c_color = Cube::cubeColor(t);
        loadObj("../obj/cube.obj");
        translate(Point4D{(double)posX*2, (double)posY*2, (double)posZ*2});
    }
};


#endif //MINECRAFT_3DZAVR_CUBE_H
