//
// Created by Иван Ильин on 14.03.2021.
//

#include "Player.h"
#include "tdzavrlib/Screen.h"
#include <iostream>

void Player::update() {

    bool inRunning_old = inRunning;
    inRunning = Screen::isKeyPressed(sf::Keyboard::A) || Screen::isKeyPressed(sf::Keyboard::D) ||Screen::isKeyPressed(sf::Keyboard::W) || Screen::isKeyPressed(sf::Keyboard::S);
    if(!inRunning_old && inRunning) {
        camera.a_stopAnimations();
        camera.a_wait(0);
        camera.a_translate(-Point4D{0, 1, 0}/4, 0.2, Animation::LoopOut::Cycle, Animation::cos);
    } else if(inRunning_old && !inRunning) {
        camera.a_stopAnimations();
        camera.a_translateToPoint(hitBox().position() + Point4D{0, 1.8, 0}, 0.2, Animation::None, Animation::cos);
    }

    // Left and right
    if (Screen::isKeyPressed(sf::Keyboard::A))
        hitBox().translate(camera.left()*Time::deltaTime()*walkSpeed);

    if (Screen::isKeyPressed(sf::Keyboard::D))
        hitBox().translate(-camera.left()*Time::deltaTime()*walkSpeed);

    // Forward and backward
    if (Screen::isKeyPressed(sf::Keyboard::W))
        hitBox().translate(camera.left().cross3D({0, 1, 0})*Time::deltaTime()*walkSpeed);

    if (Screen::isKeyPressed(sf::Keyboard::S))
        hitBox().translate(-camera.left().cross3D({0, 1, 0})*Time::deltaTime()*walkSpeed);

    if (Screen::isKeyPressed(sf::Keyboard::Space) && hitBox().inCollision()) {
        hitBox().setVelocity(hitBox().velocity() + Point4D{0, sqrt(2 * g * jumpHeight), 0});
    }

    //camera.translateToPoint(hitBox().position() + Point4D{0, 1.8, 0});

    // Mouse movement
    Point4D disp = screen.getMouseDisplacement();

    camera.rotateY(-disp.x / 1000.0);
    hitBox().rotate({0, -disp.x / 1000.0, 0});

    double rotationLeft = disp.y / 1000.0;

    // You can only see in range [-90 : 90] grad
    if(camera.angleLeftUpLookAt().x + rotationLeft > M_PI/2)
        rotationLeft = M_PI / 2 - camera.angleLeftUpLookAt().x;
    if(camera.angleLeftUpLookAt().x + rotationLeft < -M_PI/2)
        rotationLeft = -M_PI / 2 - camera.angleLeftUpLookAt().x;

    camera.rotateLeft(rotationLeft);
}
