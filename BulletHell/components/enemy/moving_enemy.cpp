#pragma once
#include "../objects/gameobject.hpp"
#include "moving_enemy.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>

static sf::Texture& get_enemy_texture()
{
    static sf::Texture enemy_tex;
    static bool loaded = false;

    if (!loaded)
    {
        if (!enemy_tex.loadFromFile("sprites/moving_enemy.png"))
            printf("Failed to load enemy texture\n");
        loaded = true;
    }
    return enemy_tex;
}


MovingEnemy::MovingEnemy(sf::Vector2f pos, Player& player) : player(player), sprite(get_enemy_texture())
{
    float scale = ENEMY_SIZE / get_enemy_texture().getSize().x;
    sprite.setScale({scale, scale});
    sprite.setPosition(pos);
    collider.set_bounds(sprite.getGlobalBounds());
}

void MovingEnemy::update(float delta, const std::vector<GameObject*>& obstacles)
{
    move_towards_player(delta, obstacles);
    collider.set_bounds(sprite.getGlobalBounds());
}

void MovingEnemy::move_towards_player(float delta, const std::vector<GameObject*>& obstacles)
{
    sf::Vector2f enemy_pos = sprite.getPosition();
    sf::Vector2f player_pos = player.get_bounds().position + player.get_bounds().size / 2.f;
    sf::Vector2f dir = player_pos - enemy_pos;

    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length != 0.f)
        dir /= length;

    sf::Vector2f movement = dir * ENEMY_SPEED * delta;


    // Enemy Collider with Obstacles
    sprite.move({movement.x, 0.f});
    collider.set_bounds(sprite.getGlobalBounds());
    for (auto& obj : obstacles)
    {
        if (collider.collides(obj->get_collider()))
            sprite.move({-movement.x, 0.f});
    }

    sprite.move({0.f, movement.y});
    collider.set_bounds(sprite.getGlobalBounds());
    for (auto& obj : obstacles)
    {
        if (collider.collides(obj->get_collider()))
            sprite.move({0.f, -movement.y});
    }
}

void MovingEnemy::on_collision(GameObject& other)
{
    Bullet* bullet = dynamic_cast<Bullet*>(&other);
    if (!bullet)
        return;
    
    if (bullet->get_owner() == BulletOwner::Enemy)
        return;
    
    enemy_health -= 25.f;
    bullet->on_collision(*this);

    if (enemy_health <= 0.f)
        alive = false;
}

void MovingEnemy::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
