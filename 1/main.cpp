#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define DIAL_MIN         0
#define DIAL_MAX         99
#define INITIAL_DIAL_POS 50

typedef enum {
    left,
    right
} dir_t;

std::vector<std::string> split_lines(std::string input) {
    std::stringstream        stream(input);

    std::string              line_buffer;
    std::vector<std::string> lines;

    while (std::getline(stream, line_buffer, '\n')) {
        lines.push_back(line_buffer);
    }

    return lines;
}

class instruction {
  public:
    dir_t dir;
    int   dist;

    instruction(const std::string line) {
        parse_dir(line);
        parse_dist(line);
    }

    void parse_dir(std::string line) {
        if (line[0] == 'L') {
            dir = left;
        } else {
            dir = right;
        }
    }

    void parse_dist(std::string line) {
        std::string dist_str = line.substr(1, line.length());
        dist                 = std::stoi(dist_str, nullptr);
    }
};

class dial {
  public:
    int pos;

    dial() { pos = INITIAL_DIAL_POS; }

    int rotate(instruction inst) {
        if (inst.dir == left) {
            rotate_left(inst.dist);
        } else if (inst.dir == right) {
            rotate_right(inst.dist);
        }

        return pos;
    }

  private:
    void rotate_left(const int dist) {
        int dist_cpy = dist;
        while (dist_cpy > 0) {
            int diff = pos - dist_cpy;
            if (diff < 0) {
                diff     = std::abs(diff) - 1;
                pos      = DIAL_MAX;
                dist_cpy = diff;
            } else {
                pos      -= dist_cpy;
                dist_cpy  = 0;
            }
        }
    }

    void rotate_right(const int dist) {
        int dist_cpy = dist;
        while (dist_cpy > 0) {
            int diff = pos + dist_cpy;
            if (diff > DIAL_MAX) {
                diff     = diff - DIAL_MAX;
                pos      = 0;
                dist_cpy = diff - 1;
            } else {
                pos      += dist_cpy;
                dist_cpy  = 0;
            }
        }
    }
};

int main(void) {
    std::ifstream     reader("input");
    std::stringstream sbuff;
    sbuff << reader.rdbuf();

    std::vector<std::string> lines = split_lines(sbuff.str());
    std::vector<instruction> instructions;

    for (const std::string line : lines) {
        instruction conversion(line);
        instructions.push_back(conversion);
    }

    dial d;
    int  ctr = 0;

    for (const instruction i : instructions) {
        int res = d.rotate(i);
        if (res == 0) {
            ctr += 1;
        }
    }

    printf("res: %d\n", ctr);

    return 0;
}