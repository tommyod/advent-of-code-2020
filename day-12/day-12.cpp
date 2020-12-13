#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <cassert>
#include <set>
#include <queue>
#include <regex>
#include <algorithm>
#include <tuple>
#include <cmath>

using namespace std;

// Read a file to a vector of strings
vector<tuple<string, int>> read_file(const string filename)
{
    vector<tuple<string, int>> lines;
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            string command = line.substr(0, 1);
            int argument = stoi(line.substr(1, line.size() - 1));
            lines.push_back({command, argument});
        }
        file.close();
    }

    return lines;
}

int part1(const vector<tuple<string, int>> &commands)
{

    // State variables
    int x_pos = 0;
    int y_pos = 0;
    int x_direction = 1;
    int y_direction = 0;

    map<string, tuple<int, int>> directions{{"N", {0, 1}},
                                            {"S", {0, -1}},
                                            {"E", {1, 0}},
                                            {"W", {-1, 0}}};

    // Loop over commands and update state
    for (auto [command, argument] : commands)
    {

        // Moving east, west, north or south
        if (directions.contains(command))
        {
            auto [xdir, ydir] = directions[command];
            x_pos += xdir * argument;
            y_pos += ydir * argument;
            continue;
        }

        // Forward movement
        if (command == "F")
        {
            x_pos += x_direction * argument;
            y_pos += y_direction * argument;
            continue;
        }

        // Rotations - use rotation matrix
        int rotation_direction;
        if (command == "R")
            rotation_direction = -1;
        else if (command == "L")
            rotation_direction = 1;

        float angle = 2 * M_PI * (argument * rotation_direction) / 360;
        int x_dir_old = x_direction;
        int y_dir_old = y_direction;
        x_direction = round(x_dir_old * cos(angle) - y_dir_old * sin(angle));
        y_direction = round(x_dir_old * sin(angle) + y_dir_old * cos(angle));

        // printf("(%i, %i) -> (%i, %i)\n", x_dir_old, y_dir_old, x_direction, y_direction);

        // Norm must be equal to unity
        assert(abs(x_direction) + abs(y_direction) == 1);
    }

    return abs(x_pos) + abs(y_pos);
}

int part2(const vector<tuple<string, int>> &commands)
{

    // State variables
    int x_pos = 0;
    int y_pos = 0;
    int x_wp = 10;
    int y_wp = 1;

    map<string, tuple<int, int>> directions{{"N", {0, 1}},
                                            {"S", {0, -1}},
                                            {"E", {1, 0}},
                                            {"W", {-1, 0}}};

    // Loop over commands and update state
    for (auto [command, argument] : commands)
    {

        // Update the waypoint
        if (directions.contains(command))
        {
            auto [xdir, ydir] = directions[command];
            x_wp += xdir * argument;
            y_wp += ydir * argument;
            continue;
        }

        // Forward movement
        if (command == "F")
        {
            x_pos += x_wp * argument;
            y_pos += y_wp * argument;
            continue;
        }

        // Rotations
        int rotation_direction;
        if (command == "R")
            rotation_direction = -1;
        else if (command == "L")
            rotation_direction = 1;

        // Use a rotation matrix to rotate the waypoint
        float angle = 2 * M_PI * (argument * rotation_direction) / 360;
        int x_wp_old = x_wp;
        int y_wp_old = y_wp;
        x_wp = round(x_wp_old * cos(angle) - y_wp_old * sin(angle));
        y_wp = round(x_wp_old * sin(angle) + y_wp_old * cos(angle));

        // Rotation will not change the norm
        assert(abs(x_wp) + abs(y_wp) == abs(x_wp_old) + abs(y_wp_old));
    }

    return abs(x_pos) + abs(y_pos);
}

int main()
{
    // Read input data
    auto commands = read_file("day-12-input.txt");

    auto answer1 = part1(commands);
    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 759);

    auto answer2 = part2(commands);
    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == 45763);
}