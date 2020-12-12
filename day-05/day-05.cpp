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

int row_id_from_string(const string &row_string)
{
    string binary_string = "";
    for (auto character : row_string)
    {
        if ((character == 'B') || (character == 'R'))
        {
            binary_string = binary_string + "1";
        }
        else
        {
            binary_string = binary_string + "0";
        }
    }
    // Convert binary string to integer, base 2
    return stoi(binary_string, nullptr, 2);
}

// Read a file to a vector of strings
vector<string> read_file(const string filename)
{
    vector<string> lines;

    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();
    }

    return lines;
}

// Convert e.g. 'FBFFFFFRLL' to an int, then compare
static bool compare_lines(const string &a, const string &b)
{
    return (row_id_from_string(a) < row_id_from_string(b));
}

int part1(const vector<string> &lines)
{
    // The result is an iterator
    auto result = max_element(lines.begin(), lines.end(), compare_lines);
    // Get the index of the max values
    int index = distance(lines.begin(), result);
    // Return the ID from the string
    return row_id_from_string(lines[index]);
}

int part2(const vector<string> &lines)
{

    // Create a set of all IDs from the lines
    set<int> all_ids;
    for (auto line : lines)
    {
        all_ids.insert(row_id_from_string(line));
    }

    // Get pointers to minimum and maximum elements in the set of IDs
    const auto [min, max] = minmax_element(begin(all_ids), end(all_ids));

    // Loop from [min, ..., max] and perform the check
    for (int id = *min; id <= *max; id++)
    {
        // ID does not exist, but one lower and one higher exists
        if ((!all_ids.contains(id)) && all_ids.contains(id - 1) && all_ids.contains(id + 1))
        {
            return id;
        }
    }

    return 0;
}

int main()
{
    // Read input data
    auto lines = read_file("day-05-input.txt");

    int answer1 = part1(lines);
    cout << "Answer to part 1: " << answer1 << endl;

    int answer2 = part2(lines);
    cout << "Answer to part 2: " << answer2 << endl;
}