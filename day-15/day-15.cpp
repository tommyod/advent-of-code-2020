#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <cassert>
#include <set>
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
    while (inputFile)
    {
        getline(inputFile, file_line, '\n');
        if (file_line.size())
        {
            lines.push_back(file_line);
        }
    }
    inputFile.close();
    return lines;
}

vector<string> split_string(const string &line, const string delim)
{
    int start = 0;
    int delta = line.substr(start, line.size() - start).find(delim);
    vector<string> output;

    do
    {
        output.push_back(line.substr(start, delta));
        // Update
        start += delta + delim.size();
        delta = line.substr(start, start - line.size()).find(delim);

    } while (delta != string::npos);

    // Last item
    output.push_back(line.substr(start, start - line.size()));
    return output;
}

long solve(vector<long> numbers_init, long goal)
{

    vector<long> numbers(goal);
    unordered_map<long, long> last_seen;
    set<long> seen;

    // Set up the first numbers
    for (long i = 0; i < numbers_init.size(); i++)
    {
        numbers[i] = numbers_init[i];
        last_seen[numbers[i]] = i;
        if (i != 0)
        {
            seen.insert(numbers[i - 1]);
        }
    }

    // Generate numbers
    for (long i = numbers_init.size(); i < goal; i++)
    {

        long previous_number = numbers[i - 1];
        long this_number;

        // If that was the first time the number has been spoken, the current player says 0.
        if (!seen.contains(previous_number))
        {
            this_number = 0;
            seen.insert(previous_number);
        }

        // Otherwise, the number had been spoken before; the current player announces
        // how many turns apart the number is from when it was previously spoken.
        else
        {
            this_number = i - 1 - last_seen[previous_number];
        }

        // Update array as well as last seen time of previous number
        numbers[i] = this_number;
        last_seen[previous_number] = i - 1;
    }

    return numbers[goal - 1];
}

int main()
{
    // Read input data
    auto lines = read_file("day-15-input.txt");
    auto numbers_as_string = split_string(lines[0], ",");
    vector<long> numbers;
    for (auto number : numbers_as_string)
    {
        numbers.push_back(stol(number));
    }

    auto answer1 = solve(numbers, 2020);
    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 595);

    auto answer2 = solve(numbers, 30000000);
    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == 1708310);
}