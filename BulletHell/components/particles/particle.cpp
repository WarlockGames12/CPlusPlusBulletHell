#include "particle.hpp"
#include <algorithm>
#include <SFML/System.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <cstdint>

Particle::Particle(sf::Vector2f pos, sf::Vector2f vel, float life_time, sf::Color color) : velocity(vel), lifeTime(life_time), max_life_time(max_life_time)
{
    shape.setSize({4.f, 4.f});
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setPosition(pos);
    shape.setFillColor(color);
}

void Particle::update(float delta){
    lifeTime -= delta;
    shape.move(velocity * delta);

    float life_ratio = lifeTime / max_life_time;
    sf::Color current_color = shape.getFillColor();
    current_color.a = static_cast<std::uint8_t>(255.f * std::max(life_ratio, 0.f));
    shape.setFillColor(current_color);
}

void Particle::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

bool Particle::is_alive() const
{
    return lifeTime > 0.f;
}