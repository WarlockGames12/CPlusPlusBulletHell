#include "world.hpp"
#include "../player/player.hpp"
#include "../walls/wall.hpp"
#include <SFML/Graphics/Image.hpp>
#include "../enemy/moving_enemy.hpp"  

World::World(float width, float height) : width(width), height(height)
{
    if (!ground_texture.loadFromFile("sprites/background_repeat.png"))
        printf("failed to load ground texture");
    ground_texture.setRepeated(true);

    spawnPlayer();

    sf::Image map_image;
    if (!map_image.loadFromFile("sprites/level1.png"))
    {
        printf("failed to load map");
        return;
    }

    constexpr int TILE_SIZE = 25;
    for(unsigned y = 0; y < map_image.getSize().y; y += TILE_SIZE)
    {
        for(unsigned x = 0; x < map_image.getSize().x; x += TILE_SIZE)
        {
            sf::Color pixel = map_image.getPixel({x, y});
            if (pixel.r < 50 && pixel.g < 50 && pixel.b < 50)
            {
                auto wall = std::make_shared<Wall>(
                    static_cast<float>(x),
                    static_cast<float>(y),
                    TILE_SIZE,
                    TILE_SIZE
                );
                objects.push_back(wall);
            }
        }
    }

    spawnEnemies();
    spawnSpikes();
}

void World::spawnPlayer() 
{
    auto player = std::make_shared<Player>(400.f, 100.f);
    objects.push_back(player);
}

void World::spawnEnemies() 
{
    // auto enemy = std::make_shared<Enemy>(200.f, 200.f);
    // objects.push_back(enemy);
}

void World::update(float dt, const std::vector<GameObject*>& obstacles)
{
    for (auto& obj : objects)
        obj->update(dt, obstacles);
}

void World::draw(sf::RenderWindow& window) 
{
    draw_tiled_background(window);
    for (auto& obj : objects)
        obj->draw(window);
}

void World::draw_tiled_background(sf::RenderWindow& window) 
{
    constexpr float TILE_SIZE = 25.f;
    sf::Sprite tile(ground_texture);
    tile.setScale({TILE_SIZE / 32.f, TILE_SIZE / 32.f}); 
    
    for (float y = 0; y < height; y += TILE_SIZE) 
    {
        for (float x = 0; x < width; x += TILE_SIZE) 
        {
            tile.setPosition({x, y});
            window.draw(tile);
        }
    }
}

std::vector<GameObject*> World::get_obstacles()
{
    std::vector<GameObject*> result;
    for (size_t i = 1; i < objects.size(); i++)  
        result.push_back(objects[i].get());
    return result;
}
