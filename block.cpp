#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

const int width = 40;
const int height = 20;
const char paddleChar = '=';
const char ballChar = 'O';
const char brickChar = '#';
const char emptyChar = ' ';

struct Paddle {
    int x;
    int y;
    int length = 7;
};

struct Ball {
    int x;
    int y;
    int dx = 1;
    int dy = -1;
};

void draw(const std::vector<std::vector<char>>& board) {
    system("clear");  // Use "cls" for Windows
    for (const auto& row : board) {
        for (char cell : row) {
            std::cout << cell;
        }
        std::cout << std::endl;
    }
}

void updateBoard(std::vector<std::vector<char>>& board, const Paddle& paddle, const Ball& ball, const std::vector<std::pair<int, int>>& bricks) {
    for (auto& row : board) {
        std::fill(row.begin(), row.end(), emptyChar);
    }

    for (const auto& brick : bricks) {
        board[brick.second][brick.first] = brickChar;
    }

    for (int i = 0; i < paddle.length; ++i) {
        board[paddle.y][paddle.x + i] = paddleChar;
    }

    board[ball.y][ball.x] = ballChar;
}

void movePaddle(Paddle& paddle, char direction) {
    if (direction == 'a' && paddle.x > 0) {
        --paddle.x;
    } else if (direction == 'd' && paddle.x + paddle.length < width) {
        ++paddle.x;
    }
}

void moveBall(Ball& ball, Paddle& paddle, std::vector<std::pair<int, int>>& bricks) {
    ball.x += ball.dx;
    ball.y += ball.dy;

    if (ball.x <= 0 || ball.x >= width - 1) {
        ball.dx = -ball.dx;
    }

    if (ball.y <= 0) {
        ball.dy = -ball.dy;
    }

    if (ball.y == paddle.y - 1 && ball.x >= paddle.x && ball.x < paddle.x + paddle.length) {
        ball.dy = -ball.dy;
    }

    for (auto it = bricks.begin(); it != bricks.end(); ++it) {
        if (ball.x == it->first && ball.y == it->second) {
            bricks.erase(it);
            ball.dy = -ball.dy;
            break;
        }
    }
}

bool gameOver(const Ball& ball) {
    return ball.y >= height;
}

int main() {
    std::vector<std::vector<char>> board(height, std::vector<char>(width, emptyChar));
    Paddle paddle = { width / 2 - 3, height - 1 };
    Ball ball = { width / 2, height / 2 };

    std::vector<std::pair<int, int>> bricks;
    for (int i = 0; i < width; i += 2) {
        for (int j = 0; j < 4; ++j) {
            bricks.push_back({ i, j });
        }
    }

    while (true) {
        if (std::cin.peek() != EOF) {
            char ch;
            std::cin >> ch;
            movePaddle(paddle, ch);
        }

        moveBall(ball, paddle, bricks);
        updateBoard(board, paddle, ball, bricks);
        draw(board);

        if (gameOver(ball)) {
            std::cout << "Game Over!" << std::endl;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
