#include <SFML/Graphics.hpp>
#include <asio.hpp>
#include <unordered_map>
#include <cstring>

using asio::ip::tcp;

struct PlayerData {
    float x, y;
    uint8_t r, g, b;
};

int main() {
    asio::io_context io;
    tcp::socket socket(io);
    socket.connect(tcp::endpoint(asio::ip::make_address("54.37.252.158"), 4000));

    sf::RenderWindow window(sf::VideoMode({800, 600}), "MultiGame Client");

    int myId = -1;
    std::unordered_map<int, PlayerData> players;

    // Recevoir mon ID et ma couleur
    asio::read(socket, asio::buffer(&myId, sizeof(myId)));

    while (window.isOpen()) {
        while (auto evt = window.pollEvent()) {
            if (evt->is<sf::Event::Closed>())
                window.close();
        }

        char cmd = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) cmd = 'W';
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) cmd = 'S';
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) cmd = 'A';
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) cmd = 'D';

        // Envoyer la commande
        asio::write(socket, asio::buffer(&cmd, 1));

        // Recevoir le nombre de joueurs
        int playerCount;
        asio::read(socket, asio::buffer(&playerCount, sizeof(playerCount)));

        players.clear();

        // Recevoir les données de tous les joueurs
        for (int i = 0; i < playerCount; i++) {
            int id;
            PlayerData data;
            asio::read(socket, asio::buffer(&id, sizeof(id)));
            asio::read(socket, asio::buffer(&data, sizeof(data)));
            players[id] = data;
        }

        // Affichage
        window.clear(sf::Color(30, 30, 30));

        for (const auto& [id, data] : players) {
            sf::RectangleShape square(sf::Vector2f(40, 40));
            square.setPosition(sf::Vector2f(data.x, data.y));
            square.setFillColor(sf::Color(data.r, data.g, data.b));

            // Ajouter un contour pour le joueur local
            if (id == myId) {
                square.setOutlineColor(sf::Color::White);
                square.setOutlineThickness(2);
            }

            window.draw(square);
        }

        window.display();
    }

    return 0;
}
