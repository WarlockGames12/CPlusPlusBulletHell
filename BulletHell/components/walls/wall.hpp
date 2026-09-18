#pragma once
#include "../objects/gameobject.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Wall : public GameObject
{
    private:
        static sf::Texture& get_wall_texture()
        {
            static sf::Texture wall_texture;
            static bool texture_loaded = false;
        
            if (!texture_loaded)
            {
                if (!wall_texture.loadFromFile("sprites/background_repeat_wall.png"))
                    printf("failed to load wall texture");
                wall_texture.setRepeated(true);
                texture_loaded = true;
            }
            return wall_texture;
        }

        static constexpr float SHADOW_OFFSET_X = 5.f;
        static constexpr float SHADOW_OFFSET_Y = 5.f;

    public:
        Wall(float x, float y, float w, float h) : sprite(get_wall_texture())
        {
            float scale_x = w / get_wall_texture().getSize().x;
            float scale_y = h / get_wall_texture().getSize().y;
            
            sprite.setPosition({x, y});
            sprite.setScale({scale_x, scale_y});
            
            // Create shadow rectangle
            shadow.setSize({w, h});
            shadow.setPosition({x + SHADOW_OFFSET_X, y + SHADOW_OFFSET_Y});
            shadow.setFillColor(sf::Color(0, 0, 0, 100));  
            
            collider.set_bounds(sf::FloatRect({x, y}, {w, h}));
        }

        void update(float, const std::vector<GameObject*>&) override
        {
            // walls don't move
        }

        void draw(sf::RenderWindow& window) override
        {
            window.draw(shadow);   
            window.draw(sprite);   
        }   

        sf::FloatRect get_bounds() const override
        {
            return sprite.getGlobalBounds();
        }

    private: 
        sf::Sprite sprite;
        sf::RectangleShape shadow;
};