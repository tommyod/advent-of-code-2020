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

using namespace std;

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

bool is_valid(const string &password, const string letter, const int low, const int high)
{

    // The iteration returns chars
    // .c_str() returns a pointer, so we have to reference the memory adress with *
    int num_chars = count_if(password.begin(), password.end(),
                             [&](const char i) { return i == *letter.c_str(); });

    return ((num_chars >= low) && (num_chars <= high));
}

int part1(const vector<string> &lines)
{
    int valid_passwords = 0;
    int low, high;
    string letter, password;

    for (auto line : lines)
    {
        // Extract information from the line
        int i = line.find("-");
        int j = line.find(" ");
        int k = line.find(": ") + 1;
        low = stoi(line.substr(0, i));
        high = stoi(line.substr(i + 1, j - i));
        letter = line.substr(j + 1, 1);
        password = line.substr(k + 1, line.size() - k - 1);

        // Check if the password is valid
        if (is_valid(password, letter, low, high))
        {
            valid_passwords++;
        }
    }

    return valid_passwords;
}

int part2(const vector<string> &lines)
{
    int valid_passwords = 0;
    int low, high;
    string letter, password;

    for (auto line : lines)
    {
        // Extract information from the line
        int i = line.find("-");
        int j = line.find(" ");
        int k = line.find(": ") + 1;
        low = stoi(line.substr(0, i));
        high = stoi(line.substr(i + 1, j - i));
        letter = line.substr(j + 1, 1);
        password = line.substr(k + 1, line.size() - k - 1);

        // Exactly one must match, but not both: XOR operator is ^ in C++
        if ((password[low - 1] == *letter.c_str()) ^ (password[high - 1] == *letter.c_str()))
        {
            valid_passwords++;
        }
    }

    return valid_passwords;
}

int main()
{

    // Read input data
    auto lines = read_file("day-02-input.txt");

    int answer1 = part1(lines);
    cout << "Answer to part 1: " << answer1 << endl;

    int answer2 = part2(lines);
    cout << "Answer to part 2: " << answer2 << endl;
}