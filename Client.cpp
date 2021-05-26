//
// Created by Иван Ильин on 25.05.2021.
//

#include "Client.h"

void Client::updatePacket() {
    std::string name = _player->name();

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
    ClientUDP::processCustomPacket(type, packet);
}

void Client::processDisconnected() {
    for (auto it = _players.begin(); it != _players.end();) {
        _world->removeMesh("Player" + std::to_string(it->first));
        _players.erase(it++);
    }
}
