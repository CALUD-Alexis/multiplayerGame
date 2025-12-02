#include <asio.hpp>
#include <iostream>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <vector>

using asio::ip::tcp;

struct Player {
    float x = 300;
    float y = 250;
    uint8_t r, g, b;
};

std::unordered_map<int, Player> players;
std::mutex playersMutex;

// Couleurs prédéfinies pour les joueurs
const std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> colors = {
    {255, 100, 100},  // Rouge
    {100, 255, 100},  // Vert
    {100, 100, 255},  // Bleu
    {255, 255, 100},  // Jaune
    {255, 100, 255},  // Magenta
    {100, 255, 255},  // Cyan
    {255, 150, 100},  // Orange
    {150, 100, 255},  // Violet
    {100, 255, 150},  // Vert clair
    {255, 100, 150},  // Rose
};

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

        // Assigner une couleur unique basée sur l'ID
        auto [r, g, b] = colors[id % colors.size()];

        {
            std::lock_guard<std::mutex> lock(playersMutex);
            players[id] = Player{300, 250, r, g, b};
        }

        std::cout << "Nouveau joueur : " << id << " (couleur: " << (int)r << "," << (int)g << "," << (int)b << ")\n";

        std::thread([id, s = std::move(socket)]() mutable {
            try {
                // Envoyer l'ID du joueur
                asio::write(s, asio::buffer(&id, sizeof(id)));

                while (true) {
                    char cmd;
                    asio::read(s, asio::buffer(&cmd, 1));

                    // Mettre à jour la position du joueur
                    {
                        std::lock_guard<std::mutex> lock(playersMutex);
                        Player &p = players[id];
                        if (cmd == 'W') p.y -= 5;
                        if (cmd == 'S') p.y += 5;
                        if (cmd == 'A') p.x -= 5;
                        if (cmd == 'D') p.x += 5;
                    }

                    // Envoyer tous les joueurs
                    std::lock_guard<std::mutex> lock(playersMutex);
                    int playerCount = players.size();
                    asio::write(s, asio::buffer(&playerCount, sizeof(playerCount)));

                    for (const auto& [playerId, player] : players) {
                        asio::write(s, asio::buffer(&playerId, sizeof(playerId)));
                        asio::write(s, asio::buffer(&player, sizeof(player)));
                    }
                }
            } catch (...) {
                std::cout << "Joueur déconnecté : " << id << "\n";
                std::lock_guard<std::mutex> lock(playersMutex);
                players.erase(id);
            }
        }).detach();
    }
}
