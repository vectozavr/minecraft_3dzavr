//
// Created by Иван Ильин on 14.03.2021.
//

#ifndef MINECRAFT_3DZAVR_PLAYER_H
#define MINECRAFT_3DZAVR_PLAYER_H

#include "tdzavrlib/Mesh.h"
#include "tdzavrlib/Camera.h"
#include "tdzavrlib/World.h"

class Player {
private:
    double health = 100;
    double jumpHeight = 1.5;
    double walkSpeed = 3;

    double g = 35;

    Camera& camera;
    World& world;
    Screen& screen;
public:
    Player(Camera& camera, World& world, Screen& screen) : camera(camera), world(world), screen(screen) {
        world.loadObj("../obj/cube.obj", "player",{0.5, 1.9, 0.5});
        hitBox().translate({0, 5, 0});
        hitBox().setCollision(true);
        hitBox().setAcceleration({0, -g, 0});
    }

    void update();

    Mesh& hitBox() { return world["player"]; }
};


#endif //MINECRAFT_3DZAVR_PLAYER_H
