#include <iostream>  // For input and output operations
#include <vector>    // To use the vector container to store game objects
#include <cstdlib>   // For random number generation
#include <cmaths>     // For mathematical functions like cos, sin
#include <ctime>     // For time-related functions

// Constants for screen size and mathematical constant PI
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600
const float PI = 3.14159;

// Structure to represent a 2D vector (x, y)
struct Vector2 {
    float x, y;

    // Overloaded operator for adding two vectors
    Vector2 operator+(const Vector2& other) const {
        return {x + other.x, y + other.y};
    }

    // Overloaded operator for subtracting two vectors
    Vector2 operator-(const Vector2& other) const {
        return {x - other.x, y - other.y;
    }

    // Overloaded operator for multiplying a vector by a scalar
    Vector2 operator*(float scalar) const {
        return {x * scalar, y * scalar};
    }

    // Overloaded operator for adding another vector to the current vector
    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
};

// Structure to represent a basic game object with position, velocity, and activity status
struct GameObject {
    Vector2 position;  // Position of the object
    Vector2 velocity;  // Velocity of the object
    bool active;       // Whether the object is active (visible, interacting, etc.)

    // Constructor to initialize the object with default values
    GameObject() : position{0, 0}, velocity{0, 0}, active(false) {}
};

// Ship structure, which inherits from GameObject, and adds additional attributes
struct Ship : public GameObject {
    float angle;  // Angle of the ship (for rotation)

    // Constructor to initialize the ship with a default angle
    Ship() : angle(0) {}

    // Function to rotate the ship by a given angle delta
    void rotate(float delta) {
        angle += delta;
        if (angle >= 360.0f) angle -= 360.0f
        if (angle < 0.0f) angle += 360.0f;


    // Function to apply thrust to the ship, affecting its velocity
    void thrust(float force) {
        // Convert the angle from degrees to radians
        float radianAngle = angle * PI / 180.0f;
        velocity.x += force * cos(radianAngle);
        velocity.y += force * sin(radianAngle);
    }

    // Function to perform a hyperspace jump, which moves the ship to a random position
    void hyperspace() {
        position.x = rand() % SCREEN_WIDTH;
        position.y = rand() % SCREEN_HEIGHT;
        // 20% chance of the ship being deactivated after hyperspace jump
        if (rand() % 10 < 2) active = false;
    }
};

// Asteroid structure, which also inherits from GameObject and includes an additional size attribute
struct Asteroid : public GameObject {
    int size;  // Size of the asteroid

    // Constructor to initialize the asteroid with a default size
    Asteroid() : size(3) {}
};

// Saucer structure, also inheriting from GameObject, with a small flag to indicate if it's a small saucer
struct Saucer : GameObject {
    bool small;  // Flag to indicate if the saucer is small

    // Constructor to initialize the saucer with default values
    Saucer() : small(false) {}
};

// PowerUp structure with different types of power-ups (SpeedBoost, RapidFire, Shield)
struct PowerUp : public GameObject {
    enum Type { SpeedBoost, RapidFire, Shield } type;  // Enum to define the type of power-up

    // Constructor to initialize a power-up with a specific type
    PowerUp(Type t) : type(t) {}
};

// Main Game class to manage all game logic and entities
class Game {
private:
    Ship player;  // Player's ship object
    std::vector<Asteroid> asteroids;  // List of asteroids in the game
    std::vector<GameObject> bullets;  // List of bullets fired by the player
    Saucer saucer;  // Saucer object
    std::vector<PowerUp> powerUps;  // List of power-ups in the game
    int score;  // Player's score
    int lives;  // Player's remaining lives
    bool rapidFireActive;  // Whether rapid fire is active
    bool shieldActive;  // Whether shield is active
    float rapidFireTimer;  // Timer for rapid fire
    float shieldTimer;  // Timer for shield

public:
    // Constructor to initialize the game state and seed the random number generator
    Game() : score(0), lives(3), rapidFireActive(false), shieldActive(false), rapidFireTimer(0), shieldTimer(0) {
        srand(time(0));  // Initialize random number generator
        initialize();  // Initialize game objects
    }

    // Function to initialize the game state, including player and objects
    void initialize() {
        player = Shi();  // Initialize the player ship
        player.active = true;
        player.position = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};  // Start in the center of the screen

        asteroids.clear();
        // Create 5 initial asteroids
        for (int i = 0; i < 5; ++i) {
            Asteroid asteroid;
            asteroid.active = true;
            asteroid.position = {float(rand() % SCREEN_WIDTH), float(rand() % SCREEN_HEIGHT)};
            asteroid.velocity = {float(rand() % 3 - 1), float(rand() % 3 - 1)};
            asteroid.size = 3;
            asteroids.push_back(asteroid);
        }

        bullets.clear();
        powerUps.clear();
        saucer.active = false;  // Saucer is inactive initially
    }

    // Main game loop function
    void run() {
        while (true) {
            handleInput();  // Handle user input
            update();  // Update game objects and state
            render();  // Render the game state to the screen
        }
    }

    // Function to handle user input for controlling the ship
    void handleInput() {
        char input;
        std::cout << "Enter command (A/D/W/S/Space/Q): ";  // Display command prompt
        std::cin >> input;  // Get user input

        switch (input) {
            case 'A':
            case 'a':
                player.rotate(-5.0f);  // Rotate left
                break;
            case 'D':
            case 'd':
                player.rotate(5.0f);  // Rotate right
                break;
            case 'W':
            case 'w':
                player.thrust(0.2f);  // Thrust forward
                break;
            case 'S':
            case 's':
                player.hyperspace();  // Perform hyperspace jump
                break;
            case ' ':
                fireBullet();  // Fire a bullet
                break;
            case 'Q':
            case 'q':
                std::cout << "Game Over! Final Score: " << score << std::endl;  // End game
                exit(0);
        }
    }

    // Function to update the game state (positions, actions, etc.)
    void update() {
        if (!player.active) {
            std::cout << "Game Over! Final Score: " << score << std::endl;
            exit(0);  // End game if the player is inactive
        }

        // Update player position based on velocity
        player.position += player.velocity;
        wrapAround(player.position);  // Wrap around screen edges

        // Update bullet positions and wrap around if necessary
        for (auto& bullet : bullets) {
            if (bullet.active) {
                bullet.position += bullet.velocity;
                wrapAround(bullet.position);
            }
        }

        // Update asteroid positions and wrap around if necessary
        for (auto& asteroid : asteroids) {
            if (asteroid.active) {
                asteroid.position += asteroid.velocity;
                wrapAround(asteroid.position);
            }
        }

        // Update saucer position if it's active
        if (saucer.active) {
            saucer.position += saucer.velocity;
            wrapAround(saucer.position1);
        } else if (rand() % 1000 < 5) {  // Random chance to spawn saucer
            saucer.active = true;
            saucer.position = {0, float(rand() % SCREEN_HEIGHT)};
            saucer.velocity = {1.0f, 0};
            saucer.small = (rand() % 2 == 0);  // Randomly make saucer small or large
        }

        spawnPowerUps();  // Check if new power-ups should spawn
        updatePowerUps();  // Update active power-ups
        checkCollisions();  // Check for any collisions
    }

    // Function to render the current game state to the screen
    void render() {
        system("clear");  // Clear the screen for a new frame
        std::cout << "Score: " << score << " | Lives:  << lives << std::endl;

        if (player.active) {
            std::cout << "Player at (" << player.position.x << ", " << player.position.y << ")\n";
        }

        // Render the asteroids
        for (const auto& asteroid : asteroids) {
            if (asteroid.active) {
                std::cout << "Asteroid at (" << asteroid.position.x << ", " << asteroid.position.y << ")\n";
            }
        }

        // Render the bullets
        for (const auto& bullet : bullets) {
            if (bullet.active) {
                std::cout << "Bullet at (" << bullet.position.x << ", " << bullet.position.y << ")\n";
            }
        }

        // Render the saucer
        if (saucer.active) {
            std::cout << "Saucer at (" << saucer.position.x << ", " << saucer.position.y << ")\n";
        }

        // Render the power-ups
        for (const auto& powerUp : powerUps) {
            if (powerUp.active) {
                std::cout << "PowerUp at (" << powerUp.position.x << ", " << powerUp.position.y << ")\n";
            }
        }
    }

    // Function to fire a bullet from the ship
    void fireBullet() {
        // Prevent firing if the previous bullet isn't finished or the rapid fire isn't active
        if (rapidFireActive || bullets.empty() || bullets.back().velocity.x != 0 || bullets.back().velocity.y != 0) {
            GameObject bullet;
            bullet.active = true;
            bullet.position = player.position;
            float radianAngle = player.angle * PI / 180.0f;
            bullet.velocity = {static_cast<float0>(cos(radianAngle)) * 5.0f, static_cast<float>(sin(radianAngle)) * 5.0f};
            bullets.push_back(bullet);  // Add the new bullet to the list
        }
    }

    // Function to spawn new power-ups with a small chance
    void spawnPowerUps() {
        if (rand() % 1000 < 2) {
            PowerUp::Type type = static_cast<PowerUp::Type>(rand() % 3);  // Randomly select power-up type
            PowerUp powerUp(type);
            powerUp.active = true;
            powerUp.position = {float(rand() % SCREEN_WIDTH), float(rand() % SCREEN_HEIGHT)};
            powerUps.push_back(powerUp);
        }
    }

    // Function to update power-ups and apply their effects if collected
    void updatePowerUps() {
        for (auto& powerUp : powerUps) {
            if (powerUp.active) {
                // Check if the player collects the power-up
                if (checkCollision(player.position, powerUp.position, 10.0f)) {
                    powerUp.active = false;
                    activatePowerUp(powerUp.type);  // Activate the corresponding power-up
                }
            }
        }

        // Decrease timers for active power-ups
        if (rapidFireActive) {
            rapidFireTimer -= 0.1f;
            if (rapidFireTimer <= 0.0f) {
                rapidFireActive = false;  // Deactivate rapid fire
            }
        }

        if (shieldActive) {
            shieldTimer -= 0.1f;
            if (shieldTimer <= 0.0f) {
                shieldActive = false;  // Deactivate shield
            }
        }
    }

    // Function to activate the effects of a power-up
    void activatePowerUp(PowerUp::Type type) {
        switch (type) {
            case PowerUp::SpeedBoost:
                player.velocity = player.velocity * 1.5f;  // Increase speed
                break;
            case PowerUp::RapidFire:
                rapidFireActive = true;
                rapidFireTimer = 10.0f;  // Set duration for rapid fire
                break;
            case PowerUp::Shield:
                shieldActive = true;
                shieldTimer = 10.0f;  // Set duration for shield
                break;
        }
    }

    // Function to check for collisions between objects
    void checkCollisions() {
        // Check collisions between player and asteroids
        for (auto& asteroid : asteroids) {
            if (asteroid.active && checkCollision(player.position, asteroid.position, 20.0f)) {
                if (shieldActive) {
                    asteroid.active = false;  // Destroy asteroid if shield is active
                } else {
                    player.active = false;  // Deactivate player if shield is not active
                    lives--;  // Decrease lives
                }
            }

            // Check collisions between bullets and asteroids
            for (auto& bullet : bullets) {
                if (bullet.active && asteroid.active && checkCollision(bullet.position, asteroid.position, 10.0f)) {
                    bullet.active = false;
                    asteroid.active = false;
                    score += asteroid.size * 10;  // Increase score based on asteroid size

                    // If asteroid size is greater than 1, spawn smaller asteroids
                    if (asteroid.size > 1) {
                        for (int i = 0; i < 2; ++i) {
                            Asteroid smallerAsteroid;
                            smallerAsteroid.size = asteroid.size - 1;
                            smallerAsteroid.active = true;
                            smallerAsteroid.position = asteroid.position;
                            smallerAsteroid.velocity = {float(rand() % 3 - 1), float(rand() % 3 - 1)};
                            asteroids.push_back(smallerAsteroid);  // Add new smaller asteroids
                        }
                    }
                }
            }
        }

        // Check collisions between player and saucer
        if (saucer.active & checkCollision(player.position, saucer.position, 15.0f)) {
            if (shieldActive) {
                saucer.active = false;  // Destroy saucer if shield is active
            } else {
                player.active = false;  // Deactivate player if shield is not active
                lives--;  // Decrease lives
            }
        }

        // Check collisions between bullets and saucer
        for (auto& bullet : bullets) {
            if (bullet.active && saucer.active && checkCollision(bullet.position, saucer.position, 10.0f)) {
                bullet.active = false;
                saucer.active = false;
                score += saucer.small ? 200 : 100;  // Increase score based on saucer size
            }
        }
    }

    // Function to check if two objects have collided based on their positions and a radius
    bool checkCollision(const Vector2& pos1, const Vector2& pos2, float radius) {
        float dx = pos1.x - pos2.x;
        float dy = pos1.y - pos2.y;
        return (dx * dx + dy * dy) <= (radius * radius);  // Check if the distance between positions is within the collision radius
    }

    // Function to handle screen wrapping, where objects reappear on the opposite side if they go off-screen
    void wrapAround(Vector2& position) {
        if (position.x < 0) position.x += SCREEN_WIDTH;
        if (position.x >= SCREEN_WIDTH) position.x -= SCREEN_WIDTH;
        if (position.y < 0) position.y += SCREEN_HEIGHT;
        if (position.y >= SCREEN_HEIGHT) position.y -= SCREEN_HEIGHT;
    }
};

// Main function that creates and runs the game
int main() {
    Game game;
    game.run(0);  // Start the game loop
    return 0;
}
