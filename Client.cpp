//
// Created by Иван Ильин on 25.05.2021.
//

#include "Client.h"
#include "utils/Log.h"

void Client::updatePacket() {
    sf::Packet packet;
    packet << MsgType::ClientUpdate << _player->position().x << _player->position().y << _player->position().z;
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

            _players[targetId]->translateToPoint({ buf[0], buf[1], buf[2]});
            _players[targetId]->setHealth(buf[3]);
            _players[targetId]->setVisible(true);
            _players[targetId]->setAcceleration({0, 0, 0});
        }
    }
}

void Client::processUpdate(sf::Packet& packet) {
    sf::Uint16 targetId;
    double buf[4];

    while (packet >> targetId >> buf[0] >> buf[1] >> buf[2] >> buf[3])
    {
        if (_players.count(targetId))
        {
            if (targetId != _socket.ownId()) {
                _players[targetId]->translateToPoint({buf[0], buf[1], buf[2]});
                _players[targetId]->setHealth(buf[3]);
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
}

void Client::processDisconnect(sf::Uint16 targetId) {
    if (targetId != _socket.ownId() && _players.count(targetId)) {
        std::string name = "Player" + std::to_string(targetId);
        _world->removeMesh(name);
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
        _world->removeMesh("Player" + std::to_string(it->first));
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
