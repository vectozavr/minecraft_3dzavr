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
        wood
    };
private:
    Type type;
public:
    explicit Cube(Type t, int posX = 0, int posY = 0, int posZ = 0) : type(t){
        switch (t) {
            case grass:
                c_color = sf::Color(122, 255, 127);
                break;
            case sand:
                c_color = sf::Color(255, 235, 153);
                break;
            case glass:
                c_color = sf::Color(176, 243, 255, 100);
                break;
            case water:
                c_color = sf::Color(120, 147, 255, 200);
                break;
            case stone:
                c_color = sf::Color(148, 148, 148);
                break;
            case earth:
                c_color = sf::Color(153, 124, 86);
                break;
            case wood:
                c_color = sf::Color(117, 74, 39);
                break;
            default:
                c_color = sf::Color(255, 245, 194);
        }
        loadObj("../obj/cube.obj");
        translate(Point4D{(double)posX*2, (double)posY*2, (double)posZ*2});
    }
};


#endif //MINECRAFT_3DZAVR_CUBE_H
