#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <tuple>

using namespace std;

// Read a file to a matrix of strings
vector<vector<char>> read_file(const string filename)
{
    vector<vector<char>> lines;
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            vector<char> row(line.begin(), line.end());
            lines.push_back(row);
        }
        file.close();
    }
    return lines;
}

// Compare two matrices (vectors of vectors)
template <typename T>
bool matrices_equal(const vector<vector<T>> &matrix1, const vector<vector<T>> &matrix2)
{

    // Get sizes
    int nrows1 = matrix1.size();
    int ncols1 = matrix1[0].size();
    int nrows2 = matrix2.size();
    int ncols2 = matrix2[0].size();

    // If sizes do not match, the matrices are not equal
    if ((nrows1 != nrows2) || (ncols1 != ncols2))
        return false;

    // Check if every entry is equal
    for (int row = 0; row < nrows1; row++)
    {
        for (int col = 0; col < ncols1; col++)
        {
            // The .at() method will fail if we go out-of-bounds
            if (matrix1.at(row).at(col) != matrix2.at(row).at(col))
                return false;
        }
    }
    // No failures
    return true;
}

template <typename T>
int count_elements(const vector<vector<T>> &matrix, const T item)
{

    int counter = 0;
    int nrows = matrix.size();
    int ncols = matrix[0].size();

    for (int row = 0; row < nrows; row++)
    {
        for (int col = 0; col < ncols; col++)
        {
            if (matrix[row][col] == item)
                counter++;
        }
    }
    return counter;
}

vector<vector<char>> iteration(const vector<vector<char>> &matrix,
                               int max_step = 1,
                               int occupied_before_free = 4)
{

    vector<vector<char>> new_matrix;
    new_matrix = matrix;
    int nrows = matrix.size();
    int ncols = matrix[0].size();
    vector<tuple<int, int>> dxdys = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    for (int row = 0; row < nrows; row++)
    {
        for (int col = 0; col < ncols; col++)
        {

            // If a seat is empty (L) and there are no occupied seats
            // adjacent to it, the seat becomes occupied.

            // If a seat is occupied (#) and four or more seats adjacent to
            // it are also occupied, the seat becomes empty.
            int occupied_seats = 0;

            // Search directions
            for (auto [dir_row, dir_col] : dxdys)
            {
                // Step lengths k
                int k = 1;
                while (k <= max_step)
                {

                    // Break if the search goes out of bounds
                    if ((row + k * dir_row < 0) || (col + k * dir_col < 0) ||
                        (row + k * dir_row >= nrows) || (col + k * dir_col >= ncols))
                    {
                        break;
                    }

                    char entry = matrix[row + k * dir_row][col + k * dir_col];

                    if (entry == '#')
                        occupied_seats++;
                    if ((entry == '#') || (entry == 'L'))
                        break;

                    k++;
                }
            }

            if ((matrix.at(row).at(col) == 'L') && (occupied_seats == 0))
            {
                new_matrix[row][col] = '#';
            }

            if ((matrix.at(row).at(col) == '#') && (occupied_seats >= occupied_before_free))
            {
                new_matrix[row][col] = 'L';
            }
        }
    }

    return new_matrix;
}

// Solve given a board and a rule for obtaining the next board
int solve(vector<vector<char>> board, int max_step = 1,
          int occupied_before_free = 4)
{
    auto board_next = iteration(board, max_step, occupied_before_free);

    while (!matrices_equal(board, board_next))
    {
        board = board_next;
        board_next = iteration(board, max_step, occupied_before_free);
    }
    return count_elements(board, '#');
}

int main()
{
    // Read input data
    auto lines = read_file("day-11-input.txt");

    auto answer1 = solve(lines, 1, 4); // 1 step, if 4 or more are occupied
    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 2265);

    auto answer2 = solve(lines, 999999, 5); // many steps, if 5 or more are occupied
    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == 2045);
}