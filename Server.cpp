//
// Created by Иван Ильин on 25.05.2021.
//

#include "Server.h"
#include "utils/Log.h"

void Server::broadcast() {
    sf::Packet updatePacket;
    updatePacket << MsgType::Update;

    for (auto& player : _players)
    {
        player.second->setHealth(player.second->health() - (Time::time() - _lastBroadcast));
        updatePacket << player.first << player.second->position().x << player.second->position().y << player.second->position().z << player.second->health();
    }

    for (auto& player : _players)
    {
        _socket.send(updatePacket, player.first);
    }
}


void Server::processConnect(sf::Uint16 targetId) {
    sf::Packet sendPacket;
    sf::Packet extraPacket;

    extraPacket << MsgType::NewClient << targetId;
    sendPacket << MsgType::Init << targetId;
    _players.insert({ targetId, std::make_shared<Player>() });
    for (const auto& player : _players)
    {
        sendPacket << player.first << player.second->position().x << player.second->position().y << player.second->position().z << player.second->health();
        if (player.first != targetId)
            _socket.sendRely(extraPacket, player.first);
    }
    _socket.sendRely(sendPacket, targetId);
}

void Server::processClientUpdate(sf::Uint16 senderId, sf::Packet& packet) {
    double buf[3];

    packet >> buf[0] >> buf[1] >> buf[2];
    _players.at(senderId)->translateToPoint({ buf[0], buf[1], buf[2] });
}

void Server::processDisconnect(sf::Uint16 senderId) {
    sf::Packet sendPacket;

    sendPacket << MsgType::Disconnect << senderId;
    _players.erase(senderId);
    _socket.removeConnection(senderId);
    for (const auto& player : _players)
        _socket.sendRely(sendPacket, player.first);
}


void Server::processCustomPacket(MsgType type, sf::Packet& packet, sf::Uint16 senderId) {
    sf::Packet sendPacket;
    int buf[3];
    sf::Uint16 tmp;
    Cube::Type cubeType;

    switch (type) {
        case MsgType::AddCube:
            packet >> tmp >> buf[0] >> buf[1] >> buf[2];
            sendPacket << MsgType::AddCube << tmp << buf[0] << buf[1] << buf[2];
            for(auto& _player : _players) {
                if(senderId != _player.first)
                    _socket.sendRely(sendPacket, _player.first);
            }

            Log::log("Server: AddCube.");

            break;
        case MsgType::RemoveCube:
            packet >> buf[0] >> buf[1] >> buf[2];
            sendPacket << MsgType::RemoveCube << buf[0] << buf[1] << buf[2];
            for(auto& _player : _players) {
                if(senderId != _player.first)
                    _socket.sendRely(sendPacket, _player.first);
            }
            Log::log("Server: RemoveCube.");
            break;
    }
}

void Server::processStop() {
    for (auto it = _players.begin(); it != _players.end();)
        _players.erase(it++);
}
