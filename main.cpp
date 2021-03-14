//
// Created by Иван Ильин on 06.02.2021.
//

#include "tdzavrlib/Tdzavr.h"
#include "tdzavrlib/CameraController.h"
#include <iostream>
#include "Player.h"
#include "Map.h"

using namespace std;

class TestGame : public Tdzavr {
private:
    CameraController cameraController;

    Player player;
    Map map;

public:
    TestGame();

    void start() override;
    void update(double elapsedTime) override;

    void switchCamera();
};

TestGame::TestGame() : cameraController(camera, screen), player(camera, world, screen), map(world)
{

}

void TestGame::start() {
    // This code executed once in the beginning:

    screen.setMode(Screen::ViewMode::Transparency);
    screen.setMouseCursorVisible(false);

    //world.loadObj("../obj/cube.obj", "ground", {10, 0.1, 10});

    map.addCube(Cube::stone, 1, 1);
    map.addCube(Cube::glass, 2, 1);
    map.addCube(Cube::water, 3, 1);
    map.addCube(Cube::sand, 4, 1);
    map.addCube(Cube::earth, 5, 1);
    map.addCube(Cube::grass, 6, 1);
    map.addCube(Cube::wood, 7, 1);

    for(int i = -10; i < 10; i++) {
        for(int j = -10; j < 10; j++) {
            map.addCube(Cube::stone, i, 0, j);
        }
    }

}

void TestGame::update(double elapsedTime) {
    // This code executed every time step:

    // Check all input after this condition please
    if (!screen.window.hasFocus())
        return;

    player.update();

    //cameraController.update();

    if(screen.isKeyTapped(sf::Keyboard::Escape))
        exit();
}

void TestGame::switchCamera() {
    if(cameraMode == CameraMode::LocalCamera)
        setCameraMode(CameraMode::ExternalObserver);
    else
        setCameraMode(CameraMode::LocalCamera);
}

int main() {
    TestGame game;
    //game.create(1920, 1080);
    //game.create(2048, 1152);
    game.create(3072, 1920);
    //game.create(3840, 2160);

    return 0;
}