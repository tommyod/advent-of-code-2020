#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <cassert>
#include <set>
#include <unordered_set>
#include <queue>
#include <numeric>
#include <regex>
#include <algorithm>
#include <tuple>
#include <iterator>
#include <list>
#include <cmath>
#include <unistd.h>
#include <cstdlib>
#include <unordered_map>

using namespace std;

// Read a file to a vector of strings
vector<string>
read_file(const string filename)
{
    vector<string> lines;
    fstream inputFile(filename, fstream::in);
    string file_line;

    // While there is data to read
    while (inputFile && getline(inputFile, file_line))
    {
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

    // No delimeter found
    if (delta == string::npos)
    {
        return {line};
    }

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

pair<map<int, string>, vector<string>> parse_lines(const vector<string> &lines)
{

    map<int, string> rules;
    vector<string> inputs;

    for (auto line : lines)
    {
        // Parse rules
        if (line.find(":") != string::npos)
        {
            int i = line.find(":");
            rules[stoi(line.substr(0, i))] = line.substr(i + 2, line.size() - i - 2);
        }
        else
        {
            // Parse inputs
            inputs.push_back(line);
        }
    }
    return make_pair(rules, inputs);
}

string remove_character(string input, char character)
{
    string out = "";
    for (auto this_char : input)
    {
        if (this_char != character)
        {
            out = out + this_char;
        }
    }
    return out;
}

int part1(map<int, string> rules, vector<string> inputs)
{
    auto rule = rules[0];
    string new_rule = "";
    regex pattern{R"(\d+)"};
    smatch match;

    // Create the regex string
    while (regex_search(rule, match, pattern))
    {
        string before = rule.substr(0, match.position());
        string after = rule.substr(match.position() + match.length(),
                                   rule.size() - (match.position() + match.length()));
        rule = before + "(" + rules[stoi(match.str())] + ")" + after;
    }

    // Parse the regex string
    rule = remove_character(rule, '"');
    rule = remove_character(rule, ' ');

    pattern = regex("^" + rule + "$"); // Must match start and end of line

    // Count matches
    int matched = 0;
    for (auto input : inputs)
    {
        smatch match;
        if (regex_search(input, match, pattern))
        {
            matched++;
        }
    }
    return matched;
}

int part2(map<int, string> rules, vector<string> inputs)
{
    auto rule = rules[0];
    regex pattern{R"(\d+)"};

    // Expanding 14 times is dumb luck
    // It provides the correct answer, but if I change to 15 I get the following error:
    // "Number of NFA states exceeds limit. Please use shorter regex string, or
    // use smaller brace expression, or make _GLIBCXX_REGEX_STATE_LIMIT larger."
    for (int expansion = 0; expansion < 14; expansion++)
    {

        string new_rule = "";
        auto search = sregex_iterator(rule.begin(), rule.end(), pattern);

        // Gather all the rules into parts, interleaving expanded and non-expanded strings
        vector<string> parts;

        int start = 0;
        for (sregex_iterator p(rule.begin(), rule.end(), pattern); p != sregex_iterator{}; ++p)
        {
            auto match = (*p);
            int end = match.position();
            parts.push_back(rule.substr(start, end - start));
            parts.push_back("(" + rules[stoi(match.str())] + ")");
            start = match.position() + match.length();
        }
        parts.push_back(rule.substr(start, rule.size() - start));

        // Collect the vector back into a string
        for (auto part : parts)
        {
            new_rule = new_rule + part;
        }
        rule = new_rule;
    }

    // Parse the regex string
    rule = remove_character(rule, '"');
    rule = remove_character(rule, ' ');

    pattern = regex("^" + rule + "$"); // Must match start and end of line

    // Count matches
    int matched = 0;
    for (auto input : inputs)
    {
        smatch match;
        if (regex_search(input, match, pattern))
        {
            matched++;
        }
    }
    return matched;
}

int main()
{

    // Read the lines from the file
    auto lines = read_file("day-19-input.txt");
    auto [rules, inputs] = parse_lines(lines);
    // Solve the first part of the problem
    auto answer1 = part1(rules, inputs);
    cout << "Answer to part 1: " << answer1 << endl;
    // assert(answer1 == 230);

    // Solve the second part of the problem
    auto lines2 = read_file("day-19-input-2.txt");
    auto [rules2, inputs2] = parse_lines(lines2);
    auto answer2 = part2(rules2, inputs2);
    cout << "Answer to part 2: " << answer2 << endl;
};