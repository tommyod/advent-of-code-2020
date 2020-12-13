#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <cassert>
#include <set>
#include <queue>
#include <regex>
#include <algorithm>
#include <tuple>

using namespace std;

// Read a file to a vector of strings
vector<long long> read_file(const string filename)
{
    vector<long long> lines = {};

    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            // String to long long
            lines.push_back(stoll(line));
        }
        file.close();
    }

    return lines;
}

template <typename T>
T part1(const vector<T> &numbers, T window_length)
{

    // Use a queue FIFO queue to keep track of what goes in and out
    // Use a set for fast lookups and iteration
    queue<T> number_queue;
    set<T> number_set;

    for (size_t i = 0; i < window_length; i++)
    {
        number_queue.push(numbers[i]);
        number_set.insert(numbers[i]);
    }

    for (size_t i = window_length; i < numbers.size(); i++)
    {

        // Check if the current number can be created
        bool can_be_written_as_sum = false;
        for (auto previous_number : number_set)
        {
            if (number_set.contains(numbers[i] - previous_number))
            {
                can_be_written_as_sum = true;
                break;
            }
        }

        if (!can_be_written_as_sum)
        {
            return numbers[i];
        }

        // Remove
        auto to_remove = number_queue.front();
        number_queue.pop();
        number_set.erase(to_remove);

        // Add
        number_queue.push(numbers[i]);
        number_set.insert(numbers[i]);
    }

    return -1;
}

template <typename T>
T part2(const vector<T> &numbers, const T target)
{

    for (size_t i = 0; i < numbers.size(); i++)
    {
        // Keep track of sum, smallest and largest in range
        T the_sum = numbers[i];
        T smallest = numbers[i];
        T largest = numbers[i];

        // At least two numbers, so start one ahead and update immediately
        for (size_t j = i + 1; j < numbers.size(); j++)
        {

            // Update values
            the_sum += numbers[j];
            smallest = min(smallest, numbers[j]);
            largest = max(largest, numbers[j]);

            // Found it
            if (the_sum == target)
            {
                return smallest + largest;
            }

            if (the_sum > target)
                break;
        }

        /* code */
    }
    return -1;
}

int main()
{
    // Read input data
    auto numbers = read_file("day-09-input.txt");

    auto answer1 = part1<long long>(numbers, 25);
    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 248131121);

    auto answer2 = part2(numbers, answer1);
    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == 31580383);
}