
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <cassert>
#include <set>
#include <unordered_set>
#include <queue>
#include <numeric>
#include <regex>
#include <algorithm>
#include <tuple>
#include <iterator>
#include <list>
#include <cmath>
#include <unistd.h>
#include <cstdlib>
#include <unordered_map>

using namespace std;

// Read a file to a vector of strings
vector<string>
read_file(const string filename)
{
    vector<string> lines;
    fstream inputFile(filename, fstream::in);
    string file_line;

    // While there is data to read
    while (inputFile && getline(inputFile, file_line))
    {
        if (file_line.size())
        {
            lines.push_back(file_line);
        }
    }
    inputFile.close();
    return lines;
}

// Parse the coordinates to strings
vector<string> parse_coords(const string line)
{

    vector<string> coords;
    set<string> double_strs{"se", "sw", "nw", "ne"};
    int start = 0;
    while (start < line.size())
    {

        //  These directions are given in your list, respectively, as e, se, sw, w, nw, and ne.
        if ((line.size() - start >= 2) && (double_strs.contains(line.substr(start, 2))))
        {
            coords.push_back(line.substr(start, 2));
            start = start + 2;
        }
        else
        {
            coords.push_back(line.substr(start, 1));
            start = start + 1;
        }
    }

    return coords;
}

// Parse coords to an x-y system
// +1 in y direction goes north-east
// +1 in x direction goes east
tuple<int, int> coord_to_ints(string str_coord)
{
    if (str_coord == "e")
    {
        return make_tuple(1, 0);
    }
    else if (str_coord == "w")
    {
        return make_tuple(-1, 0);
    }
    else if (str_coord == "ne")
    {
        return make_tuple(0, 1);
    }
    else if (str_coord == "nw")
    {
        return make_tuple(-1, 1);
    }
    else if (str_coord == "se")
    {
        return make_tuple(1, -1);
    }
    else if (str_coord == "sw")
    {
        return make_tuple(0, -1);
    }
    else
    {
        assert(false); // Should never happen
    }
}

map<tuple<int, int>, bool> part1(const vector<string> &lines)
{

    map<tuple<int, int>, bool> tile_is_black;

    // For each line
    for (auto &line : lines)
    {
        // Start coodinates
        int x_coord = 0;
        int y_coord = 0;

        // Parse the steps into delta X and delta Y
        for (auto &coord : parse_coords(line))
        {

            // Increment current coordinates
            auto [delta_x, delta_y] = coord_to_ints(coord);
            x_coord = x_coord + delta_x;
            y_coord = y_coord + delta_y;
        }

        // If it's already black, flip it
        // If not, set it to black
        auto coord = make_tuple(x_coord, y_coord);
        if (tile_is_black.contains(coord))
        {
            tile_is_black[coord] = !(tile_is_black[coord]);
        }
        else
        {
            tile_is_black[coord] = true;
        }
    }

    return tile_is_black;
}

map<tuple<int, int>, bool> set_empty_neighbors_to_white(map<tuple<int, int>, bool> &tile_is_black)
{

    map<tuple<int, int>, bool> tile_is_black_new;
    vector<tuple<int, int>> neighbors{
        {1, 0}, {-1, 0}, {0, 1}, {-1, 1}, {1, -1}, {0, -1}};

    // For each tile
    for (auto [key, value] : tile_is_black)
    {
        auto [x, y] = key;

        // Copy this position
        tile_is_black_new[key] = tile_is_black[key];

        // For each neighbor
        for (auto [delta_x, delta_y] : neighbors)
        {
            int this_x = x + delta_x;
            int this_y = y + delta_y;

            // Does not contain the key -> white tile
            if (!tile_is_black.contains({this_x, this_y}))
            {
                tile_is_black_new[{this_x, this_y}] = false;
            }
        }
    }

    return tile_is_black_new;
}

// Solve part 2
// This solution is slow, because I add a new layer of white tiles in
// every iteration. This is not necessary, but it's the simplest way
// I found to get a correct algorithm working
int part2(map<tuple<int, int>, bool> &tile_is_black, int simulations = 100)
{

    vector<tuple<int, int>> neighbors{
        {1, 0}, {-1, 0}, {0, 1}, {-1, 1}, {1, -1}, {0, -1}};

    // For each day in the simulation
    for (int simulation = 1; simulation <= simulations; simulation++)
    {

        // cout << "Simulation " << simulation << endl;
        // cout << "Tiles in memory: " << tile_is_black.size() << endl;

        tile_is_black = set_empty_neighbors_to_white(tile_is_black);
        map<tuple<int, int>, bool> tile_is_black_new;

        // For each tile
        for (auto [key, value] : tile_is_black)
        {
            auto [x, y] = key;
            int black_neighbors = 0;
            int white_neighbors = 0;
            //cout << x << " " << y << endl;

            // For each neighbor
            for (auto [delta_x, delta_y] : neighbors)
            {
                int this_x = x + delta_x;
                int this_y = y + delta_y;

                // Does not contain the key -> white tile
                if (!tile_is_black.contains({this_x, this_y}))
                {
                    white_neighbors++;
                }
                else if (!tile_is_black[{this_x, this_y}])
                {
                    white_neighbors++;
                }
                else if (tile_is_black[{this_x, this_y}])
                {
                    black_neighbors++;
                }
                else
                {
                    assert(false); // should never happen
                }
            }

            assert(black_neighbors + white_neighbors == 6);

            // Any black tile with zero or more than 2 black tiles
            // immediately adjacent to it is flipped to white.
            if (tile_is_black[{x, y}] && ((black_neighbors == 0) || (black_neighbors > 2)))
            {
                tile_is_black_new[{x, y}] = false;
            }

            // Any white tile with exactly 2 black tiles
            // immediately adjacent to it is flipped to black.
            else if ((!tile_is_black[{x, y}]) && (black_neighbors == 2))
            {
                tile_is_black_new[{x, y}] = true;
            }
            // If neither condition triggered, copy the value unchanged
            else
            {
                tile_is_black_new[{x, y}] = tile_is_black[{x, y}];
            }
        }

        // Update for next iteration
        tile_is_black = tile_is_black_new;
    }

    int black_tiles = 0;
    for (auto [key, value] : tile_is_black)
    {
        if (value)
        {
            black_tiles++;
        }
    }
    return black_tiles;
}

int main()
{

    auto lines = read_file("day-24-input.txt");

    // =========================================
    // ========= Solve part 1 ==================
    // =========================================

    // All tiles start white
    auto tile_is_black = part1(lines);

    int answer1 = 0;
    for (auto [key, value] : tile_is_black)
    {
        if (value)
        {
            answer1++;
        }
    }

    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 289);

    // =========================================
    // ========= Solve part 2 ==================
    // =========================================

    auto answer2 = part2(tile_is_black, 100);
    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == 3551);
};