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

// Read a file to a vector of strings
vector<string> read_file(const string filename)
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

// Create a cartesian product, e.g. cp({0, 1, 2}, {8, 9}) -> {{0, 8}, {0, 9}, {1, 8}, ...}
vector<vector<int>> cartesian_product(const vector<vector<int>> &dimensions)
{

    vector<vector<int>> answer;

    // Base case
    if (dimensions.size() == 1)
    {
        for (auto &element : dimensions[0])
        {
            answer.push_back({element});
        }
        return answer;
    }

    // General case with recursion - split into first and rest
    vector<int> first = dimensions[0];
    vector<vector<int>> rest(dimensions.begin() + 1, dimensions.end());

    // Combine the first element with the cartesian products of the rest
    for (auto &element : first)
    {
        for (auto &remaining : cartesian_product(rest))
        {

            vector<int> ans{element};
            for (auto &r : remaining)
            {
                ans.push_back(r);
            }
            answer.push_back(ans);
        }
    }

    return answer;
}

// Create neighbor indices from a multidimensional index
vector<vector<int>> neighbors(vector<int> indices)
{

    vector<vector<int>> answer;
    vector<vector<int>> cartesian_args;

    for (int i = 0; i < indices.size(); i++)
    {
        int index = indices[i];
        cartesian_args.push_back({index - 1, index, index + 1});
    }

    for (auto possible_inds : cartesian_product(cartesian_args))
    {
        // Only neighbors, so skip the index matching the input arg
        if (possible_inds == indices)
            continue;

        answer.push_back(possible_inds);
    }

    return answer;
}

// Solve the general problem
int solve(const vector<string> lines, int dims)
{

    // Using a mapping to represent an n-dimensional array
    map<vector<int>, int> cube;

    // Populate the cube with the initial values
    for (int row = 0; row < lines.size(); row++)
    {
        for (int column = 0; column < lines.size(); column++)
        {
            if (lines[row][column] == '#')
            {
                // Add empty dimensions
                vector<int> arg{row, column};
                for (int i = 0; i < (dims - 2); i++)
                {
                    arg.push_back(0);
                }
                assert(arg.size() == dims);

                cube[arg] = 1;
            }
        }
    }

    // For each iteration
    for (int iteration = 0; iteration < 6; iteration++)
    {

        // Find all indices to loop
        // Here we must include neighbors of active states,
        // since the active state can propagate over to them too
        set<vector<int>> indices_to_loop;
        for (auto [index, element] : cube)
        {
            indices_to_loop.insert(index);
            for (auto neighbor_index : neighbors(index))
            {
                indices_to_loop.insert(neighbor_index);
            }
        }

        // Copy the old cube to a new cube
        map<vector<int>, int> new_cube = cube;

        // For every index that could possibly be affected by an update
        for (auto index : indices_to_loop)
        {

            int element;
            if (cube.contains(index) && (cube[index] == 1))
            {
                element = 1;
            }
            else
            {
                element = 0;
            }

            // Compute the number of active neighbors
            int active_neighbors = 0;
            for (auto neighbor_index : neighbors(index))
            {
                if (!cube.contains(neighbor_index))
                    continue;
                active_neighbors += cube[neighbor_index];
            }

            // printf("(%i, %i, %i) has %i active neighbors\n", index[0], index[1], index[2], active_neighbors);

            // If a cube is active and exactly 2 or 3 of its neighbors are also active,
            // the cube remains active. Otherwise, the cube becomes inactive.
            if ((element == 1) && ((active_neighbors == 2) || (active_neighbors == 3)))
            {
                new_cube[index] = 1;
            }
            else if (element == 1)
            {
                new_cube[index] = 0;
            }
            // If a cube is inactive but exactly 3 of its neighbors are active,
            // the cube becomes active. Otherwise, the cube remains inactive.
            else if ((element == 0) && (active_neighbors == 3))
            {
                new_cube[index] = 1;
            }
            else if ((element == 0))
            {
                new_cube[index] = 0;
            }
        }

        // Update priori to the next iteration
        cube = new_cube;
    }

    // Compute the answer - the number of active indices
    int answer = 0;
    for (auto [key, value] : cube)
    {
        answer += value;
    }

    return answer;
}

int main()
{
    // Read input data
    auto lines = read_file("day-17-input.txt");

    auto answer1 = solve(lines, 3);
    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 380);

    auto answer2 = solve(lines, 4);
    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == 2332);
}