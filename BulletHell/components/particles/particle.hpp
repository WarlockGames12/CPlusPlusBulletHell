#pragma once
#include <SFML/Graphics.hpp>

class Particle{
    public: 
        Particle(sf::Vector2f pos, sf::Vector2f vel, float life_time, sf::Color color);
        void update(float delta);
        void draw(sf::RenderWindow& window);
        bool is_alive() const;
    private:
        sf::Vector2f velocity;
        float lifeTime;
        float max_life_time;

        sf::Color color;
        sf::RectangleShape shape;
};