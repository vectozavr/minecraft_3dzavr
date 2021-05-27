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
#include <cmath>

#include "Client.h"
#include "Server.h"
#include <fstream>

using namespace std;

// Read server/client settings and start both.
// If client doesn't connect to the localhost - server doesn't start.
void InitNetwork(const shared_ptr<Server>& server, const shared_ptr<Client>& client)
{
    std::string clientIp;
    sf::Uint16 clientPort;
    sf::Uint16 serverPort;
    std::ifstream connectfile("connect.txt", std::ifstream::in);

    // If failed to read client settings
    if (!connectfile.is_open() || !(connectfile >> clientIp >> clientPort) || sf::IpAddress(clientIp) == sf::IpAddress::None)
    {
        connectfile.close();
        // Create file and write default settings
        clientIp = "127.0.0.1";
        clientPort = 54000;
        std::ofstream temp("connect.txt", std::ofstream::out);
        temp << clientIp << std::endl << clientPort;
        temp.close();
    }
    connectfile.close();

    // If failed to read server settings
    connectfile.open("server.txt", std::ifstream::in);
    if (!connectfile.is_open() || !(connectfile >> serverPort))
    {
        connectfile.close();
        // Create file and write default settings
        serverPort = 54000;
        std::ofstream temp("server.txt", std::ofstream::out);
        temp << serverPort;
        temp.close();
    }
    connectfile.close();

    if (clientIp == sf::IpAddress::LocalHost)
        server->start(serverPort);

    client->connect(clientIp, clientPort);
}

class Minecraft : public Tdzavr {
private:
    CameraController externalCameraController;

    shared_ptr<Player> player;
    shared_ptr<Map> map;

    sf::Sound backgroundNoise;
    sf::Sound music;

    Window mainMenu;
    Window settings;
    Window about;

    shared_ptr<Server> server;
    shared_ptr<Client> client;

    bool inGame = false;

public:
    Minecraft();

    void start() override;
    void update(double elapsedTime) override;

    void gui() override;

    void switchCamera();

    void play();
};

Minecraft::Minecraft() : externalCameraController(external_camera, screen)
{

}

void Minecraft::start() {
    // This code executed once in the beginning:

    screen->setMouseCursorVisible(true);

    //world.loadObj("../obj/cube.obj", "point", {0.1, 0.1, 0.1});
    //world["point"].setColor({255, 0, 0});

    map = std::make_shared<Map>(world);
    map->loadMap("../maps/map_test");

    player = std::make_shared<Player>();
    client = std::make_shared<Client>(player, world, map);
    server = std::make_shared<Server>();

    player->attachWorld(world, map);

    setUpdateWorld(false);

    player->attachCamera(camera, screen);
    world->addMesh(player, player->name());

    player->add_removeCubeCallBack([this] (Pos pos, Cube::Type type) {client->addCube(pos, type); }, [this] (Pos pos) {client->removeCube(pos);});

    // windows init:
    mainMenu.title("Main menu");
    mainMenu.setBackgroundTexture("../textures/back.png", 1.1, 1.1, screen->width(), screen->height());

    mainMenu.addButton(screen->width()/2, 200, 200, 20, [this] () { this->play(); }, "Play", 5, 5, "../textures/gui.png", {0, 66}, {0, 86}, {0, 46}, "../tdzavrlib/fonts/Roboto-Medium.ttf", {255, 255, 255}, "../sound/click.ogg");
    mainMenu.addButton(screen->width()/2, 350, 200, 20, [this] () { this->player->translateToPoint({0, 6, 0}); this->player->setVelocity({}); this->play(); }, "Respawn", 5, 5, "../textures/gui.png", {0, 66}, {0, 86}, {0, 46}, "../tdzavrlib/fonts/Roboto-Medium.ttf", {255, 255, 255}, "../sound/click.ogg");
    mainMenu.addButton(screen->width()/2, 500, 200, 20, [this] () { this->map->saveMap("../maps/map_test"); }, "Save map", 5, 5, "../textures/gui.png", {0, 66}, {0, 86}, {0, 46}, "../tdzavrlib/fonts/Roboto-Medium.ttf", {255, 255, 255}, "../sound/click.ogg");


    mainMenu.addButton(screen->width()/2, 750, 200, 20, [this] () { client->disconnect(); server->stop(); this->exit();}, "Exit", 5, 5, "../textures/gui.png", {0, 66}, {0, 86}, {0, 46}, "../tdzavrlib/fonts/Roboto-Medium.ttf", {255, 255, 255}, "../sound/click.ogg");

    // connecting to the server
    InitNetwork(server, client);
    // Waiting for connect and updating server if it's same window
    while (client->isWorking() && !client->connected())
    {
        client->update();
        server->update();
        Time::update();
    }
    // If connect fail - return to menu
    if (!client->isWorking())
    {
        inGame = false;
        server->stop();
    }
}

void Minecraft::update(double elapsedTime) {
    // This code executed every time step:

    server->update();
    client->update();

    // Check all input after this condition please
    if (!screen->window.hasFocus())
        return;

    if(screen->isKeyTapped(sf::Keyboard::Escape)) {
        inGame = !inGame;
        screen->setMouseCursorVisible(!inGame);

        /*
        if(!inGame) {
            server->stop();
            client->disconnect();
        }
         */
    }

    if(inGame) {
        screen->title("Minecraft");
        if (cameraMode == CameraMode::LocalCamera)
            player->update();
        else
            externalCameraController.update();

        if (screen->isKeyTapped(sf::Keyboard::E)) {
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
        sprite.setPosition(screen->width() / 2 - 27, screen->height() / 2 - 27);
        screen->window.draw(sprite);
    }
}

void Minecraft::play() {
    inGame = true;

    screen->setMouseCursorVisible(!inGame);
}

int main(int argc, char* argv[]) {
    Minecraft game;
    game.create(1280, 720, "Minecraft");
    //game.create(1920, 1080, "Minecraft");
    //game.create(2048, 1152, "Minecraft");
    //game.create(3072, 1920, "Minecraft");
    //game.create(3840, 2160, "Minecraft");

    return 0;
}