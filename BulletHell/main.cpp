#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include "components/world/world.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Bullet Hell");

    sf::Clock clock;
    World world;
    
    while(window.isOpen())
    {
        std::optional<sf::Event> maybeEvent;
        while ((maybeEvent = window.pollEvent()))
        {
            sf::Event& event = *maybeEvent;

            if (event.is<sf::Event::Closed>())
                window.close();
        }

        std::vector<GameObject*> obstacles = world.get_obstacles();
        float delta = clock.restart().asSeconds();
        world.update(delta, obstacles);

        window.clear(sf::Color::Black);
        world.draw(window);
        window.display();
    }
    return 0;
}