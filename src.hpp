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

bool is_food(Map* map, int x, int y);
bool is_wall(Map* map, int x, int y);
void eat_food(Map* map, int x, int y);
int get_height(Map* map);
int get_width(Map* map);

class Snake {
public:
    std::deque<std::pair<int, int>> body;
    instruction current_direction;
    bool is_dead;
    int score;
    int rounds;

    Snake(int x, int y, instruction dir) {
        body.push_back({x, y});
        current_direction = dir;
        is_dead = false;
        score = 0;
        rounds = 0;
    }

    void move(instruction next_instr, Map* map) {
        if (is_dead) return;

        rounds++;
        // Check reverse direction AFTER updating move_dir from N if needed?
        // Wait, current_direction is already updated in previous moves.
        // If next_instr is NONE, move_dir = current_direction, no reverse check needed.
        // If next_instr is not NONE, we check it against current_direction.
        if (next_instr != instruction::NONE) {
            if ((next_instr == instruction::UP && current_direction == instruction::DOWN) ||
                (next_instr == instruction::DOWN && current_direction == instruction::UP) ||
                (next_instr == instruction::LEFT && current_direction == instruction::RIGHT) ||
                (next_instr == instruction::RIGHT && current_direction == instruction::LEFT)) {
                is_dead = true;
                // Move snake one last time? "直接判定为死亡"
                // Usually this means it dies at the CURRENT round.
                return;
            }
            current_direction = next_instr;
        }

        std::pair<int, int> head = body.front();
        int nx = head.first;
        int ny = head.second;

        if (current_direction == instruction::UP) nx--;
        else if (current_direction == instruction::DOWN) nx++;
        else if (current_direction == instruction::LEFT) ny--;
        else if (current_direction == instruction::RIGHT) ny++;

        int height = get_height(map);
        int width = get_width(map);

        // Check boundaries
        if (nx < 0 || nx >= height || ny < 0 || ny >= width) {
            is_dead = true;
            return;
        }

        // Check wall
        if (is_wall(map, nx, ny)) {
            is_dead = true;
            return;
        }

        // Check if hitting its own body
        // Note: if the tail moves out, it's fine, BUT if it eats food, the tail stays.
        // Actually, if it hits the body, it's dead regardless of whether it's food or not,
        // EXCEPT if it hits the tail and the tail is about to move.
        // Wait, the rules say: "碰到墙壁、边界或者自己的身体，游戏结束".
        // Usually, in snake, hitting the tail that is about to move is NOT death.
        // But let's check the order: "每次移动一格，每次移动后，尾部减少一格".
        // If it eats food, length increases by 1.

        bool eating = is_food(map, nx, ny);

        // Check body collision BEFORE moving (but considering if tail will move)
        // If snake length is 1, body.size() - 1 is 0, so it won't hit itself.
        for (int i = 0; i < (int)(eating ? body.size() : body.size() - 1); ++i) {
            if (body[i].first == nx && body[i].second == ny) {
                is_dead = true;
                return;
            }
        }

        body.push_front({nx, ny});
        if (eating) {
            eat_food(map, nx, ny);
        } else {
            body.pop_back();
        }
    }

    std::pair<int, std::pair<int, int> *> get_snake() {
        int len = body.size();
        std::pair<int, int>* arr = new std::pair<int, int>[len];
        for (int i = 0; i < len; ++i) {
            arr[i] = body[i];
        }
        return {len, arr};
    }
};

#endif
