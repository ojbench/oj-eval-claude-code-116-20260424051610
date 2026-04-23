#ifndef SRC_HPP
#define SRC_HPP

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <deque>

enum class instruction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

struct Map;

// These are implemented in main.cpp
bool is_food(Map* map, int x, int y);
bool is_wall(Map* map, int x, int y);
void eat_food(Map* map, int x, int y);
int get_height(Map* map);
int get_width(Map* map);

struct Map {
    int height, width;
    std::vector<std::string> grid;
    bool is_food(int x, int y) { return grid[x][y] == '*'; }
    bool is_wall(int x, int y) { return grid[x][y] == '#'; }
    void eat_food(int x, int y) { grid[x][y] = '.'; }
    int get_height() { return height; }
    int get_width() { return width; }
};

class Snake {
public:
    std::deque<std::pair<int, int>> body;
    instruction current_direction;
    bool is_dead;

    Snake() : is_dead(false), current_direction(instruction::NONE) {}
    Snake(int x, int y, instruction dir) : is_dead(false), current_direction(dir) {
        body.push_back({x, y});
    }

    void move(instruction next_instr, Map* map) {
        if (is_dead) return;

        instruction next_dir = current_direction;
        if (next_instr != instruction::NONE) {
            if ((next_instr == instruction::UP && current_direction == instruction::DOWN) ||
                (next_instr == instruction::DOWN && current_direction == instruction::UP) ||
                (next_instr == instruction::LEFT && current_direction == instruction::RIGHT) ||
                (next_instr == instruction::RIGHT && current_direction == instruction::LEFT)) {
                is_dead = true;
                return;
            }
            next_dir = next_instr;
        }
        current_direction = next_dir;

        std::pair<int, int> head = body.front();
        int nx = head.first;
        int ny = head.second;

        if (current_direction == instruction::UP) nx--;
        else if (current_direction == instruction::DOWN) nx++;
        else if (current_direction == instruction::LEFT) ny--;
        else if (current_direction == instruction::RIGHT) ny++;

        int h = ::get_height(map);
        int w = ::get_width(map);

        if (nx < 0 || nx >= h || ny < 0 || ny >= w || ::is_wall(map, nx, ny)) {
            is_dead = true;
            return;
        }

        bool eating = ::is_food(map, nx, ny);
        for (int i = 0; i < (int)(eating ? body.size() : body.size() - 1); ++i) {
            if (body[i].first == nx && body[i].second == ny) {
                is_dead = true;
                return;
            }
        }

        body.push_front({nx, ny});
        if (eating) {
            ::eat_food(map, nx, ny);
        } else {
            body.pop_back();
        }
    }

    std::pair<int, std::pair<int, int> *> get_snake() {
        int len = (int)body.size();
        std::pair<int, int>* arr = new std::pair<int, int>[len];
        for (int i = 0; i < len; ++i) {
            arr[i] = body[i];
        }
        return {len, arr};
    }
};

class Game {
public:
    Map* map;
    Snake snake;
    int round;

    Game() : map(nullptr), round(0) {}

    void initialize() {
        int h, w;
        if (!(std::cin >> h >> w)) return;
        map = new Map();
        map->height = h;
        map->width = w;
        map->grid.resize(h);
        int sx = -1, sy = -1;
        for (int i = 0; i < h; ++i) {
            std::cin >> map->grid[i];
            for (int j = 0; j < w; ++j) {
                if (map->grid[i][j] == '@') {
                    sx = i; sy = j;
                    map->grid[i][j] = '.';
                }
            }
        }
        char dir_char;
        std::cin >> dir_char;
        instruction start_dir;
        if (dir_char == 'U') start_dir = instruction::UP;
        else if (dir_char == 'D') start_dir = instruction::DOWN;
        else if (dir_char == 'L') start_dir = instruction::LEFT;
        else start_dir = instruction::RIGHT;
        snake = Snake(sx, sy, start_dir);
    }

    void print_game() {
        std::vector<std::string> display = map->grid;
        for (size_t i = 1; i < snake.body.size(); ++i) {
            display[snake.body[i].first][snake.body[i].second] = 'o';
        }
        display[snake.body[0].first][snake.body[0].second] = '@';
        for (int i = 0; i < map->height; ++i) {
            std::cout << display[i] << "\n";
        }
    }

    void play() {
        std::string instr_str;
        while (std::cin >> instr_str) {
            instruction instr;
            if (instr_str == "U") instr = instruction::UP;
            else if (instr_str == "D") instr = instruction::DOWN;
            else if (instr_str == "L") instr = instruction::LEFT;
            else if (instr_str == "R") instr = instruction::RIGHT;
            else instr = instruction::NONE;

            snake.move(instr, map);

            if (snake.is_dead) {
                std::cout << "Game Over\n";
                std::cout << "Score: " << (int)snake.body.size() + round << "\n";
                return;
            }

            round++;
            std::cout << "Round: " << round << "\n";
            print_game();
        }
    }
};

#endif
