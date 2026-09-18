#pragma once
#include "../objects/gameobject.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

enum class BulletOwner
{
    Player,
    Enemy
};

class Bullet : public GameObject
{
    private:
        static sf::Texture& get_bullet_texture()
        {
            static sf::Texture bullet_texture;
            static bool texture_loaded = false;
        
            if (!texture_loaded)
            {
                if (!bullet_texture.loadFromFile("sprites/bullet.png"))
                    printf("failed to load bullet texture");
                bullet_texture.setRepeated(true);
                texture_loaded = true;
            }
            return bullet_texture;
        }

    private:
        static constexpr float BULLET_SIZE = 10.f;
        static constexpr float WORLD_WIDTH = 800.f;
        static constexpr float WORLD_HEIGHT = 600.f;
    
        sf::Sprite sprite;
        sf::Vector2f velocity;
        bool alive = true;
        BulletOwner owner;

    public:
        Bullet(float x, float y, sf::Vector2f direction, float speed, BulletOwner owner = BulletOwner::Player) : sprite(get_bullet_texture())
        {
            sprite.setTexture(get_bullet_texture());

            sf::Vector2u tex_size = get_bullet_texture().getSize();
            float tex_scale = BULLET_SIZE / static_cast<float>(tex_size.x);
            sprite.setScale({tex_scale, tex_scale});
            sprite.setOrigin({tex_size.x / 2.f, tex_size.y / 2.f});
            sprite.setPosition({x, y});

            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length > 0.f)
                direction /= length;
        
            velocity = direction * speed;
            float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
            sprite.setRotation(sf::degrees(angle));

            collider.set_bounds(sprite.getGlobalBounds());
        }
    
        void update(float delta, const std::vector<GameObject*>& obstacles) override
        {
            sprite.move(velocity * delta);
            collider.set_bounds(sprite.getGlobalBounds());
        
            sf::Vector2f pos = sprite.getPosition();
            if (pos.x < 0.f || pos.x > WORLD_WIDTH || pos.y < 0.f || pos.y > WORLD_HEIGHT)
            {
                alive = false;
                return;
            }
        
            for (auto& obj : obstacles)
            {
                if (collider.collides(obj->get_collider()))
                {
                    alive = false;
                    break;
                }
            }
        }

        void draw(sf::RenderWindow& window) override
        {
            window.draw(sprite);
        }
    
        sf::FloatRect get_bounds() const override
        {
            return sprite.getGlobalBounds();
        }

        BulletOwner get_owner() const { return owner; }
        bool is_active() const { return alive; }
};


