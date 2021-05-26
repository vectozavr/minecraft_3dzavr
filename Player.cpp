//
// Created by Иван Ильин on 14.03.2021.
//

#include "Player.h"
#include "tdzavrlib/Screen.h"
#include <iostream>
#include "tdzavrlib/ResourceManager.h"
#include "tdzavrlib/utils/Log.h"

void Player::update() {
    bool inRunning_old = inRunning;
    inRunning = _screen != nullptr && (Screen::isKeyPressed(sf::Keyboard::A) || Screen::isKeyPressed(sf::Keyboard::D) ||Screen::isKeyPressed(sf::Keyboard::W) || Screen::isKeyPressed(sf::Keyboard::S));

    // in case when the camera is attached we make some animation during running
    if(_camera != nullptr && inRunning && !_camera->isInAnim()) {
        _camera->a_translate("hor_oscil", -_camera->left()/3, 0.3, Animation::LoopOut::None, Animation::cos);
        _camera->a_wait("hor_oscil", 0);
        _camera->a_translate("hor_oscil", _camera->left()/3, 0.3, Animation::LoopOut::None, Animation::cos);

        _camera->a_translate("vert_oscil", -Point4D{0, 1, 0}/6, 0.15, Animation::LoopOut::None, Animation::cos);
        _camera->a_wait("vert_oscil", 0);
        _camera->a_translate("vert_oscil", Point4D{0, 1, 0}/6, 0.15, Animation::LoopOut::None, Animation::cos);
        _camera->a_wait("vert_oscil", 0);
        _camera->a_translate("vert_oscil", -Point4D{0, 1, 0}/6, 0.15, Animation::LoopOut::None, Animation::cos);
        _camera->a_wait("vert_oscil", 0);
        _camera->a_translate("vert_oscil", Point4D{0, 1, 0}/6, 0.15, Animation::LoopOut::None, Animation::cos);

        _camera->a_translateToPoint("init", position() + Point4D{0, 1.8, 0}, 0.3, Animation::None, Animation::cos);

    } else if(_camera != nullptr && inRunning_old && !inRunning) {
        _camera->a_stopAllAnimations();
        _camera->a_translateToPoint("init", position() + Point4D{0, 1.8, 0}, 0.15, Animation::None, Animation::cos);
    }

    auto rayToFloor = (*_world).rayCast(position(), position() + Point4D{0, -5, 0});

    if(_world != nullptr && _map != nullptr && _screen != nullptr && _camera != nullptr) {
        // Left and right
        if (Screen::isKeyPressed(sf::Keyboard::A))
            translate(_camera->left() * Time::deltaTime() * walkSpeed);

        if (Screen::isKeyPressed(sf::Keyboard::D))
            translate(-_camera->left() * Time::deltaTime() * walkSpeed);

        // Forward and backward
        if (Screen::isKeyPressed(sf::Keyboard::W))
            translate(_camera->left().cross3D({0, 1, 0}) * Time::deltaTime() * walkSpeed);

        if (Screen::isKeyPressed(sf::Keyboard::S))
            translate(-_camera->left().cross3D({0, 1, 0}) * Time::deltaTime() * walkSpeed);

        if (Screen::isKeyPressed(sf::Keyboard::LShift))
            translate(Point4D{0, -1, 0} * Time::deltaTime() * walkSpeed);

        if (Screen::isKeyPressed(sf::Keyboard::Space) && inCollision() && (position() - rayToFloor.first).abs() < 6) {
            setVelocity(velocity() + Point4D{0, sqrt(2 * g * jumpHeight), 0});
        }

        // Mouse movement
        Point4D disp = _screen->getMouseDisplacement();

        //camera.rotateY(-disp.x / 1000.0);
        rotate({0, -disp.x / 1000.0, 0});

        double rotationLeft = disp.y / 1000.0;

        // You can only see in range [-90 : 90] grad
        if (_camera->angleLeftUpLookAt().x + rotationLeft > M_PI / 2)
            rotationLeft = M_PI / 2 - _camera->angleLeftUpLookAt().x;
        if (_camera->angleLeftUpLookAt().x + rotationLeft < -M_PI / 2)
            rotationLeft = -M_PI / 2 - _camera->angleLeftUpLookAt().x;

        _camera->rotateLeft(rotationLeft);
        (*_world)[name() + "_hand"]->rotateRelativePoint(position() + Point4D{0, 1.8, 0}, _camera->left(), rotationLeft);

        if (_screen->isKeyTapped(sf::Keyboard::Right)) {
            // change 'selectedBlock'
            selectedBlock = static_cast<Cube::Type>(((int) selectedBlock + 1) % (int) Cube::Type::none);
            Log::log("selected block " + std::to_string(selectedBlock));
            (*_world)[name() + "cube_in_hand"]->setColor(Cube::cubeColor(selectedBlock));
        }
        if (_screen->isKeyTapped(sf::Keyboard::Left)) {
            // change 'selectedBlock'
            if ((int) selectedBlock > 0) {
                selectedBlock = static_cast<Cube::Type>(((int) selectedBlock - 1) % (int) Cube::Type::none);
                Log::log("selected block " + std::to_string(selectedBlock));
            } else {
                selectedBlock = static_cast<Cube::Type>((int) Cube::Type::none - 1);
                Log::log("selected block " + std::to_string(selectedBlock));
            }
            (*_world)[name() + "cube_in_hand"]->setColor(Cube::cubeColor(selectedBlock));
        }
        if (_screen->isButtonTapped(sf::Mouse::Button::Right)) {
            auto rayCast = (*_world).rayCast(_camera->position(), _camera->position() + _camera->lookAt() * 10);

            if ((rayCast.first - _camera->position()).abs() < 10) {
                Point4D cubePoint = rayCast.second.pos() + rayCast.second.norm();
                (*_map).addCube(selectedBlock, round(cubePoint.x / 2), round(cubePoint.y / 2), round(cubePoint.z / 2));

                //world["player_hand"].a_translate("add_or_remove_cube", camera.lookAt(), 0.08, Animation::None, Animation::cos);
                //world["player_hand"].a_wait("add_or_remove_cube", 0);
                //world["player_hand"].a_translate("add_or_remove_cube", -camera.lookAt(), 0.08, Animation::None, Animation::cos);

                // sound
                std::string bufferName;
                switch (selectedBlock) {
                    case Cube::Type::stone:
                        bufferName = "stone";
                        break;
                    case Cube::Type::snow:
                        bufferName = "stone";
                        break;
                    case Cube::Type::earth:
                        bufferName = "gravel";
                        break;
                    case Cube::Type::wood:
                        bufferName = "wood";
                        break;
                    case Cube::Type::grass:
                        bufferName = "grass";
                        break;
                    case Cube::Type::sand:
                        bufferName = "sand";
                        break;
                    case Cube::Type::water:
                        bufferName = "stone";
                        break;
                    case Cube::Type::glass:
                        bufferName = "stone";
                        break;
                    case Cube::Type::none:
                        bufferName = "stone";
                }
                int soundNum = round((double) rand() / RAND_MAX * 3) + 1; // random number from 1 to 4
                addBlock.setBuffer(*ResourceManager::loadSoundBuffer(
                        "../sound/" + bufferName + std::to_string(soundNum) + ".ogg"));
                addBlock.play();
            }
        }
        if (_screen->isButtonTapped(sf::Mouse::Button::Left)) {
            auto rayCast = (*_world).rayCast(_camera->position(), _camera->position() + _camera->lookAt() * 10);

            if ((rayCast.first - _camera->position()).abs() < 10) {
                Point4D cubePoint = rayCast.second.pos() - rayCast.second.norm();
                Cube::Type t = (*_map).removeCube(round(cubePoint.x / 2), round(cubePoint.y / 2), round(cubePoint.z / 2));

                if (t != Cube::none) {
                    //world["player_hand"].a_translate("add_or_remove_cube", camera.lookAt(), 0.08, Animation::None, Animation::cos);
                    //world["player_hand"].a_wait("add_or_remove_cube", 0);
                    //world["player_hand"].a_translate("add_or_remove_cube", -camera.lookAt(), 0.08, Animation::None, Animation::cos);

                    // sound
                    std::string bufferName;
                    switch (t) {
                        case Cube::Type::stone:
                            bufferName = "stone";
                            break;
                        case Cube::Type::snow:
                            bufferName = "stone";
                            break;
                        case Cube::Type::earth:
                            bufferName = "gravel";
                            break;
                        case Cube::Type::wood:
                            bufferName = "wood";
                            break;
                        case Cube::Type::grass:
                            bufferName = "grass";
                            break;
                        case Cube::Type::sand:
                            bufferName = "sand";
                            break;
                        case Cube::Type::water:
                            bufferName = "glass";
                            break;
                        case Cube::Type::glass:
                            bufferName = "glass";
                            break;
                        case Cube::Type::none:
                            bufferName = "stone";
                    }
                    int soundNum = round((double) rand() / RAND_MAX * 3) + 1; // random number from 1 to 4
                    removeBlock.setBuffer(*ResourceManager::loadSoundBuffer(
                            "../sound/" + bufferName + std::to_string(soundNum) + ".ogg"));
                    removeBlock.play();
                }
            }
        }

        if (inRunning && inCollision() && walk.getStatus() != sf::Sound::Status::Playing) {
            if ((position() - rayToFloor.first).abs() < 2) {
                int soundNum = round((double) rand() / RAND_MAX * 5) + 1;
                walk.setBuffer(
                        *ResourceManager::loadSoundBuffer("../sound/stonestep" + std::to_string(soundNum) + ".ogg"));
                walk.setVolume(30);
                walk.play();
            }
        }

        if ((oldVelocity - velocity()).abs() > 30) {
            fall.setBuffer(*ResourceManager::loadSoundBuffer("../sound/fallbig.ogg"));
            //fall.setVolume(30);
            fall.play();
        }
    }

    oldVelocity = velocity();
}
