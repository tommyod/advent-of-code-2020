
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
#include <cmath>
#include <unistd.h>
#include <cstdlib>
#include <unordered_map>

using namespace std;

class Tile
{

public:
    // Properties of a Tile
    vector<int> left_col;
    vector<int> right_col;
    vector<int> top_row;
    vector<int> bottom_row;
    long id;

    // Pointers to neighbors
    int left = -1;
    int right = -1;
    int top = -1;
    int bottom = -1;
    int container_pos = -1;

    bool is_connected()
    {
        return (left + right + top + bottom) > -4;
    }

    bool is_dummy()
    {
        return left_col.size() == 0;
    }

    Tile() {}

    Tile(long id, vector<vector<int>> matrix)
    {

        this->id = id;
        int rows = matrix.size();
        int columns = matrix[0].size();
        assert(rows == columns);

        // Rows
        top_row = matrix[0];
        bottom_row = matrix[rows - 1];

        // Columns
        for (int row = 0; row < rows; row++)
        {
            left_col.push_back(matrix[row][0]);
            right_col.push_back(matrix[row][columns - 1]);
        }
        reverse(left_col.begin(), left_col.end());
        reverse(bottom_row.begin(), bottom_row.end());

        // Verify that corners match
        verify();
    }

    void verify()
    {
        // Verify that corners match
        assert(left_col[left_col.size() - 1] == top_row[0]);
        assert(top_row[top_row.size() - 1] == right_col[0]);
        assert(right_col[right_col.size() - 1] == bottom_row[0]);
        assert(bottom_row[bottom_row.size() - 1] == left_col[0]);
    }

    Tile(long id, vector<int> l, vector<int> t, vector<int> r, vector<int> b)
    {

        this->id = id;
        left_col = l;
        right_col = r;
        top_row = t;
        bottom_row = b;

        // Verify that corners match
        verify();
    }

    // Flip over | (vertical line)
    void flip()
    {
        // Columns are exchanged and reversed
        auto temp = left_col;
        left_col = right_col;
        reverse(left_col.begin(), left_col.end());

        right_col = temp;
        reverse(right_col.begin(), right_col.end());

        // Rows are reversed
        reverse(top_row.begin(), top_row.end());
        reverse(bottom_row.begin(), bottom_row.end());

        //cout << "verify flip" << endl;
        verify();
        //return Tile(id, new_left_col, new_top_row, new_right_col, new_bottom_row);
    }

    // Flip over | (vertical line)
    void rotate()
    {
        // Change each one
        auto temp = left_col;

        left_col = top_row;
        top_row = right_col;
        right_col = bottom_row;
        bottom_row = temp;
        //cout << "verify rotation" << endl;
        verify();
        //return Tile(id, new_left_col, new_top_row, new_right_col, new_bottom_row);
    }

    void print()
    {
        int size = top_row.size();
        for (auto element : top_row)
        {
            cout << element << " ";
        }
        cout << endl;
        for (int row = 1; row < size - 1; row++)
        {
            cout << left_col[row] << " ";
            for (int col = 1; col < size - 1; col++)
            {
                cout << "  ";
            }
            cout << right_col[row] << endl;
        }
        for (auto element : bottom_row)
        {
            cout << element << " ";
        }
        cout << endl;
    }

    bool set_right(Tile &tile)
    {
        if (right == -1)
        {
            right = tile.container_pos;
            tile.left = container_pos;
            return true;
        }
        return false;
    }

    bool set_left(Tile &tile)
    {
        if (left == -1)
        {
            left = tile.container_pos;
            tile.right = container_pos;
            return true;
        }
        return false;
    }

    bool set_top(Tile &tile)
    {
        if (top == -1)
        {
            top = tile.container_pos;
            tile.bottom = container_pos;
            return true;
        }
        return false;
    }

    bool set_bottom(Tile &tile)
    {
        if (bottom == -1)
        {
            bottom = tile.container_pos;
            tile.top = container_pos;
            return true;
        }
        return false;
    }
};

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

vector<Tile> lines_to_tiles(const vector<string> &lines)
{

    // Part 1: Partition the input lines
    vector<string> tile_data;
    vector<long> tile_ids;

    for (auto &line : lines)
    {
        if (line.starts_with("Tile"))
        {

            int start = line.find(" ") + 1;
            long tile_id = stol(line.substr(start, line.size() - start - 1));
            tile_ids.push_back(tile_id);
        }
        else
        {
            tile_data.push_back(line);
        }
    }

    // Part 2: collection inputs into a vector of Tile objects
    int rows_per_tile = tile_data.size() / tile_ids.size();
    vector<Tile> tiles;

    // Loop over all chunks of inputs
    for (int tile = 0; tile < tile_ids.size(); tile++)
    {
        auto id = tile_ids[tile];
        vector<vector<int>> tile_matrix;

        // Loop over rows in the chunk, convert to integers
        for (int j = 0; j < rows_per_tile; j++)
        {
            vector<int> row;
            for (auto character : tile_data[tile * rows_per_tile + j])
            {
                if (character == '.')
                    row.push_back(0);
                else
                    row.push_back(1);
            }
            tile_matrix.push_back(row);
        }

        // Print some helpful messages
        cout << "Loading the following tile with ID: " << id << endl;
        for (auto row : tile_matrix)
        {
            for (auto var : row)
            {
                cout << var << " ";
            }
            cout << endl;
        }

        // Add the tile
        tiles.push_back(Tile(id, tile_matrix));

        // (tile_data.begin() + rows_per_tile * tile, tile_data.begin() + rows_per_tile * (tile + 1));
    }

    for (int i = 0; i < tiles.size(); i++)
    {
        tiles[i].container_pos = i;
    }

    return tiles;
}

void print_vector(vector<int> v)
{
    for (auto var : v)
    {
        cout << var << " ";
    }
    cout << endl;
}

void replace_dummy_tile(Tile &dummy_tile, Tile &tile)
{

    tile.top = dummy_tile.top;
    tile.bottom = dummy_tile.bottom;
    tile.left = dummy_tile.left;
    tile.right = dummy_tile.right;
}

bool reverse_match(vector<int> a, vector<int> b)
{
    reverse(a.begin(), a.end());
    return a == b;
}

bool tiles_match(Tile &tile, Tile &other)
{
    for (int i = 0; i < 4; i++)
    {

        // cout << "before after rot:\n"<< &tile << endl;
        other.rotate();

        if (reverse_match(tile.top_row, other.bottom_row))
        {
            return true;
        }
        if (reverse_match(tile.bottom_row, other.top_row))
        {
            return true;
        }
        if (reverse_match(tile.left_col, other.right_col))
        {
            return true;
        }
        if (reverse_match(tile.right_col, other.left_col))
        {
            return true;
        }

        other.flip();

        if (reverse_match(tile.top_row, other.bottom_row))
        {
            return true;
        }
        if (reverse_match(tile.bottom_row, other.top_row))
        {
            return true;
        }
        if (reverse_match(tile.left_col, other.right_col))
        {
            return true;
        }
        if (reverse_match(tile.right_col, other.left_col))
        {
            return true;
        }

        other.flip();
    }

    return false;
}

pair<bool, Tile> find_possible_mach(vector<Tile> &tiles, Tile &tile, Tile &dummy_tile)
{

    // Whether or not a match should be attempted
    bool top = (dummy_tile.top != -1) ? true : false;
    bool bottom = (dummy_tile.bottom != -1) ? true : false;
    bool left = (dummy_tile.left != -1) ? true : false;
    bool right = (dummy_tile.right != -1) ? true : false;

    if (!(top || bottom || left || right))
    {
        return make_pair(false, tile);
    }

    /*
    if (top)
        cout << "top" << endl;
    if (bottom)
        cout << "bottom" << endl;
    if (left)
        cout << "left" << endl;
    if (right)
        cout << "right" << endl;

        */

    for (int i = 0; i < 4; i++)
    {

        // cout << "before after rot:\n"<< &tile << endl;
        tile.rotate();
        // cout << &tile << endl;
        bool matches = true;

        if (top && (!reverse_match(tiles[dummy_tile.top].bottom_row, tile.top_row)))
        {
            //print_vector((*dummy_tile.top).bottom_row);
            //print_vector(tile.top_row);
            matches = false;
        }
        if (bottom && (!reverse_match(tiles[dummy_tile.bottom].top_row, tile.bottom_row)))
        {
            matches = false;
        }
        if (left && (!reverse_match(tiles[dummy_tile.left].right_col, tile.left_col)))
        {
            matches = false;
        }
        if (right && (!reverse_match(tiles[dummy_tile.right].left_col, tile.right_col)))
        {
            matches = false;
        }

        if (matches)
        {
            return make_pair(true, tile);
        }

        tile.flip();

        matches = true;
        if (top && (!reverse_match(tiles[dummy_tile.top].bottom_row, tile.top_row)))
        {
            //print_vector((*dummy_tile.top).bottom_row);
            //print_vector(tile.top_row);
            matches = false;
        }
        if (bottom && (!reverse_match(tiles[dummy_tile.bottom].top_row, tile.bottom_row)))
        {
            matches = false;
        }
        if (left && (!reverse_match(tiles[dummy_tile.left].right_col, tile.left_col)))
        {
            matches = false;
        }
        if (right && (!reverse_match(tiles[dummy_tile.right].left_col, tile.right_col)))
        {
            matches = false;
        }

        if (matches)
        {
            return make_pair(true, tile);
        }

        tile.flip();
    }

    return make_pair(false, tile);
}

int main()
{

    vector<int> a(5);
    iota(a.begin(), a.end(), 0);
    auto p = Tile(5, {1, 2, 3, 4}, {4, 5, 6, 7}, {7, 8, 9, 1}, {1, 2, 3, 1});
    p.flip();
    p.print();

    auto lines = read_file("day-20-input.txt");
    auto tiles = lines_to_tiles(lines);

    unsigned long long answer = 1;
    int corner_tiles = 0;
    // For each tile
    for (int i = 0; i < tiles.size(); i++)
    {
        int matches = 0;
        // For every other tile
        for (int j = 0; j < tiles.size(); j++)
        {

            if (i == j)
            {
                continue;
            }

            if (tiles_match(tiles[i], tiles[j]))
            {
                matches++;
            }
        }

        if (matches == 2)
        {
            answer = answer * (unsigned long long)tiles[i].id;
            corner_tiles++;
        }

        cout << tiles[i].id << " " << matches << endl;
    }

    assert(corner_tiles == 4);
    cout << answer << endl;

    return 0;

    auto tile = tiles[0];

    Tile dummy_left = Tile();
    tile.set_left(dummy_left);
    Tile dummy_right = Tile();
    tile.set_right(dummy_right);
    Tile dummy_top = Tile();
    tile.set_top(dummy_top);
    Tile dummy_bottom = Tile();
    tile.set_bottom(dummy_bottom);

    tiles[0] = tile;

    queue<Tile> dummies;
    dummies.push(dummy_bottom);
    dummies.push(dummy_left);
    dummies.push(dummy_right);
    dummies.push(dummy_top);

    while (dummies.size() > 0)
    {
        usleep(1000000);
        cout << "=== Iteration ===" << endl;
        cout << "Remaining dummies : " << dummies.size() << endl;

        int remaining_tiles = 0;
        for (int tile_num = 1; tile_num < tiles.size(); tile_num++)
        {
            auto t = tiles[tile_num];
            if (!t.is_connected())
            {
                remaining_tiles += 1;
            }
        }

        cout << "Remaining tiles : " << remaining_tiles << endl;
        if (remaining_tiles == 0)
        {
            int k;
        }

        auto dummy_tile = dummies.front();
        dummies.pop();

        printf("Looking at dummy tile with: \n top: %i\n bottom %i\n left %i\n right %i\n",
               dummy_tile.top, dummy_tile.bottom, dummy_tile.left, dummy_tile.right);

        // Attempt to find a matching tile
        for (int tile_num = 1; tile_num < tiles.size(); tile_num++)
        {

            auto possible_tile = tiles[tile_num];

            if (possible_tile.is_connected())
            {
                continue;
            }

            cout << "Possible tile ID: " << possible_tile.id << endl;

            // Attempt to match the tile
            auto [matched, matched_tile] = find_possible_mach(tiles, possible_tile, dummy_tile);
            if (matched)
            {
                //tiles[possible_tile.container_pos] = matched_tile;
                //assert(possible_tile.container_pos == matched_tile.container_pos);
                cout << " Found a match" << endl;
                //printf("Tile with ID % adjacent to tile with ID %i", );

                // Copy position pointers

                // Update this tile
                matched_tile.top = dummy_tile.top;
                matched_tile.bottom = dummy_tile.bottom;
                matched_tile.left = dummy_tile.left;
                matched_tile.right = dummy_tile.right;

                // Update neighbors
                if (matched_tile.top != -1)
                {
                    tiles[matched_tile.top].bottom = matched_tile.container_pos;
                    printf("bottom of %i is %i \n", tiles[matched_tile.top].id, matched_tile.id);
                }
                if (matched_tile.bottom != -1)
                {
                    tiles[matched_tile.bottom].top = matched_tile.container_pos;
                    printf("top of %i is %i \n", tiles[matched_tile.bottom].id, matched_tile.id);
                }
                if (matched_tile.left != -1)
                {
                    tiles[matched_tile.left].right = matched_tile.container_pos;
                    printf("right of %i is %i \n", tiles[matched_tile.left].id, matched_tile.id);
                }
                if (matched_tile.right != -1)
                {
                    tiles[matched_tile.right].left = matched_tile.container_pos;
                    printf("left of %i is %i \n", tiles[matched_tile.right].id, matched_tile.id);
                }

                printf("Replaced dummy by tile (container_pos: %i ) with: \n top: %i\n bottom %i\n left %i\n right %i\n",
                       matched_tile.container_pos, matched_tile.top, matched_tile.bottom, matched_tile.left, matched_tile.right);

                // Copy pointers
                //remove_dummy_tile(*dummy_tile, matched_tile);
                dummy_left = Tile();
                if (matched_tile.set_left(dummy_left))
                {
                    dummies.push(dummy_left);
                };
                dummy_right = Tile();
                if (matched_tile.set_right(dummy_right))
                {
                    dummies.push(dummy_right);
                };
                dummy_top = Tile();
                if (matched_tile.set_top(dummy_top))
                {
                    dummies.push(dummy_top);
                };
                dummy_bottom = Tile();
                if (matched_tile.set_bottom(dummy_bottom))
                {
                    dummies.push(dummy_bottom);
                };
                //tiles.push(matched_tile);

                tiles[matched_tile.container_pos] = matched_tile;

                break;
            }
        }
    }

    answer = 1;

    for (auto tile : tiles)
    {
        int top_temp = min(tile.top, 0);
        int bottom_temp = min(tile.bottom, 0);
        int left_temp = min(tile.left, 0);
        int right_temp = min(tile.right, 0);
        int free_space = -(top_temp + bottom_temp + left_temp + right_temp);

        cout << tile.id << ". Free space: " << free_space << endl;
        printf("pos: %i \n top: %i\n bottom %i\n left %i\n right %i\n\n",
               tile.container_pos, tile.top, tile.bottom, tile.left, tile.right);

        if (top_temp + bottom_temp + left_temp + right_temp == -2)
        {
            //cout << tile.id << endl;
            answer = answer * (unsigned long long)tile.id;
        }
    }

    cout << answer << endl;
};