#include <iostream>
#include <vector>
#include <string>
#include "src.hpp"

struct Map {
    int height, width;
    std::vector<std::string> grid;
};

bool is_food(Map* map, int x, int y) {
    return map->grid[x][y] == '*';
}

bool is_wall(Map* map, int x, int y) {
    return map->grid[x][y] == '#';
}

void eat_food(Map* map, int x, int y) {
    map->grid[x][y] = '.';
}

int get_height(Map* map) {
    return map->height;
}

int get_width(Map* map) {
    return map->width;
}

void print_map(Map* map, Snake& snake) {
    std::vector<std::string> display = map->grid;
    // Clear all food/walls that might be covered by snake for a fresh view if needed?
    // Actually, the snake is ON TOP of the grid.
    // The grid might contain '.' or '*' or '#'.
    // If it's food, it will be eaten.

    // Draw snake body
    for (size_t i = 1; i < snake.body.size(); ++i) {
        display[snake.body[i].first][snake.body[i].second] = 'o';
    }
    // Draw snake head
    display[snake.body[0].first][snake.body[0].second] = '@';

    for (int i = 0; i < map->height; ++i) {
        std::cout << display[i] << "\n";
    }
}

int main() {
    int h, w;
    if (!(std::cin >> h >> w)) return 0;
    Map map;
    map.height = h;
    map.width = w;
    map.grid.resize(h);
    int sx = -1, sy = -1;
    for (int i = 0; i < h; ++i) {
        std::cin >> map.grid[i];
        for (int j = 0; j < w; ++j) {
            if (map.grid[i][j] == '@') {
                sx = i;
                sy = j;
                map.grid[i][j] = '.'; // Clear head from base map
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

    Snake snake(sx, sy, start_dir);

    std::string instr_str;
    int round = 0;
    while (std::cin >> instr_str) {
        instruction instr;
        if (instr_str == "U") instr = instruction::UP;
        else if (instr_str == "D") instr = instruction::DOWN;
        else if (instr_str == "L") instr = instruction::LEFT;
        else if (instr_str == "R") instr = instruction::RIGHT;
        else instr = instruction::NONE;

        snake.move(instr, &map);
        round++;

        if (snake.is_dead) {
            std::cout << "Game Over\n";
            std::cout << "Score: " << snake.body.size() + round << "\n";
            return 0;
        }

        std::cout << "Round: " << round << "\n";
        print_map(&map, snake);
    }

    return 0;
}
