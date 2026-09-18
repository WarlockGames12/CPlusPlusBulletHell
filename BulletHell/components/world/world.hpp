#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "../objects/gameobject.hpp"

class World 
{
    public:
        World(float width = 800.f, float height = 600.f);    
        void update(float dt, const std::vector<GameObject*>& obstacles);
        void draw(sf::RenderWindow& window);
        std::vector<GameObject*> get_obstacles();

        sf::FloatRect getBounds() const 
        {
            return sf::FloatRect{sf::Vector2f(0.f, 0.f), sf::Vector2f(width, height)};
        }
        

    private:
        float width, height;
        std::vector<std::shared_ptr<GameObject>> objects;
        sf::Texture ground_texture;

        void spawnPlayer();
        void spawnEnemies();
        void spawnSpikes();
        void draw_tiled_background(sf::RenderWindow& window);
};