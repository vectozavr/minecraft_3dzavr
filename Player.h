//
// Created by Иван Ильин on 14.03.2021.
//

#ifndef MINECRAFT_3DZAVR_PLAYER_H
#define MINECRAFT_3DZAVR_PLAYER_H

#include <SFML/Audio/Sound.hpp>
#include "tdzavrlib/Mesh.h"
#include "tdzavrlib/Camera.h"
#include "tdzavrlib/World.h"
#include "Map.h"

class Player {
private:
    double health = 100;
    double jumpHeight = 1.5;
    double walkSpeed = 7;

    double g = 35;

    Camera& camera;
    World& world;
    Screen& screen;
    Map& map;

    bool inRunning = false;

    Cube::Type selectedBlock = Cube::Type::stone;

    // sounds
    sf::Sound addBlock;
    sf::Sound removeBlock;
    sf::Sound walk;
    sf::Sound fall;

    Point4D oldVelocity;


public:
    Player(Camera& camera, World& world, Screen& screen, Map& map) : camera(camera), world(world), screen(screen), map(map) {
        world.loadObj("../obj/cube.obj", "player",{0.5, 1.9, 0.5});
        hitBox().translate({0, 6, 0});
        hitBox().setCollision(true);
        hitBox().setAcceleration({0, -g, 0});

        camera.translateToPoint(hitBox().position() + Point4D{0, 1.8, 0});
        hitBox().attach(reinterpret_cast<Mesh *>(&camera));

        world.loadObj("../obj/cube.obj", "player_hand",{0.3, 0.3, 0.8});
        world["player_hand"].setCollider(false);
        world["player_hand"].translateToPoint(hitBox().position() + Point4D{-1.5, 0, 0.7});

        hitBox().attach(&world["player_hand"]);
        hitBox().setVisible(false);
        world["player_hand"].rotate({-M_PI/10, 0, M_PI/6});
    }

    void update();

    Mesh& hitBox() { return world["player"]; }
};


#endif //MINECRAFT_3DZAVR_PLAYER_H
