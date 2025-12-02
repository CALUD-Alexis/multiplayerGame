#include <asio.hpp>
#include <iostream>
#include <unordered_map>
#include <thread>

using asio::ip::tcp;

struct Player {
    float x = 100;
    float y = 100;
};

std::unordered_map<int, Player> players;

int main() {
    asio::io_context io;
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 4000));

    std::cout << "Serveur lancé sur le port 4000\n";

    int nextId = 0;

    while (true) {
        tcp::socket socket(io);
        acceptor.accept(socket);

        int id = nextId;
        nextId++;

        players[id] = Player();
        std::cout << "Nouveau joueur : " << id << "\n";

        std::thread([id, s = std::move(socket)]() mutable {
            try {
                while (true) {
                    char cmd;
                    asio::read(s, asio::buffer(&cmd, 1));

                    Player &p = players[id];
                    if (cmd == 'W') p.y -= 5;
                    if (cmd == 'S') p.y += 5;
                    if (cmd == 'A') p.x -= 5;
                    if (cmd == 'D') p.x += 5;

                    float data[2] = {p.x, p.y};
                    asio::write(s, asio::buffer(data));
                }
            } catch (...) {
                std::cout << "Joueur déconnecté : " << id << "\n";
                players.erase(id);
            }
        }).detach();
    }
}
