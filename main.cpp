//
// Created by Иван Ильин on 06.02.2021.
//

#include "tdzavrlib/Tdzavr.h"
#include "tdzavrlib/CameraController.h"
#include <iostream>
#include "Player.h"
#include "Map.h"

using namespace std;

class Minecraft : public Tdzavr {
private:
    CameraController cameraController;
    CameraController externalCameraController;

    Player player;
    Map map;

public:
    Minecraft();

    void start() override;
    void update(double elapsedTime) override;

    void switchCamera();
};

Minecraft::Minecraft() : cameraController(camera, screen), externalCameraController(external_camera, screen), map(world), player(camera, world, screen, map)
{

}

void Minecraft::start() {
    // This code executed once in the beginning:

    screen.setMouseCursorVisible(false);

    //world.loadObj("../obj/cube.obj", "ground", {10, 0.1, 10});

    world.loadObj("../obj/cube.obj", "point", {0.1, 0.1, 0.1});
    world["point"].setColor({255, 0, 0});

    map.addCube(Cube::stone, 1, 1);
    map.addCube(Cube::glass, 2, 1);
    map.addCube(Cube::water, 3, 1);
    map.addCube(Cube::sand, 4, 1);
    map.addCube(Cube::earth, 5, 1);
    map.addCube(Cube::grass, 6, 1);
    map.addCube(Cube::wood, 7, 1);
    map.addCube(Cube::snow, 8, 1);
//
    for(int i = -10; i < 10; i++) {
        for(int j = -10; j < 10; j++) {
            for(int k = 0; k < 1; k++)
                map.addCube(Cube::earth, i, k, j);
        }
    }

}

void Minecraft::update(double elapsedTime) {
    // This code executed every time step:

    // Check all input after this condition please
    if (!screen.window.hasFocus())
        return;

    if(cameraMode == CameraMode::LocalCamera)
        player.update();
    else
        externalCameraController.update();

    //cameraController.update();

    if(screen.isKeyTapped(sf::Keyboard::Escape))
        exit();

    if(screen.isKeyTapped(sf::Keyboard::E))
        switchCamera();

    //Line line(camera.position(), camera.position() + camera.lookAt()*10);

    auto rayCast = world.rayCast(camera.position(), camera.position() + camera.lookAt()*10);
    world["point"].translateToPoint(rayCast.first);
}

void Minecraft::switchCamera() {
    if(cameraMode == CameraMode::LocalCamera)
        setCameraMode(CameraMode::ExternalObserver);
    else
        setCameraMode(CameraMode::LocalCamera);
}

bool is_in_anim(const std::map<std::string, std::vector<double>>& map_of_vectors) {
    for(auto& animList : map_of_vectors)
        if(!animList.second.empty())
            return true;

    return false;
}

int main() {
    Minecraft game;
    //game.create(1920, 1080);
    //game.create(2048, 1152);
    game.create(3072, 1920);
    //game.create(3840, 2160);

    /*
    std::map<std::string, std::vector<double>> map_of_vectors;

    int animation_time = 0;

    while(animation_time < 1000) {
        if(!is_in_anim(map_of_vectors)) {
            map_of_vectors["hor_oscil"].emplace_back(2);
            map_of_vectors["hor_oscil"].emplace_back(1);
            map_of_vectors["hor_oscil"].emplace_back(2);

            map_of_vectors["vert_oscil"].emplace_back(2);
            map_of_vectors["vert_oscil"].emplace_back(1);
            map_of_vectors["vert_oscil"].emplace_back(2);
            map_of_vectors["vert_oscil"].emplace_back(1);
            map_of_vectors["vert_oscil"].emplace_back(2);
            map_of_vectors["vert_oscil"].emplace_back(1);
            map_of_vectors["vert_oscil"].emplace_back(2);

        }

        if(animation_time % 2 == 0) {
            auto animationList = map_of_vectors.begin();
            while (animationList != map_of_vectors.end()) {
                auto it = animationList->second.begin();
                while (!animationList->second.empty() && (it != animationList->second.end())) {
                    animationList->second.erase(it);
                }
                animationList++;
            }
        }

        animation_time++;
    }
    */

    return 0;
}