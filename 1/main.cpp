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
    int count;
    int zeroed;

    dial() {
        pos    = INITIAL_DIAL_POS;
        zeroed = 0;
        count  = 0;
    }

    void rotate(instruction inst) {
        if (inst.dir == left) {
            rotate_left(inst.dist);
        } else if (inst.dir == right) {
            rotate_right(inst.dist);
        }

        if (pos == 0) {
            count += 1;
        }
    }

  private:
    void rotate_left(const int dist) {
        for (int i = 0; i < dist; i++) {
            pos--;
            if (pos == 0) {
                zeroed++;
            }
            if (pos < 0) {
                pos = DIAL_MAX;
            }
        }
    }

    void rotate_right(const int dist) {
        for (int i = 0; i < dist; i++) {
            pos++;
            if (pos > DIAL_MAX) {
                pos = 0;
                zeroed++;
            }
        }
    }
};

int main(void) {
    // const std::string        test_input = "R1000";
    // const std::string test_input =
    //     "L68\nL30\nR48\nL5\nR60\nL55\nL1\nL99\nR14\nL82";
    // std::vector<std::string> lines = split_lines(test_input);

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
    for (const instruction i : instructions) {
        d.rotate(i);
    }

    // part 1
    printf("res (part 1): %d\n", d.count);

    // part 2
    printf("res (part 2): %d\n", d.zeroed);
    return 0;
}