#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <cassert>
#include <set>

using namespace std;

// Read a file to a vector of strings
vector<int> read_file(const string filename)
{
    vector<int> lines;
    fstream input_file(filename, fstream::in);
    string file_line;

    // While there is data to read
    while (input_file)
    {
        getline(input_file, file_line, '\n');
        if (file_line.size() > 0)
        {
            lines.push_back(stoi(file_line));
        }
    }
    input_file.close();
    return lines;
}

int part1(const vector<int> &numbers, const set<int> &numbers_set)
{
    int goal = 2020;
    int answer = 0;
    for (int number : numbers)
    {
        // The number is in the set
        assert(numbers_set.contains(number));

        // If (2020 - number) is in the set
        if (numbers_set.contains(goal - number))
        {
            answer = number * (goal - number);
        }
    }
    return answer;
}

int part2(const vector<int> &numbers, const set<int> &numbers_set)
{
    int goal = 2020;
    int answer, number1, number2;

    for (int i = 0; i < numbers.size(); i++)
    {
        for (int j = i + 1; j < numbers.size(); j++)
        {
            number1 = numbers[i];
            number2 = numbers[j];

            // If (2020 - number1 - number2) is in the set
            if (numbers_set.contains(goal - number1 - number2))
            {
                answer = number1 * number2 * (goal - number1 - number2);
            }
        }
    }

    return answer;
}

int main()
{

    // Read input data
    vector<int> numbers = read_file("day-01-input.txt");
    set<int> numbers_set(numbers.begin(), numbers.end());

    int answer1 = part1(numbers, numbers_set);
    cout << "Answer to part 1: " << answer1 << endl;

    int answer2 = part2(numbers, numbers_set);
    cout << "Answer to part 2: " << answer2 << endl;
}