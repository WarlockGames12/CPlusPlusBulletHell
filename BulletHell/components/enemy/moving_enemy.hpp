#pragma once
#include "../objects/gameobject.hpp"
#include "../player/player.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>

class MovingEnemy : public GameObject
{
    public: 
        MovingEnemy(sf::Vector2f pos, Player& player);

        void update(float delta, const std::vector<GameObject*>& obstacles) override;
        void draw(sf::RenderWindow& window) override;
        void on_collision(GameObject& other) override;
        sf::FloatRect get_bounds() const override{
            return sprite.getGlobalBounds();
        };
        bool is_alive() const {return alive;}

    private: 
        void move_towards_player(float delta, const std::vector<GameObject*>& obstacles);
    private:
        Player& player;
        sf::Sprite sprite;
        sf::Vector2f velocity;
        float enemy_health = 100.f;
        bool alive = true;

        static constexpr float ENEMY_SIZE = 30.f;
        static constexpr float ENEMY_SPEED = 80.f;
};