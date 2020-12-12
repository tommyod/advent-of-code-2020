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

// Return a mapping for each group
// The mappings consist of counts, e.g. {'a': 3, ...}
vector<map<char, int>> lines_to_maps(const vector<string> lines)
{

    vector<map<char, int>> maps;

    map<char, int> this_map;

    for (auto line : lines)
    {
        if (line == "")
        {
            maps.push_back(this_map);
            this_map.clear();
            continue;
        }

        for (auto character : line)
        {
            if (this_map.contains(character))
            {
                this_map[character] += 1;
            }
            else
            {
                this_map[character] = 1;
            }
        }
    }

    // Final map
    maps.push_back(this_map);
    return maps;
}

// Return one set for each person
vector<vector<set<char>>> lines_to_sets(const vector<string> lines)
{

    vector<vector<set<char>>> groups;
    vector<set<char>> this_group;

    for (auto line : lines)
    {
        if (line == "")
        {
            groups.push_back(this_group);
            this_group.clear();
            continue;
        }

        set<char> answers(line.begin(), line.end());
        this_group.push_back(answers);
    }

    // Final group
    groups.push_back(this_group);
    return groups;
}

int part1(const vector<string> &lines)
{
    auto maps = lines_to_maps(lines);

    int answer = 0;
    for (auto mapping : maps)
    {
        // Anyone answered yes
        answer += mapping.size();
    }

    return answer;
}

int part2(const vector<string> &lines)
{
    auto groups = lines_to_sets(lines);
    int answer = 0;

    // Within each group of people
    for (auto group : groups)
    {

        // Get a set of all questions within this group
        set<int> questions;
        for (auto person : group)
        {
            for (auto answer : person)
            {
                questions.insert(answer);
            }
        }

        // For every question asked, check if everyone answered yes
        for (auto question : questions)
        {
            bool all_answered = true;

            for (auto person : group)
            {
                if (!person.contains(question))
                {
                    all_answered = false;
                    break;
                }
            }

            // Increment the counter with bool flag
            answer += (int)all_answered;
        }
    }

    return answer;
}

int main()
{
    // Read input data
    auto lines = read_file("day-06-input.txt");

    int answer1 = part1(lines);
    cout << "Answer to part 1: " << answer1 << endl;

    int answer2 = part2(lines);
    cout << "Answer to part 2: " << answer2 << endl;
}