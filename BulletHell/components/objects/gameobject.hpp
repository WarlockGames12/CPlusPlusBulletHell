#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <SFML/Graphics.hpp>
#include "collider.hpp"

class GameObject
{
    public: 
        virtual ~GameObject() = default;
        virtual void update(float delta, const std::vector<GameObject*>& obstacles) = 0;
        virtual void draw(sf::RenderWindow& window) = 0;
        virtual sf::FloatRect get_bounds() const = 0;

        // Collider Access
        Collider& get_collider(){ return collider; }
        const Collider& get_collision() const { return collider; }

        virtual void on_collision(GameObject& other){}

    protected: 
        Collider collider;
};

#endif