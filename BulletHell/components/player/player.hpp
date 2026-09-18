#pragma once
#include "../objects/gameobject.hpp"
#include "../bullet/bullet.hpp"
#include "../particles/particle.hpp"
#include <SFML/Audio.hpp>
#include <optional>

class Player : public GameObject{
    private:
        static sf::Texture& get_player_texture()
        {
            static sf::Texture player_texture;
            static bool texture_loaded = false;
        
            if (!texture_loaded)
            {
                if (!player_texture.loadFromFile("sprites/player.png"))
                    printf("failed to load player texture");
                player_texture.setRepeated(true);
                texture_loaded = true;
            }
            return player_texture;
        }

    public:
        Player(float x, float y);

        void update(float delta, const std::vector<GameObject*>& obstacles) override;
        void draw(sf::RenderWindow& window) override;

        // Collision Settings:
        sf::FloatRect get_bounds() const override {
            return sprite.getGlobalBounds();
        }

        void on_collision(GameObject& other) override;

    private:
        // Player Settings:
        static constexpr float PLAYER_SIZE = 30.f;
        static constexpr float MOVE_SPEED = 100.f;
        static constexpr float DASH_SPEED = 200.f;
        static constexpr float DASH_DURATION = 0.2f;
        static constexpr float DASH_COOLDOWN = 1.f;
        static constexpr float WORLD_WIDTH = 800.f;
        static constexpr float WORLD_HEIGHT = 600.f;
        static constexpr float SHADOW_OFFSET_X = 2.f;
        static constexpr float SHADOW_OFFSET_Y = 2.f;
        static constexpr float SHOOT_COOLDOWN = 0.2f;

        // Player State
        sf::Sprite sprite;
        float speed = MOVE_SPEED;
        float shoot_cooldown_remaining = 0.f;

        // Particle Settings:
        std::vector<std::unique_ptr<Particle>> particles;
        void spawn_hit_particles();

        // Dash State
        struct DashState 
        {
            bool is_active = false;
            float time_remaining = 0.f;
            float cooldown_remaining = 0.f;
        } dash;

        // Health Settings:
        float player_health = 100.f;

        // Audio Settings:
        sf::SoundBuffer dash_buffer; 
        std::optional<sf::Sound> dash_sound;

        sf::SoundBuffer shoot_buffer; 
        std::optional<sf::Sound> shoot_sound;

        // Shadow Settings: 
        sf::RectangleShape shadow;

        // Shoot Settings:
        std::vector<std::shared_ptr<Bullet>> bullets;
        void shoot(sf::Vector2f dir);

        // Helper Methods
        void handle_input(float delta, const std::vector<GameObject*>& obstacles);
        void dash_update(float delta, const std::vector<GameObject*>& obstacles);
        void move_and_collide(sf::Vector2f delta, const std::vector<GameObject*>& obstacles);
        void clamp_to_world();
        sf::Vector2f get_input_direction();
};