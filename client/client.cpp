#include <SFML/Graphics.hpp>
#include <asio.hpp>

using asio::ip::tcp;

int main() {
    asio::io_context io;
    tcp::socket socket(io);
    socket.connect(tcp::endpoint(asio::ip::make_address("54.37.252.158"), 4000));

    sf::RenderWindow window(sf::VideoMode({600, 600}), "Carré multijoueur");
    sf::RectangleShape square(sf::Vector2f(40, 40));
    square.setFillColor(sf::Color::Red);

    float x = 0, y = 0;

    while (window.isOpen()) {
        while (auto evt = window.pollEvent()) {
            if (evt->is<sf::Event::Closed>())
                window.close();
        }

        char cmd = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) cmd = 'W';
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) cmd = 'S';
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) cmd = 'A';
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) cmd = 'D';

        if (cmd != 0) {
            asio::write(socket, asio::buffer(&cmd, 1));

            float data[2];
            asio::read(socket, asio::buffer(data, sizeof(data)));

            x = data[0];
            y = data[1];
        }

        square.setPosition({x, y});

        window.clear();
        window.draw(square);
        window.display();
    }

    return 0;
}
