//
// Created by Иван Ильин on 14.03.2021.
//

#ifndef MINECRAFT_3DZAVR_PLAYER_H
#define MINECRAFT_3DZAVR_PLAYER_H

#include <SFML/Audio/Sound.hpp>
#include <utility>
#include "tdzavrlib/Mesh.h"
#include "tdzavrlib/Camera.h"
#include "tdzavrlib/World.h"
#include "Map.h"

class Player : public Mesh{
private:
    double _health = 100;
    double jumpHeight = 1.5;
    double walkSpeed = 7;
    double _headAngle = 0;

    double g = 35;

    std::shared_ptr<Camera> _camera;
    std::shared_ptr<Screen> _screen;

    std::shared_ptr<World> _world;
    std::shared_ptr<Map> _map;

    bool inRunning = false;

    Cube::Type selectedBlock = Cube::Type::stone;

    // sounds
    sf::Sound addBlock;
    sf::Sound removeBlock;
    sf::Sound walk;
    sf::Sound fall;

    Point4D oldVelocity;

    std::string _name = "Ivan";

    std::function<void(Pos pos, Cube::Type type)> addCubeCallBack;
    std::function<void(Pos pos)> removeCubeCallBack;

public:
    Player() {
        loadObj("../obj/cube.obj", {0.5, 1.9, 0.5});
        setAcceleration({0, -g, 0});
        setCollision(true);
        setVisible(false);
        setColor({240, 168, 168});
    };

    void update();

    void attachCamera(std::shared_ptr<Camera>& camera, std::shared_ptr<Screen> screen) {
        _camera = camera;
        _screen = std::move(screen);

        camera->translateToPoint(position() + Point4D{0, 1.8, 0});
        this->attach(camera);
    }

    void attachWorld(const std::shared_ptr<World>& world, const std::shared_ptr<Map>& map, const Point4D& pos = {0, 6, 0}) {
        _world = world;
        _map = map;

        translate(pos);

        _world->loadObj("../obj/cube.obj", name() + "_hand",{0.3, 0.3, 0.8});
        (*_world)[name() + "_hand"]->setCollider(false);
        (*_world)[name() + "_hand"]->translateToPoint(position() + Point4D{-1.5, 0, 0.7});


        attach((*_world)[name() + "_hand"]);
        (*_world)[name() + "_hand"]->rotate({-M_PI/10, 0, M_PI/6});

        (*_world)[name() + "_hand"]->setVisible(false);

        _world->loadObj("../obj/cube.obj", name() + "cube_in_hand");
        (*_world)[name() + "cube_in_hand"]->setCollider(false);
        (*_world)[name() + "cube_in_hand"]->translateToPoint((*_world)[name() + "_hand"]->position() + Point4D{-1.5, -1, 0.7});
        (*_world)[name() + "cube_in_hand"]->rotate({0, M_PI/10, 0});
        (*_world)[name() + "_hand"]->attach((*_world)[name() + "cube_in_hand"]);

        (*_world)[name() + "cube_in_hand"]->setColor(Cube::cubeColor(selectedBlock));
    }

    void setHealth(double h) {
        _health = h;
    }

    void add_removeCubeCallBack(std::function<void(Pos pos, Cube::Type type)> add, std::function<void(Pos pos)> remove) {
        addCubeCallBack = std::move(add);
        removeCubeCallBack = std::move(remove);
    }

    [[nodiscard]] double health() const { return _health; }
    [[nodiscard]] std::string name() const { return "Player_" + _name; }


    std::shared_ptr<Camera> camera() { return _camera; }

    // This is for situation when you want to store the position of the head but you dont have attached camera
    void setHeadAngle(double a) { _headAngle = a; }
    [[nodiscard]] double headAngle() const { return _headAngle; };
};


#endif //MINECRAFT_3DZAVR_PLAYER_H
