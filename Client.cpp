//
// Created by Иван Ильин on 25.05.2021.
//

#include "Client.h"
#include "utils/Log.h"

void Client::updatePacket() {
    sf::Packet packet;
    packet << MsgType::ClientUpdate << _player->position().x << _player->position().y << _player->position().z << _player->angle().y << _player->camera()->angleLeftUpLookAt().x;
    _socket.send(packet, _socket.serverId());
}

void Client::processInit(sf::Packet& packet) {
    sf::Uint16 targetId;
    double buf[4];

    while (packet >> targetId >> buf[0] >> buf[1] >> buf[2] >> buf[3])
    {
        if(targetId != _socket.ownId()) {
            std::string name = "Player" + std::to_string(targetId);
            _players.insert({ targetId, std::make_shared<Player>() });
            (*_world).addMesh(_players[targetId], name);
            _players[targetId]->setVisible(true);
            _players[targetId]->setAcceleration({0, 0, 0});

            // add head and other stuff:
            _world->loadObj("../obj/cube.obj", name + "_head",{0.7, 0.7, 0.7});
            (*_world)[name + "_head"]->translate({0, 2, 0});
            (*_world)[name + "_head"]->setCollider(false);
            _players[targetId]->attach((*_world)[name + "_head"]);

            _world->loadObj("../obj/cube.obj", name + "_eye1",{0.2, 0.2, 0.05});
            (*_world)[name + "_eye1"]->translate({0.3, 2.1, 0.7});
            (*_world)[name + "_eye1"]->setCollider(false);
            (*_world)[name + "_eye1"]->setColor({147, 159, 255});
            (*_world)[name + "_head"]->attach((*_world)[name + "_eye1"]);

            _world->loadObj("../obj/cube.obj", name + "_eye2",{0.2, 0.2, 0.05});
            (*_world)[name + "_eye2"]->translate({-0.3, 2.1, 0.7});
            (*_world)[name + "_eye2"]->setCollider(false);
            (*_world)[name + "_eye2"]->setColor({147, 159, 255});
            (*_world)[name + "_head"]->attach((*_world)[name + "_eye2"]);

            _players[targetId]->translateToPoint({ buf[0], buf[1], buf[2]});
            _players[targetId]->setHealth(buf[3]);
        }
    }
}

void Client::processUpdate(sf::Packet& packet) {
    sf::Uint16 targetId;
    double buf[6];

    while (packet >> targetId >> buf[0] >> buf[1] >> buf[2] >> buf[3] >> buf[4] >> buf[5])
    {
        if (_players.count(targetId))
        {
            if (targetId != _socket.ownId()) {
                std::string name = "Player" + std::to_string(targetId);

                _players[targetId]->translateToPoint({buf[0], buf[1], buf[2]});
                _players[targetId]->setHealth(buf[3]);
                _players[targetId]->rotateToAngle({0, buf[4], 0});
                //(*_world)[name + "_head"]->rotateToAngle({buf[5], 2*buf[4], 0});
                (*_world)[name + "_head"]->rotate(Matrix4x4::RotationY(buf[4])*Point4D{1, 0, 0}, buf[5] - _players[targetId]->headAngle());
                _players[targetId]->setHeadAngle(buf[5]);
            }else
                _player->setHealth(buf[3]);
        }
    }
}

void Client::processNewClient(sf::Packet& packet) {
    sf::Uint16 targetId;

    packet >> targetId;

    std::string name = "Player" + std::to_string(targetId);
    _players.insert({ targetId, std::make_shared<Player>() });
    _world->addMesh(_players[targetId], name);
    _players[targetId]->setVisible(true);
    _players[targetId]->setAcceleration({0, 0, 0});

    // add head and other stuff:
    _world->loadObj("../obj/cube.obj", name + "_head",{0.7, 0.7, 0.7});
    (*_world)[name + "_head"]->translate({0, 2, 0});
    (*_world)[name + "_head"]->setCollider(false);
    _players[targetId]->attach((*_world)[name + "_head"]);

    _world->loadObj("../obj/cube.obj", name + "_eye1",{0.2, 0.2, 0.05});
    (*_world)[name + "_eye1"]->translate({0.3, 2.1, 0.7});
    (*_world)[name + "_eye1"]->setCollider(false);
    (*_world)[name + "_eye1"]->setColor({147, 159, 255});
    (*_world)[name + "_head"]->attach((*_world)[name + "_eye1"]);

    _world->loadObj("../obj/cube.obj", name + "_eye2",{0.2, 0.2, 0.05});
    (*_world)[name + "_eye2"]->translate({-0.3, 2.1, 0.7});
    (*_world)[name + "_eye2"]->setCollider(false);
    (*_world)[name + "_eye2"]->setColor({147, 159, 255});
    (*_world)[name + "_head"]->attach((*_world)[name + "_eye2"]);
}

void Client::processDisconnect(sf::Uint16 targetId) {
    if (targetId != _socket.ownId() && _players.count(targetId)) {
        std::string name = "Player" + std::to_string(targetId);
        _world->removeMesh(name);
        _world->removeMesh(name + "_head");
        _world->removeMesh(name + "_eye1");
        _world->removeMesh(name + "_eye2");

        _players.erase(targetId);
    }
}


void Client::processCustomPacket(MsgType type, sf::Packet& packet) {
    Cube::Type cubeType;
    int buff[3];

    switch (type) {
        case MsgType::AddCube:
            sf::Uint16 tmp;
            packet >> tmp >> buff[0] >> buff[1] >> buff[2];
            cubeType = Cube::Type(tmp);
            _map->addCube(cubeType, buff[0], buff[1], buff[2]);
            Log::log("Client: AddCube (" + std::to_string(tmp) + ") at [" + std::to_string(buff[0]) + ", " + std::to_string(buff[1]) + ", " + std::to_string(buff[2]) + "]");

            break;
        case MsgType::RemoveCube:
            packet >> buff[0] >> buff[1] >> buff[2];
            _map->removeCube(buff[0], buff[1], buff[2]);
            Log::log("Client: RemoveCube at [" + std::to_string(buff[0]) + ", " + std::to_string(buff[1]) + ", " + std::to_string(buff[2]) + "]");

            break;
    }
}

void Client::processDisconnected() {
    for (auto it = _players.begin(); it != _players.end();) {
        std::string name = "Player" + std::to_string(it->first);

        _world->removeMesh(name);
        _world->removeMesh(name + "_head");
        _world->removeMesh(name + "_eye1");
        _world->removeMesh(name + "_eye2");
        _players.erase(it++);
    }
}

void Client::addCube(Pos pos, Cube::Type type) {
    sf::Packet packet;

    packet << MsgType::AddCube << (sf::Uint16)type << pos.x << pos.y << pos.z;
    _socket.send(packet, _socket.serverId());

    Log::log("Client: AddCube (" + std::to_string(type) + ") at [" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + "]");
}

void Client::removeCube(Pos pos) {
    sf::Packet packet;
    packet << MsgType::RemoveCube << pos.x << pos.y << pos.z;
    _socket.send(packet, _socket.serverId());

    Log::log("Client: RemoveCube at [" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + "]");
}
