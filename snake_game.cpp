#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>

// Constants
const int WIDTH = 40;
const int HEIGHT = 20;
const char SNAKE_CHAR = 'O';
const char FOOD_CHAR = '*';
const char WALL_CHAR = '#';
const char EMPTY_CHAR = ' ';

// Directions
enum Direction { UP, DOWN, LEFT, RIGHT, STOP };

// Game state
struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class SnakeGame {
private:
    std::vector<Point> snake;
    Point food;
    Direction direction;
    Direction nextDirection;
    int score;
    bool gameOver;

public:
    SnakeGame() : direction(STOP), nextDirection(STOP), score(0), gameOver(false) {
        srand(static_cast<unsigned>(time(0)));
        
        // Initialize snake in the middle
        snake.push_back(Point(WIDTH / 2, HEIGHT / 2));
        snake.push_back(Point(WIDTH / 2 - 1, HEIGHT / 2));
        snake.push_back(Point(WIDTH / 2 - 2, HEIGHT / 2));
        
        // Place initial food
        spawnFood();
    }

    void spawnFood() {
        Point newFood;
        bool validPosition = false;
        
        while (!validPosition) {
            newFood.x = rand() % (WIDTH - 2) + 1;
            newFood.y = rand() % (HEIGHT - 2) + 1;
            
            validPosition = true;
            for (const auto& segment : snake) {
                if (segment == newFood) {
                    validPosition = false;
                    break;
                }
            }
        }
        food = newFood;
    }

    void input() {
        if (_kbhit()) {
            int key = _getch();
            
            switch (key) {
                case 'w':
                case 'W':
                    if (direction != DOWN) nextDirection = UP;
                    break;
                case 's':
                case 'S':
                    if (direction != UP) nextDirection = DOWN;
                    break;
                case 'a':
                case 'A':
                    if (direction != RIGHT) nextDirection = LEFT;
                    break;
                case 'd':
                case 'D':
                    if (direction != LEFT) nextDirection = RIGHT;
                    break;
                case 27: // ESC
                    gameOver = true;
                    break;
            }
        }
    }

    void update() {
        if (nextDirection != STOP) {
            direction = nextDirection;
        }

        if (direction == STOP) return;

        // Calculate new head position
        Point newHead = snake[0];
        
        switch (direction) {
            case UP:
                newHead.y--;
                break;
            case DOWN:
                newHead.y++;
                break;
            case LEFT:
                newHead.x--;
                break;
            case RIGHT:
                newHead.x++;
                break;
            case STOP:
                break;
        }

        // Check wall collision
        if (newHead.x <= 0 || newHead.x >= WIDTH - 1 || 
            newHead.y <= 0 || newHead.y >= HEIGHT - 1) {
            gameOver = true;
            return;
        }

        // Check self collision
        for (const auto& segment : snake) {
            if (newHead == segment) {
                gameOver = true;
                return;
            }
        }

        // Add new head
        snake.insert(snake.begin(), newHead);

        // Check food collision
        if (newHead == food) {
            score += 10;
            spawnFood();
        } else {
            // Remove tail if no food eaten
            snake.pop_back();
        }
    }

    void draw() {
        system("cls");
        
        std::cout << "Snake Game - Score: " << score << std::endl;
        std::cout << "Controls: W/A/S/D to move, ESC to quit" << std::endl;
        std::cout << std::string(WIDTH, WALL_CHAR) << std::endl;

        for (int y = 1; y < HEIGHT - 1; y++) {
            std::cout << WALL_CHAR;
            
            for (int x = 1; x < WIDTH - 1; x++) {
                Point current(x, y);
                bool drawn = false;

                // Draw snake
                for (size_t i = 0; i < snake.size(); i++) {
                    if (snake[i] == current) {
                        if (i == 0) {
                            std::cout << '@'; // Head
                        } else {
                            std::cout << SNAKE_CHAR;
                        }
                        drawn = true;
                        break;
                    }
                }

                // Draw food
                if (!drawn && current == food) {
                    std::cout << FOOD_CHAR;
                    drawn = true;
                }

                // Draw empty space
                if (!drawn) {
                    std::cout << EMPTY_CHAR;
                }
            }
            
            std::cout << WALL_CHAR << std::endl;
        }

        std::cout << std::string(WIDTH, WALL_CHAR) << std::endl;
    }

    bool isGameOver() const {
        return gameOver;
    }

    int getScore() const {
        return score;
    }

    void run() {
        while (!gameOver) {
            draw();
            input();
            update();
            Sleep(100); // 100ms delay for game speed
        }

        system("cls");
        std::cout << "Game Over! Final Score: " << score << std::endl;
    }
};

int main() {
    std::cout << "Welcome to Snake Game!" << std::endl;
    std::cout << "Press any key to start..." << std::endl;
    _getch();

    SnakeGame game;
    game.run();

    return 0;
}
