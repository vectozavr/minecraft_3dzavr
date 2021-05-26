//
// Created by Иван Ильин on 25.05.2021.
//

#ifndef MINECRAFT_3DZAVR_CLIENT_H
#define MINECRAFT_3DZAVR_CLIENT_H

#include <utility>

#include "tdzavrlib/network/ClientUDP.h"
#include "Player.h"

class Client : public ClientUDP {
private:
    std::shared_ptr<Player> _player;
    std::shared_ptr<World> _world;
    std::shared_ptr<Map> _map;

    std::map<sf::Uint16, std::shared_ptr<Player>> _players{};
public:
    Client(std::shared_ptr<Player>  player, std::shared_ptr<World>  world, std::shared_ptr<Map>  map) : _player(std::move(player)), _world(std::move(world)), _map(std::move(map)) {};

    void updatePacket() override;

    void processInit(sf::Packet& packet) override;
    void processUpdate(sf::Packet& packet) override;
    void processNewClient(sf::Packet& packet) override;
    void processDisconnect(sf::Uint16 targetId) override;

    void processCustomPacket(MsgType type, sf::Packet& packet) override;

    void processDisconnected() override;

    void addCube(Pos pos, Cube::Type type);
    void removeCube(Pos pos);
};


#endif //MINECRAFT_3DZAVR_CLIENT_H
