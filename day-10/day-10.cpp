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
vector<int> read_file(const string filename)
{
    vector<int> lines = {0};

    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            lines.push_back(stoi(line));
        }
        file.close();
    }

    std::sort(lines.begin(), lines.end());
    lines.push_back(lines[lines.size() - 1] + 3);

    return lines;
}

int part1(const vector<int> &lines)
{
    int diffs_one = 0;
    int diffs_three = 0;
    int diff;

    for (size_t i = 1; i < lines.size(); i++)
    {
        diff = lines[i] - lines[i - 1];
        if (diff == 1)
            diffs_one++;
        if (diff == 3)
            diffs_three++;
    }

    return diffs_one * diffs_three;
}

unsigned long long part2(const vector<int> &lines)
{

    int answer = 0;

    // Create a set for fast lookups
    set<int> connectors(lines.begin(), lines.end());

    // Create a DP table "connectors ending in"
    // with connections_ending_in[i] = number of sequences ending in i
    auto max_element_pointer = max_element(lines.begin(), lines.end());
    vector<unsigned long long> connections_ending_in(*max_element_pointer + 1, 0);

    // Initial condition
    connections_ending_in[0] = 1;

    // Perform the DP loop
    for (int i = 1; i <= connections_ending_in.size(); i++)
    {
        // If the connector is not available, skip it
        if (!connectors.contains(i))
        {
            continue;
        }

        // If the connector is available, the number of sequences ending in i
        // consists of the sum of the previous 3 available sequences ending in i-1, i-2, i-3
        for (int j = max(0, i - 3); j < i; j++)
        {
            if (connectors.contains(j))
            {
                connections_ending_in[i] += connections_ending_in[j];
            }
        }
    }

    // Return the answer, using unsigned long long (largest integer type in C++)
    return connections_ending_in[connections_ending_in.size() - 1];
}

int main()
{
    // Read input data
    auto lines = read_file("day-10-input.txt");

    auto answer1 = part1(lines);
    cout << "Answer to part 1: " << answer1 << endl; // 2059

    auto answer2 = part2(lines);
    cout << "Answer to part 2: " << answer2 << endl; //
}