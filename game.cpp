#include "game.h"
#include "texture_manager.h"
#include "map.h"
#include "ecs/components.h"
#include "vector_2d.h"
#include "ecs/collision.h"

// Map object declaration.
Map *map;

// Renderer declaration.
SDL_Renderer *Game::renderer = nullptr;

// Game event declaration.
SDL_Event Game::event;

// Colliders declaration.
std::vector<ColliderComponent *> Game::colliders;

// Game status indicators.
bool is_tag_running = true;
int game_time = 0;

// Entity component system manager declaration.
Manager manager;

// Group labels declaration.
enum GroupLabel : std::size_t {
    GROUP_MAP,
    GROUP_PLAYERS,
    GROUP_ENEMIES,
    GROUP_COLLIDERS,
    GROUP_STATS_OBJECT
};

// Players declaration.
auto &player(manager.add_entity());
auto &players(manager.get_group(GROUP_PLAYERS));

// Enemies declaration.
auto &enemy(manager.add_entity());
auto &enemies(manager.get_group(GROUP_ENEMIES));

// Player status objects declaration.
auto &heart1(manager.add_entity());
auto &heart2(manager.add_entity());
auto &heart3(manager.add_entity());
auto &game_over(manager.add_entity());
auto &stats(manager.get_group(GROUP_STATS_OBJECT));

// Map objects declaration.
auto &east_wall(manager.add_entity());
auto &west_wall(manager.add_entity());
auto &north_wall(manager.add_entity());
auto &south_wall(manager.add_entity());
auto &tiles(manager.get_group(GROUP_MAP));

// Center point of an entity.
Vector2D center_of(Entity &entity) {
    TransformComponent &transform = entity.get_component<TransformComponent>();
    return Vector2D(
        transform.position.x + transform.width * transform.scale / 2.0f,
        transform.position.y + transform.height * transform.scale / 2.0f
    );
}

// Vector from the center of an entity to the center of the player.
Vector2D direction_to_player(Entity &entity) {
    Vector2D from = center_of(entity);
    Vector2D to = center_of(player);
    return Vector2D(to.x - from.x, to.y - from.y);
}

// Check if an entity is completely outside the screen.
bool is_off_screen(Entity &entity) {
    TransformComponent &transform = entity.get_component<TransformComponent>();
    float width = transform.width * transform.scale;
    float height = transform.height * transform.scale;
    return transform.position.x + width < 0 || transform.position.x > SCREEN_WIDTH ||
           transform.position.y + height < 0 || transform.position.y > SCREEN_HEIGHT;
}

Game::Game() {}

// Initialize the elements of the game, such as the game window and the renderer.
void Game::initialize(const char *title, int x_pos, int y_pos, int width, int height, bool full_screen) {
    // FULLSCREEN_DESKTOP keeps the desktop resolution. The renderer scales the
    // logical size to it (see below), so the game fills the screen.
    Uint32 flags = 0;
    if (full_screen) flags = SDL_WINDOW_FULLSCREEN_DESKTOP;

    // If SDL cannot be initialized, stop.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        this->is_running = false;
        return;
    }
    std::cout << "Subsystem initialized!" << std::endl;

    // Create the game window.
    this->window = SDL_CreateWindow(title, x_pos, y_pos, width, height, flags);
    // Set the window icon.
    SDL_Surface *icon = SDL_LoadBMP("assets/player.bmp");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);
    if (window) std::cout << "Window created!" << std::endl;

    // Create the game renderer.
    this->renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer) {
        // Draw at a fixed logical size. SDL scales it to the window size,
        // keeps the aspect ratio, and centers it with black bars.
        SDL_RenderSetLogicalSize(renderer, width, height);
        // Draw each frame on a texture of the logical size, then scale the texture
        // to the window as one image. Scaling each tile on its own leaves thin gaps
        // between the tiles when the scale is not a whole number.
        this->frame = SDL_CreateTexture(
            renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height
        );
        std::cout << "Renderer created!" << std::endl;
    }

    // Generate the game map.
    Map::load_map("assets/map.txt", 25, 20);

    // Add the player to the ECS manager.
    player.add_component<TransformComponent>(384, 526, 43, 43, 1);
    player.add_component<SpriteComponent>("assets/player.bmp");
    player.add_component<KeyboardController>();
    player.add_component<ColliderComponent>("Player");
    player.add_component<StatsComponent>(3);
    player.add_group(GROUP_PLAYERS);

    // Add the health points to the ECS manager.
    heart1.add_component<TransformComponent>(830, 54);
    heart1.add_component<SpriteComponent>("assets/health_point.bmp");
    heart1.add_group(GROUP_STATS_OBJECT);
    heart2.add_component<TransformComponent>(830, 108);
    heart2.add_component<SpriteComponent>("assets/health_point.bmp");
    heart2.add_group(GROUP_STATS_OBJECT);
    heart3.add_component<TransformComponent>(830, 162);
    heart3.add_component<SpriteComponent>("assets/health_point.bmp");
    heart3.add_group(GROUP_STATS_OBJECT);

    // Add the game-over banner to the ECS manager.
    game_over.add_component<TransformComponent>(135, 250, 106, 25, 5);
    game_over.add_component<SpriteComponent>();
    game_over.add_group(GROUP_STATS_OBJECT);

    // Add the enemy to the ECS manager.
    enemy.add_component<TransformComponent>(384, 15, 46, 45, 2);
    enemy.add_component<SpriteComponent>("assets/enemy.bmp");
    enemy.add_component<ColliderComponent>("Enemy");
    enemy.add_component<StatsComponent>(10);
    enemy.add_group(GROUP_ENEMIES);

    // Add the walls to the ECS manager.
    east_wall.add_component<TransformComponent>(790, 0, 10, 640, 1);
    east_wall.add_component<SpriteComponent>("assets/vertical_wall.bmp");
    east_wall.add_component<ColliderComponent>("eWall");
    east_wall.add_group(GROUP_MAP);
    west_wall.add_component<TransformComponent>(0, 0, 10, 640, 1);
    west_wall.add_component<SpriteComponent>("assets/vertical_wall.bmp");
    west_wall.add_component<ColliderComponent>("wWall");
    west_wall.add_group(GROUP_MAP);
    north_wall.add_component<TransformComponent>(0, 0, 800, 10, 1);
    north_wall.add_component<SpriteComponent>("assets/horizontal_wall.bmp");
    north_wall.add_component<ColliderComponent>("nWall");
    north_wall.add_group(GROUP_MAP);
    south_wall.add_component<TransformComponent>(0, 630, 800, 10, 1);
    south_wall.add_component<SpriteComponent>("assets/horizontal_wall.bmp");
    south_wall.add_component<ColliderComponent>("sWall");
    south_wall.add_group(GROUP_MAP);

    this->is_running = true;
}

// Handle all the events that happened since the last frame.
void Game::handle_events() {
    while (SDL_PollEvent(&event)) {
        // Operate the given event depending on its type.
        switch (event.type) {
            case SDL_QUIT:
                this->is_running = false;
                break;
            case SDL_KEYDOWN:
                // F11 switches between window and fullscreen.
                if (event.key.keysym.scancode == SDL_SCANCODE_F11 && event.key.repeat == 0) {
                    toggle_full_screen();
                }
                break;
            default:
                break;
        }
        // Send the event to the player's keyboard controller.
        // After the game ends, the player entity no longer exists.
        if (is_tag_running) {
            player.get_component<KeyboardController>().handle_event(event);
        }
    }
}

// Switch between window and fullscreen. The logical size keeps the scaling.
void Game::toggle_full_screen() {
    bool is_full_screen = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN;
    SDL_SetWindowFullscreen(window, is_full_screen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
}

// Update the elements of the game entities.
void Game::update() {
    // Update the status of the game objects.
    manager.refresh();
    manager.update();
    // Stop if the game has ended.
    if (!is_tag_running) return;

    // Timer, recorded second by second.
    if (SDL_GetTicks() > static_cast<Uint32>(1000 * game_time)) {
        game_time++;
        // Add a cooldown to ensure that no skill fires twice in the same second.
        enemy.get_component<StatsComponent>().skill_cooldown = true;
        // Deduct the invincible time.
        if (player.get_component<StatsComponent>().invincible_time != 0) {
            player.get_component<StatsComponent>().invincible_time--;
        } else {
            player.get_component<SpriteComponent>().set_texture("assets/player.bmp");
        }
        // Victory after 50 seconds.
        if (game_time == 50) {
            // Show the victory banner.
            game_over.get_component<SpriteComponent>().set_texture("assets/victory.bmp");
            // Destroy all moving objects.
            player.destroy();
            for (auto &e : enemies) {
                e->destroy();
            }
            is_tag_running = false;
        }
    }

    if (enemy.get_component<StatsComponent>().skill_cooldown == true) {
        // Every 30 seconds, add a tracking bullet to the field.
        if (game_time % 30 == 0) {
            // Set the enemy in cooldown.
            enemy.get_component<StatsComponent>().skill_cooldown = false;
            auto &bullet(manager.add_entity());
            // Bullets are generated at the enemy's position.
            bullet.add_component<TransformComponent>(
                enemy.get_component<TransformComponent>().position.x,
                enemy.get_component<TransformComponent>().position.y,
                24, 22, 2
            );
            bullet.add_component<SpriteComponent>("assets/tracking_bullet.bmp");
            bullet.add_component<ColliderComponent>("TrackingEnemySummon");
            bullet.add_component<StatsComponent>(5);
            bullet.add_group(GROUP_ENEMIES);
            // Set the initial speed and direction.
            bullet.get_component<TransformComponent>().velocity.x = 1;
            bullet.get_component<TransformComponent>().velocity.y = 1;
            bullet.get_component<TransformComponent>().speed = 1.5;
        }
        // Every 15 seconds, add a rebounding bullet to the field.
        if (game_time % 15 == 0) {
            // Set the enemy in cooldown.
            enemy.get_component<StatsComponent>().skill_cooldown = false;
            auto &bullet(manager.add_entity());
            // Bullets are generated at the enemy's position.
            bullet.add_component<TransformComponent>(
                enemy.get_component<TransformComponent>().position.x,
                enemy.get_component<TransformComponent>().position.y,
                64, 64, 1
            );
            bullet.add_component<SpriteComponent>("assets/rebound_bullet.bmp");
            bullet.add_component<ColliderComponent>("ReboundEnemySummon");
            bullet.add_component<StatsComponent>(5);
            bullet.add_group(GROUP_ENEMIES);
            // Build a vector pointing from the bullet to the player.
            Vector2D direction = direction_to_player(bullet);
            // Convert the vector into a unit vector.
            direction.unit_vector();
            // Set the initial speed and direction.
            bullet.get_component<TransformComponent>().velocity.x = direction.x;
            bullet.get_component<TransformComponent>().velocity.y = direction.y;
            bullet.get_component<TransformComponent>().speed = 5;
        }
        // Every 3 seconds, add an arc-shaped bullet line to the field.
        if (game_time % 3 == 0) {
            // Set the enemy in cooldown.
            enemy.get_component<StatsComponent>().skill_cooldown = false;
            // Add new bullets.
            for (int i = 0; i < game_time / 2; i++) {
                auto &bullet(manager.add_entity());
                // Bullets are generated at the enemy's position.
                bullet.add_component<TransformComponent>(
                    enemy.get_component<TransformComponent>().position.x,
                    enemy.get_component<TransformComponent>().position.y,
                    22, 22, 2
                );
                bullet.add_component<SpriteComponent>("assets/spread_bullet.bmp");
                bullet.add_component<ColliderComponent>("SpreadEnemySummon");
                bullet.add_component<StatsComponent>(5);
                bullet.add_group(GROUP_ENEMIES);
                // Calculate the initial direction of the bullets.
                Vector2D direction(0, 0);
                int bullet_count = 0;
                if ((game_time / 2) % 2 == 1) {
                    bullet_count = (game_time - 1) / 2;
                    direction.x = 125 * (i - (bullet_count - 1) / 2);
                } else {
                    bullet_count = game_time / 2;
                    direction.x = 125 * (i - bullet_count / 2);
                }
                direction.y = 125 * bullet_count;
                // Convert the vector into a unit vector.
                direction.unit_vector();
                // Set the initial speed and direction.
                bullet.get_component<TransformComponent>().velocity.x = direction.x;
                bullet.get_component<TransformComponent>().velocity.y = direction.y;
                bullet.get_component<TransformComponent>().speed = 3;
            }
        }
    }

    // In every game loop, adjust the tracking bullets' direction based on the player's position.
    for (auto &e : enemies) {
        if (e->get_component<ColliderComponent>().tag == "TrackingEnemySummon") {
            // Build a vector pointing from the bullet to the player.
            Vector2D direction = direction_to_player(*e);
            // Convert the vector into a unit vector.
            direction.unit_vector();
            e->get_component<TransformComponent>().velocity.x = direction.x;
            e->get_component<TransformComponent>().velocity.y = direction.y;
        }
    }

    // Destroy the bullets that have left the screen.
    // refresh() removes them at the start of the next frame.
    for (auto &e : enemies) {
        if (e != &enemy && is_off_screen(*e)) e->destroy();
    }

    // Collision evaluation.
    for (auto cc : colliders) {
        // Player collision.
        if (Collision::aabb(player.get_component<ColliderComponent>(), *cc)) {
            // The player hits a vertical wall.
            if (cc->tag == "eWall") {
                player.get_component<TransformComponent>().position.x -=
                    player.get_component<TransformComponent>().speed;
            } else if (cc->tag == "wWall") {
                player.get_component<TransformComponent>().position.x +=
                    player.get_component<TransformComponent>().speed;
            }
            // The player hits a horizontal wall.
            if (cc->tag == "nWall") {
                player.get_component<TransformComponent>().position.y +=
                    player.get_component<TransformComponent>().speed;
            } else if (cc->tag == "sWall") {
                player.get_component<TransformComponent>().position.y -=
                    player.get_component<TransformComponent>().speed;
            }
            // The player hits an enemy when not invincible.
            if (
                (cc->tag == "Enemy" || cc->tag == "SpreadEnemySummon" ||
                 cc->tag == "TrackingEnemySummon" || cc->tag == "ReboundEnemySummon") &&
                player.get_component<StatsComponent>().invincible_time == 0
            ) {
                // The player's health point is damaged.
                player.get_component<StatsComponent>().health_point--;
                player.get_component<StatsComponent>().invincible_time = 2;
                player.get_component<SpriteComponent>().set_texture("assets/damaged_player.bmp");
                // Switch the texture.
                switch (player.get_component<StatsComponent>().health_point) {
                    case 2:
                        heart1.get_component<SpriteComponent>().set_texture("assets/damaged_health_point.bmp");
                        break;
                    case 1:
                        heart2.get_component<SpriteComponent>().set_texture("assets/damaged_health_point.bmp");
                        break;
                    case 0:
                        // When the player's health points are used up, the game is over.
                        heart3.get_component<SpriteComponent>().set_texture("assets/damaged_health_point.bmp");
                        game_over.get_component<SpriteComponent>().set_texture("assets/game_over.bmp");
                        // Destroy all moving game objects.
                        player.destroy();
                        for (auto &e : enemies) {
                            e->destroy();
                        }
                        is_tag_running = false;
                        break;
                }
            }
        }
        // Enemy collision.
        for (auto &e : enemies) {
            if (Collision::aabb(e->get_component<ColliderComponent>(), *cc)) {
                if (
                    (cc->tag == "eWall" || cc->tag == "wWall") &&
                    e->get_component<ColliderComponent>().tag == "ReboundEnemySummon"
                ) {
                    e->get_component<TransformComponent>().velocity.x *= -1;
                }
                if (
                    (cc->tag == "nWall" || cc->tag == "sWall") &&
                    e->get_component<ColliderComponent>().tag == "ReboundEnemySummon"
                ) {
                    e->get_component<TransformComponent>().velocity.y *= -1;
                }
            }
        }
    }
}

// Update the game screen in accordance with the game entities.
void Game::render() {
    if (frame) {
        // Draw on the frame texture. White is the background of the health points.
        SDL_SetRenderTarget(renderer, frame);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
    } else {
        // Without a frame texture, draw on the window directly:
        // black bars around the logical area, and white inside it.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, nullptr);
    }
    for (auto &t : tiles) {
        t->draw();
    }
    for (auto &p : players) {
        p->draw();
    }
    for (auto &e : enemies) {
        e->draw();
    }
    for (auto &s : stats) {
        s->draw();
    }
    if (frame) {
        // Copy the frame to the window. The logical size scales it and adds black bars.
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, frame, nullptr, nullptr);
    }
    SDL_RenderPresent(renderer);
}

// Clean the whole game when it is closed.
// 1. Remove all entities, so that their textures are freed while the renderer exists.
// 2. Destroy the renderer before its window. SDL_DestroyWindow also frees the
//    window's renderer, so the reverse order uses a freed pointer and crashes.
void Game::clean() {
    manager.clear();
    SDL_DestroyTexture(this->frame);
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
    std::cout << "Game cleaned!" << std::endl;
}

// Check if the game keeps running in the game loop.
bool Game::running() {
    return this->is_running;
}

// Add a map tile.
void Game::add_tile(int id, int x, int y) {
    auto &tile(manager.add_entity());
    tile.add_component<TileComponent>(x, y, 32, 32, id);
    tile.add_group(GROUP_MAP);
}
