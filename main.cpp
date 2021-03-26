//
// Created by Иван Ильин on 06.02.2021.
//

#include "tdzavrlib/Tdzavr.h"
#include "tdzavrlib/CameraController.h"
#include <iostream>
#include "Player.h"
#include "Map.h"
#include "tdzavrlib/ResourceManager.h"
#include "gui/Window.h"

using namespace std;

class Minecraft : public Tdzavr {
private:
    CameraController externalCameraController;

    Player player;
    Map map;

    sf::Sound backgroundNoise;
    sf::Sound music;

    Window mainMenu;
    Window settings;
    Window about;


    bool inGame = false;

public:
    Minecraft();

    void start() override;
    void update(double elapsedTime) override;

    void gui() override;

    void switchCamera();

    void play();
};

Minecraft::Minecraft() : externalCameraController(external_camera, screen), map(world), player(camera, world, screen, map)
{

}

void test() {

}

void Minecraft::start() {
    // This code executed once in the beginning:

    screen.setMouseCursorVisible(true);

    //world.loadObj("../obj/cube.obj", "point", {0.1, 0.1, 0.1});
    //world["point"].setColor({255, 0, 0});

    map.loadMap("../maps/map_test");

    setUpdateWorld(false);

    // windows init:
    mainMenu.title("Main menu");
    mainMenu.setBackgroundTexture("../textures/back.png", 1.1, 1.1, screen.width(), screen.height());

    mainMenu.addButton(screen.width()/2, 200, 200, 20, [this] () { this->play(); }, "Play", 5, 5, "../textures/gui.png", {0, 66}, {0, 86}, {0, 46}, "../tdzavrlib/fonts/Roboto-Medium.ttf", {255, 255, 255}, "../sound/click.ogg");
    mainMenu.addButton(screen.width()/2, 350, 200, 20, [this] () { this->map.saveMap("../maps/map_test"); }, "Save map", 5, 5, "../textures/gui.png", {0, 66}, {0, 86}, {0, 46}, "../tdzavrlib/fonts/Roboto-Medium.ttf", {255, 255, 255}, "../sound/click.ogg");
    mainMenu.addButton(screen.width()/2, 650, 200, 20, [this] () { this->exit(); }, "Exit", 5, 5, "../textures/gui.png", {0, 66}, {0, 86}, {0, 46}, "../tdzavrlib/fonts/Roboto-Medium.ttf", {255, 255, 255}, "../sound/click.ogg");
}

void Minecraft::update(double elapsedTime) {
    // This code executed every time step:

    // Check all input after this condition please
    if (!screen.window.hasFocus())
        return;

    if(screen.isKeyTapped(sf::Keyboard::Escape)) {
        inGame = !inGame;
        screen.setMouseCursorVisible(!inGame);
    }

    if(inGame) {
        screen.title("Minecraft");
        if (cameraMode == CameraMode::LocalCamera)
            player.update();
        else
            externalCameraController.update();

        if (screen.isKeyTapped(sf::Keyboard::E)) {
            switchCamera();
        }
    } else {
        mainMenu.update(screen);
    }


    setUpdateWorld(inGame);

    //auto rayCast = world.rayCast(camera.position(), camera.position() + camera.lookAt()*10);
    //world["point"].translateToPoint(rayCast.first);


    // background sounds and music control
    if(backgroundNoise.getStatus() != sf::Sound::Status::Playing) {
        backgroundNoise.setBuffer(*ResourceManager::loadSoundBuffer("../sound/backNoise.ogg"));
        backgroundNoise.setVolume(5);
        backgroundNoise.play();
    }

    if(music.getStatus() != sf::Sound::Status::Playing) {
        int soundNum = round((double) rand() / RAND_MAX * 8) + 1; // random number from 1 to 9
        music.setBuffer(*ResourceManager::loadSoundBuffer("../sound/music/music" + std::to_string(soundNum) + ".ogg"));
        music.setVolume(15);
        music.play();
    }
}

void Minecraft::switchCamera() {
    if(cameraMode == CameraMode::LocalCamera)
        setCameraMode(CameraMode::ExternalObserver);
    else
        setCameraMode(CameraMode::LocalCamera);
}

void Minecraft::gui() {

    if(inGame) {
        sf::Sprite sprite;
        sprite.setTexture(*ResourceManager::loadTexture("../textures/gui.png"));
        sprite.setTextureRect(sf::IntRect(243, 3, 9, 9));
        sprite.scale(6, 6);
        sprite.setPosition(screen.width() / 2 - 27, screen.height() / 2 - 27);
        screen.window.draw(sprite);
    }
}

void Minecraft::play() {
    screen.setMouseCursorVisible(false);
    inGame = true;
}

int main(int argc, char* argv[]) {
    Minecraft game;
    game.create(1920, 1080);
    //game.create(2048, 1152);
    //game.create(3072, 1920, "Minecraft", true);
    //game.create(3840, 2160);

    return 0;
}