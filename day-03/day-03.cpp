#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <cassert>
#include <set>
#include <regex>
#include <algorithm>
#include <tuple>

using namespace std;

// Read a file to a vector of strings
vector<vector<bool>> read_file(const string filename)
{
    vector<vector<bool>> matrix;

    ifstream file(filename);
    if (file.is_open())
    {
        string line;

        while (getline(file, line))
        {
            vector<bool> row;
            for (auto character : line)
            {
                if (character == '.')
                {
                    row.push_back(false);
                }
                else
                {
                    row.push_back(true);
                }
            }

            matrix.push_back(row);
        }
        file.close();
    }

    return matrix;
}

bool is_valid(const string &password, const string letter, const int low, const int high)
{

    // The iteration returns chars
    // .c_str() returns a pointer, so we have to reference the memory adress with *
    int num_chars = count_if(password.begin(), password.end(),
                             [&](const char i) { return i == *letter.c_str(); });

    return ((num_chars >= low) && (num_chars <= high));
}

int part1(const vector<vector<bool>> &matrix, int delta_rows, int delta_cols)
{

    // Initialize variables
    int row = 0;
    int column = 0;
    int num_trees = 0;
    int nrows = matrix.size();
    int ncolumns = matrix[0].size();

    // Loop over and count
    while (row < nrows)
    {
        num_trees += (int)matrix[row][column];
        row = row + delta_rows;
        column = (column + delta_cols) % ncolumns;
    }

    return num_trees;
}

long part2(const vector<vector<bool>> &matrix, const vector<tuple<int, int>> &to_check)
{

    // We get the answer to part 2 by calling the part 1 function and multiplying results
    long answer = 1;
    for (auto deltas : to_check)
    {
        // Use get<i>(tuple_arg) to get elements in a tuple
        answer = answer * (long)part1(matrix, get<0>(deltas), get<1>(deltas));
    }

    return answer;
}

int main()
{

    // Read input data
    auto matrix = read_file("day-03-input.txt");

    int answer1 = part1(matrix, 1, 3);
    cout << "Answer to part 1: " << answer1 << endl;

    /*
    Right 1, down 1.
    Right 3, down 1. (This is the slope you already checked.)
    Right 5, down 1.
    Right 7, down 1.
    Right 1, down 2.
    */
    vector<tuple<int, int>> to_check = {{1, 1}, {1, 3}, {1, 5}, {1, 7}, {2, 1}};
    auto answer2 = part2(matrix, to_check);
    cout << "Answer to part 2: " << answer2 << endl;
}