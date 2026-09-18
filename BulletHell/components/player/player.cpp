#include "player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <algorithm>
#include <cmath>
#include <random>

Player::Player(float x, float y) : sprite(get_player_texture())
{
    float scale = PLAYER_SIZE / get_player_texture().getSize().x;
    sprite.setScale({scale, scale});
    sprite.setPosition(sf::Vector2f(x, y));
    speed = MOVE_SPEED;

    collider.set_bounds(sprite.getGlobalBounds());

    if (!dash_buffer.loadFromFile("sounds/player/dash.wav"))
        printf("No dash Sound found");

    if (!shoot_buffer.loadFromFile("sounds/player/player_shoots.wav"))
        printf("No shoot Sound found");

    // Create shadow rectangle
    shadow.setSize({PLAYER_SIZE, PLAYER_SIZE});
    shadow.setPosition({sprite.getPosition().x + SHADOW_OFFSET_X, sprite.getPosition().y + SHADOW_OFFSET_Y});
    shadow.setFillColor(sf::Color(0, 0, 0, 100));  

    dash_sound.emplace(dash_buffer);
    shoot_sound.emplace(shoot_buffer);
}

void Player::spawn_hit_particles()
{
    sf::Vector2f center = sprite.getPosition() + sprite.getGlobalBounds().size / 2.f;
    constexpr int COUNT = 12;

    for (int i = 0; i < COUNT; i++)
    {
        float angle = static_cast<float>(rand()) / RAND_MAX * 2.f * 3.14159f;
        float speed = 50.f + static_cast<float>(rand()) / RAND_MAX * 100.f;

        sf::Vector2f vel { std::cos(angle) * speed, std::sin(angle) * speed};
        particles.push_back(std::make_unique<Particle>(center, vel, 0.4, sf::Color::Red));
    }
}

sf::Vector2f Player::get_input_direction()
{
    sf::Vector2f direction(0.f, 0.f);
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        direction.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        direction.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        direction.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        direction.x += 1.f;
    
    return direction;
}

void Player::handle_input(float delta, const std::vector<GameObject*>& obstacles)
{
    sf::Vector2f direction = get_input_direction();
    sf::Vector2f movement = direction * speed * delta;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && dash.cooldown_remaining <= 0.f && !dash.is_active)
    {
        dash.is_active = true;
        dash.time_remaining = DASH_DURATION;
        if (dash_sound.has_value())
            dash_sound.value().play();
    }

    move_and_collide(movement, obstacles);
}

void Player::move_and_collide(sf::Vector2f delta, const std::vector<GameObject*>& obstacles)
{
    // For x axis
    sprite.move(sf::Vector2f(delta.x, 0.f));
    collider.set_bounds(sprite.getGlobalBounds());

    for (auto& obj : obstacles)
    {
        if (collider.collides(obj->get_collider()))
        {
            auto other_bounds = obj->get_bounds();
            auto player_bounds = sprite.getGlobalBounds();

            if (delta.x > 0.f) 
                sprite.setPosition(sf::Vector2f(other_bounds.position.x - player_bounds.size.x, sprite.getPosition().y));
            else if (delta.x < 0.f)
                sprite.setPosition(sf::Vector2f(other_bounds.position.x + other_bounds.size.x, sprite.getPosition().y));

            collider.set_bounds(sprite.getGlobalBounds());
        }
    }

    // For y axis
    sprite.move(sf::Vector2f(0.f, delta.y));
    collider.set_bounds(sprite.getGlobalBounds());

    for (auto& obj : obstacles)
    {
        if (collider.collides(obj->get_collider()))
        {
            auto other_bounds = obj->get_bounds();
            auto player_bounds = sprite.getGlobalBounds();

            if (delta.y > 0.f) 
                sprite.setPosition(sf::Vector2f(sprite.getPosition().x, other_bounds.position.y - player_bounds.size.y));
            else if (delta.y < 0.f) 
                sprite.setPosition(sf::Vector2f(sprite.getPosition().x, other_bounds.position.y + other_bounds.size.y));

            collider.set_bounds(sprite.getGlobalBounds());
        }
    }
}

void Player::clamp_to_world()
{
    sf::Vector2f position = sprite.getPosition();
    sf::Vector2f size = sprite.getGlobalBounds().size;

    // Clamp X axis
    if (position.x < 0.f)
        position.x = 0.f;
    else if (position.x + size.x > WORLD_WIDTH)
        position.x = WORLD_WIDTH - size.x;

    // Clamp Y axis
    if (position.y < 0.f)
        position.y = 0.f;
    else if (position.y + size.y > WORLD_HEIGHT)
        position.y = WORLD_HEIGHT - size.y;

    sprite.setPosition(position);
    collider.set_bounds(sprite.getGlobalBounds());
}

void Player::dash_update(float delta, const std::vector<GameObject*>& obstacles)
{
    if (dash.time_remaining > 0.f)
    {
        sf::Vector2f direction = get_input_direction();
        sf::Vector2f dash_movement = direction * DASH_SPEED * delta;

        move_and_collide(dash_movement, obstacles);

        dash.time_remaining -= delta;
        if (dash.time_remaining <= 0.f)
        {
            dash.is_active = false;
            dash.cooldown_remaining = DASH_COOLDOWN;
        }
    }
}

void Player::update(float delta, const std::vector<GameObject*>& obstacles)
{
    if (dash.cooldown_remaining > 0.f)
        dash.cooldown_remaining -= delta;

    handle_input(delta, obstacles);

    if (dash.is_active)
        dash_update(delta, obstacles);

    
    if (shoot_cooldown_remaining > 0.f)
        shoot_cooldown_remaining -= delta;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && shoot_cooldown_remaining <= 0.f)
    {
        if (shoot_sound.has_value())
            shoot_sound.value().play();
        shoot(get_input_direction());
        shoot_cooldown_remaining = SHOOT_COOLDOWN;
    }
        
    for (auto& bullet : bullets)
        bullet->update(delta, obstacles);

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const auto& b) { return !b->is_active(); }),
        bullets.end()
    );

    for (auto& p : particles)
        p->update(delta);

    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const auto& p) { return !p->is_alive(); }),
        particles.end()
    );

    collider.set_bounds(sprite.getGlobalBounds());
    shadow.setPosition({sprite.getPosition().x + SHADOW_OFFSET_X, sprite.getPosition().y + SHADOW_OFFSET_Y});
    clamp_to_world();   
}

void Player::shoot(sf::Vector2f dir)
{
    if (dir.x == 0.f && dir.y == 0.f)
        return; 

    float bullet_speed = 300.f;

    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    sf::Vector2f normalized_dir = dir / length;

    sf::Vector2f player_center = sprite.getPosition() + sprite.getGlobalBounds().size / 2.f;
    float spawn_offset = sprite.getGlobalBounds().size.x / 2.f + 5.f;

    sf::Vector2f spawn_position = player_center + normalized_dir * spawn_offset;

    auto bullet = std::make_shared<Bullet>(spawn_position.x, spawn_position.y, normalized_dir, bullet_speed, BulletOwner::Player);
    bullets.push_back(bullet);
}

void Player::on_collision(GameObject& other)
{
    dash.is_active = false;
    dash.time_remaining = 0.f;
    spawn_hit_particles();
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(shadow);
    window.draw(sprite);

    for (auto& bullet : bullets)
        bullet->draw(window);

    for (auto& p : particles)
        p->draw(window);
}