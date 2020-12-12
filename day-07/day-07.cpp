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
#include <deque>

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

// Parse each line to a tuple
tuple<string, map<string, int>> parse_line(const string &line)
{

    // example: 'light red bags contain 1 bright white bag, 2 muted yellow bags.'
    int i = line.find("contain");
    string parent = line.substr(0, i - 6);
    string rest = line.substr(i + 8, line.size() - i - 9);

    // No second part of the string, return immediately
    if (rest == "no other bags")
    {
        return {parent, {}};
    }

    // Loop through every "child" bag
    map<string, int> children;

    while (rest.find(", ") != string::npos)
    {

        // Example: '1 bright white bag(s)'
        string bag_description = rest.substr(0, rest.find(", "));
        int j = bag_description.find(" ");

        // Get the number of bags, then get the bag color
        int bag_number = stoi(bag_description.substr(0, i));
        string bag_color = bag_description.substr(j + 1, bag_description.size() - j - 1);
        bag_color = bag_color.substr(0, bag_color.rfind(" "));

        // Add to the mapping, e.g. yellow -> 3
        children[bag_color] = bag_number;
        rest = rest.substr(rest.find(", ") + 2, rest.size() - rest.find(", ") - 2);
    }

    // Final item
    string bag_description = rest.substr(0, rest.find(", "));
    int j = bag_description.find(" ");
    int bag_number = stoi(bag_description.substr(0, i));
    string bag_color = bag_description.substr(j + 1, bag_description.size() - j - 1);
    bag_color = bag_color.substr(0, bag_color.rfind(" "));
    children[bag_color] = bag_number;

    return {parent, children};
}

// Parse the lines in the input
map<string, map<string, int>> parse_lines(const vector<string> &lines)
{
    map<string, map<string, int>> parsed;
    for (auto line : lines)
    {
        auto [parent, children] = parse_line(line);
        parsed[parent] = children;
    }
    return parsed;
}

// Graph is a mapping [parent] -> children.
map<string, set<string>> reverse_graph(map<string, set<string>> &graph)
{
    map<string, set<string>> reversed;
    // For each child, create a mapping to parents
    for (auto [parent, children] : graph)
    {
        for (auto child : children)
        {
            if (reversed.contains(child))
            {
                reversed[child].insert(parent);
            }
            else
            {
                reversed[child] = {parent};
            }
        }
    }

    return reversed;
}

// Solve part 1 of the problem
int part1(const vector<string> &lines)
{
    auto graph = parse_lines(lines);

    // For part 1 we do not need the counts
    map<string, set<string>> graph_no_counts;
    for (auto [parent, children] : graph)
    {
        for (auto [child, count] : children)
        {

            if (graph_no_counts.contains(parent))
            {
                graph_no_counts[parent].insert(child);
            }
            else
            {
                graph_no_counts[parent] = {child};
            }
        }
    }

    // Solve the problem using DFS
    auto reversed_graph = reverse_graph(graph_no_counts);
    set<string> seen;
    deque<string> bfs_queue;
    bfs_queue.push_back("shiny gold");

    int answer = -1; // Do not count the start node

    while (!bfs_queue.empty())
    {

        string item = bfs_queue.front();
        bfs_queue.pop_front();

        if (seen.contains(item))
        {
            continue;
        }

        // Increment counter and set of seen nodes
        seen.insert(item);
        answer++;

        // Add next layer of nodes to the queue
        for (auto parent : reversed_graph[item])
            bfs_queue.push_back(parent);
    }

    return answer;
}

// Compute how many bags need to be included, using recursive depth first search
int bags_including(const string bag_name, map<string, map<string, int>> &graph)
{
    int answer = 1; // This bag must be included

    // Loop through every child bag
    for (auto [child, count] : graph[bag_name])
    {
        // Must include `count` of the child bag, plus every bag required for that bag
        answer += count * bags_including(child, graph);
    }
    return answer;
}

// Solve part 1
int part2(const vector<string> &lines)
{

    auto graph = parse_lines(lines);
    string query = "shiny gold";
    return bags_including(query, graph) - 1;
}

int main()
{
    // Read input data
    auto lines = read_file("day-07-input.txt");

    auto answer1 = part1(lines);
    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 126);

    auto answer2 = part2(lines);
    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == 220149);
}