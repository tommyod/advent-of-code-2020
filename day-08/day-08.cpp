#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <stack>
#include <unordered_set>
#include <set>
#include <sstream>
#include <cassert>
#include <chrono>
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

// Parse a string such as 'jmp -266' -> ('jmp', -266)
pair<string, int> parse_command(const string command)
{
    string action = command.substr(0, 3);
    // stoi = string to integer
    int argument = stoi(command.substr(4, command.size() - 4));
    return pair<string, int>(action, argument);
}

int simulate(const vector<string> commands)
{

    /* ==================================================================
    ================== SIMULATE THE PROGRAM =================
    ================================================================== */

    int current_index = 0;
    int accumulator = 0;
    while (current_index != commands.size())
    {
        auto [action, argument] = parse_command(commands[current_index]);
        if (action == "nop")
        {
            current_index++;
        }
        else if (action == "acc")
        {
            accumulator += argument;
            current_index++;
        }
        else
        {
            current_index += argument;
        }
    }

    return accumulator;
}

int part1(const vector<string> &commands)
{

    set<int> seen;
    int accumulator = 0;
    int index = 0;
    while (true)
    {

        if (seen.contains(index))
            return accumulator;

        seen.insert(index);

        // Parse the command
        auto [action, argument] = parse_command(commands[index]);

        // Perform actions
        if (action == "nop")
            index++;
        else if (action == "jmp")
            index += argument;
        else if (action == "acc")
        {
            index++;
            accumulator += argument;
        }
    }
    return -1;
}

int part2(vector<string> commands)
{
    /* ==================================================================
       === CATEGORIZE EACH INDEX AS EITHER GOING TO CYCLE OR GOING OUT ==
       ================================================================== */

    unordered_set<int> cycles;
    unordered_set<int> paths_leading_out;

    // Loop over every starting point and see where it leads
    for (int start_index = 0; start_index < commands.size(); start_index++)
    {
        unordered_set<int> seen;
        int current_index = start_index;

        // Unravel paths from `start_index`
        while (true)
        {
            seen.insert(current_index);

            // Parse the command
            auto [action, argument] = parse_command(commands[current_index]);

            // NOP or ACC -> move one step ahead
            if (action == "nop" || action == "acc")
                current_index += 1;
            else if (action == "jmp")
                current_index += argument;

            // Completed a cycle - exists in "seen"
            if (seen.find(current_index) != seen.end())
            {
                cycles.insert(seen.begin(), seen.end());
                break;
            }

            // Got to the end
            if (current_index == commands.size())
            {
                paths_leading_out.insert(seen.begin(), seen.end());
                break;
            }

            // Got to a known cycle
            if (cycles.find(current_index) != cycles.end())
            {
                cycles.insert(seen.begin(), seen.end());
                break;
            }

            // Got to a known path leading out
            if (paths_leading_out.find(current_index) != paths_leading_out.end())
            {
                paths_leading_out.insert(seen.begin(), seen.end());
                break;
            }
        }
    }

    // Every index should be categorized by now
    assert(cycles.size() + paths_leading_out.size() == commands.size());

    /* ==================================================================
    ================== FIND OUT WHICH COMMAND WAS WRONG =================
    ================================================================== */

    int change_index;
    for (int start_index = 0; start_index < commands.size(); start_index++)
    {
        // Parse the command
        auto [action, argument] = parse_command(commands[start_index]);

        // Nothing wrong with 'acc' -> continue
        if (action == "acc")
            continue;

        int next_index;
        if (action == "jmp")
        {
            action = "nop";
            next_index = start_index + 1;
        }
        else
        {
            action = "jmp";
            next_index = start_index + argument;
        }

        // If the current index would lead to a cycle, but the next index when the command
        // was switched would sent us on a path leading out
        if ((cycles.find(start_index) != cycles.end()) && paths_leading_out.find(next_index) != paths_leading_out.end())
        {
            change_index = start_index;
            commands[start_index] = action + " " + to_string(argument);
            break;
        }
    }

    /* ==================================================================
    ================== SIMULATE THE PROGRAM =================
    ================================================================== */
    int accumulator = simulate(commands);
    return accumulator;
}

int main()
{
    // Read input data
    auto commands = read_file("day-08-input.txt");

    auto answer1 = part1(commands);
    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 1553);

    auto answer2 = part2(commands);
    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == 1877);
}