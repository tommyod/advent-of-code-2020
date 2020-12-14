#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <tuple>
#include <bitset>
#include <map>

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

// Masking as described in part 1 of the problem
string apply_mask(const string &binary, const string mask)
{
    assert(binary.size() == mask.size());
    string answer = "";
    for (size_t i = 0; i < binary.size(); i++)
    {
        if (mask[i] == 'X')
            answer += binary[i];
        else
            answer += mask[i];
    }
    return answer;
}

string apply_mask_floating(const string &binary, const string mask)
{
    assert(binary.size() == mask.size());
    string answer = "";
    for (size_t i = 0; i < binary.size(); i++)
    {
        if (mask[i] == 'X')
            answer = answer + "X";
        else if (mask[i] == '1')
            answer = answer + "1";
        else
            answer = answer + binary[i];
    }
    return answer;
}

template <typename T>
T part1(const vector<string> lines)
{

    string mask;
    map<T, T> memory;

    for (auto line : lines)
    {
        // mask = 00110X11X0000110X0000001000111010X00
        if (line.starts_with("mask"))
        {
            mask = line.substr(7, line.size() - 7);
            continue;
        }
        string memory_key_str = line.substr(0, line.find("=") - 1);
        T memory_key = stol(memory_key_str.substr(4, memory_key_str.size() - 5));
        T memory_value = stol(line.substr(line.find("=") + 2, line.size() - line.find("=") - 2));

        // Convert value to binary
        string memory_value_binary_string = bitset<36>(memory_value).to_string();
        memory[memory_key] = stol(apply_mask(memory_value_binary_string, mask), nullptr, 2);
    }

    // Sum every every value in the mapping and return
    return accumulate(memory.begin(), memory.end(), (T)0,
                      [](T initial, map<T, T>::value_type &next) {
                          return initial + next.second;
                      });
}

// combinations("0X1X") -> 0010, 0011, 0110, 0111
// Recursive algorithm
vector<string> combinations(const string &address)
{

    vector<string> results = {};

    for (int i = 0; i < address.size(); i++)
    {
        if (address[i] != 'X')
            continue;

        // Split 000X11101X1 into: 000, 11101X1
        string first = address.substr(0, max(0, i));
        string rest = address.substr(i + 1, address.size() - i - 1);

        for (auto tail : combinations(first + "0" + rest))
        {
            results.push_back(tail);
        }
        for (auto tail : combinations(first + "1" + rest))
        {
            results.push_back(tail);
        }
        break; // Once the first X is dealt with, the recursion handles the rest
    }

    // Base case
    if (results.size() == 0)
    {
        results.push_back(address);
    }

    return results;
}

template <typename T>
T part2(const vector<string> lines)
{

    string mask;
    map<T, T> memory;

    for (auto line : lines)
    {
        // mask = 00110X11X0000110X0000001000111010X00
        if (line.starts_with("mask"))
        {
            mask = line.substr(7, line.size() - 7);
            continue;
        }

        string memory_key_str = line.substr(0, line.find("=") - 1);
        T memory_key = stol((memory_key_str.substr(4, memory_key_str.size() - 5)));
        string memory_key_binary_string = bitset<36>(memory_key).to_string();
        auto memory_keys_binary_strings = combinations(apply_mask_floating(memory_key_binary_string, mask));

        T memory_value = stol(line.substr(line.find("=") + 2, line.size() - line.find("=") - 2));

        for (auto memory_key : memory_keys_binary_strings)
        {
            memory[stol(memory_key, nullptr, 2)] = memory_value;
        }
    }

    // Sum every every value in the mapping and return
    return accumulate(memory.begin(), memory.end(), (T)0,
                      [](T initial, map<T, T>::value_type &next) {
                          return initial + next.second;
                      });
}

int main()
{
    // Read input data
    auto lines = read_file("day-14-input.txt");

    auto answer1 = part1<long>(lines);
    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 7997531787333);

    auto answer2 = part2<long>(lines);
    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == 3564822193820);
}