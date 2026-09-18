#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include <optional>
#include <SFML/Graphics.hpp>

class Collider{
    public:
        Collider() = default;
        explicit Collider(const sf::FloatRect& bounds) : bounds(bounds) {}

        void set_bounds(const sf::FloatRect& new_bounds)
        {
            bounds = new_bounds;
        }

        const sf::FloatRect& getBounds() const
        {
            return bounds;
        }

        bool collides(const Collider& other) const 
        {
            return bounds.findIntersection(other.bounds).has_value();
        }

    private: 
        sf::FloatRect bounds;
};

#endif