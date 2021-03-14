//
// Created by Иван Ильин on 14.03.2021.
//

#include "Player.h"
#include "tdzavrlib/Screen.h"
#include <iostream>

void Player::update() {
    // Left and right
    if (Screen::isKeyPressed(sf::Keyboard::A))
        hitBox().translate(camera.left()*Time::deltaTime()*5.0);

    if (Screen::isKeyPressed(sf::Keyboard::D))
        hitBox().translate(-camera.left()*Time::deltaTime()*5.0);

    // Forward and backward
    if (Screen::isKeyPressed(sf::Keyboard::W))
        hitBox().translate(camera.left().cross3D({0, 1, 0})*Time::deltaTime()*5.0);

    if (Screen::isKeyPressed(sf::Keyboard::S))
        hitBox().translate(-camera.left().cross3D({0, 1, 0})*Time::deltaTime()*5.0);

    if (Screen::isKeyPressed(sf::Keyboard::LShift))
        hitBox().translate(0.0, -Time::deltaTime()*5.0, 0);

    if (Screen::isKeyPressed(sf::Keyboard::Space) && hitBox().inCollision())
        hitBox().setVelocity(hitBox().velocity() + Point4D{0, sqrt(2*g*1.5), 0});

    camera.translateToPoint(hitBox().position() + Point4D{0, 1.8, 0});

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
