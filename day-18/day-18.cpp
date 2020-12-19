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

// Indices of a character in a string
vector<int> indices_matching(string problem, char delim)
{
    vector<int> answer;
    for (int i = 0; i < problem.size(); i++)
    {
        if (problem[i] == delim)
        {
            answer.push_back(i);
        }
    }
    return answer;
}

// Solve part 1 of the problem without parentheses
string solve_no_parenthesis(string problem)
{

    auto space_inds = indices_matching(problem, ' ');
    assert((space_inds.size() == 0) || (space_inds.size() >= 2));

    if ((space_inds.size() == 0))
    {
        return problem;
    }

    // Base case, e.g. "11 + 22"
    if (space_inds.size() == 2)
    {
        string expr = problem.substr(space_inds[0] + 1, 1);
        unsigned long long a = stoull(problem.substr(0, space_inds[0]));
        unsigned long long b = stoull(problem.substr(space_inds[1], problem.size() - space_inds[1]));
        // cout << a << expr << b << endl;
        if (expr == "+")
        {
            return to_string(a + b);
        }
        if (expr == "*")
        {
            return to_string(a * b);
        }
    }

    // General case, e.g. "1 + 2 + 3"
    string first_part = problem.substr(0, space_inds[2]);
    string rest_part = problem.substr(space_inds[2], problem.size() - space_inds[2]);
    return solve_no_parenthesis(solve_no_parenthesis(first_part) + rest_part);
}

// Solve part 1 of the problem with parentheses
string solve_order(string problem)
{
    // If there are no parenthesis
    if (indices_matching(problem, '(').size() == 0)
    {
        return solve_no_parenthesis(problem);
    }

    int first_start_paren;

    for (int i = 0; i < problem.size(); i++)
    {
        char character = problem[i];
        if (character == '(')
        {
            first_start_paren = i;
        }
        else if (character == ')')
        {
            int first_i = first_start_paren;
            string sub_problem = problem.substr(first_i + 1, i - first_i - 1);
            string before_sub = problem.substr(0, first_i);
            string after_sub = problem.substr(i + 1, problem.size() - i - 1);
            return solve_order(before_sub + solve_order(sub_problem) + after_sub);
        }
    }

    return "null";
}

// Solve part 2 of the problem without parentheses
string solve_no_parenthesis_plusfirst(string problem)
{

    auto space_inds = indices_matching(problem, ' ');
    assert((space_inds.size() == 0) || (space_inds.size() >= 2));

    // =============================================================================
    // Base case, e.g. "1 + 2"
    if (space_inds.size() == 2)
    {
        string expr = problem.substr(space_inds[0] + 1, 1);
        unsigned long long a = stoull(problem.substr(0, space_inds[0]));
        unsigned long long b = stoull(problem.substr(space_inds[1], problem.size() - space_inds[1]));
        // cout << a << expr << b << endl;
        if (expr == "+")
        {
            return to_string(a + b);
        }
        if (expr == "*")
        {
            return to_string(a * b);
        }
    }

    string first_part;
    string rest_part;

    // =============================================================================
    // If + is in the problem
    regex pattern{R"(\d*\ \+\ \d*)"};
    smatch matches;
    while (regex_search(problem, matches, pattern))
    {
        first_part = problem.substr(0, matches.position());
        rest_part = problem.substr(matches.position() + matches.length(),
                                   problem.size() - (matches.position() + matches.length()));

        // Recurse on the first match
        return solve_no_parenthesis_plusfirst(first_part +
                                              solve_no_parenthesis_plusfirst(matches.str()) +
                                              rest_part);
    }

    // =============================================================================
    // General case when + is not in the problem, e.g. "1 * 2 * 3"
    first_part = problem.substr(0, space_inds[2]);
    rest_part = problem.substr(space_inds[2], problem.size() - space_inds[2]);
    return solve_no_parenthesis(solve_no_parenthesis(first_part) + rest_part);
}

// Solve part 2 of the problem with parentheses
string solve_plusfirst(string problem)
{
    // If there are no parenthesis
    if (indices_matching(problem, '(').size() == 0)
    {
        return solve_no_parenthesis_plusfirst(problem);
    }

    stack<int> starting_parenthesis_indices;

    for (int i = 0; i < problem.size(); i++)
    {
        char character = problem[i];
        if (character == '(')
        {
            starting_parenthesis_indices.push(i);
        }
        else if (character == ')')
        {
            int first_i = starting_parenthesis_indices.top();
            string sub_problem = problem.substr(first_i + 1, i - first_i - 1);
            string before_sub = problem.substr(0, first_i);
            string after_sub = problem.substr(i + 1, problem.size() - i - 1);
            // cout << before_sub << "x" << sub_problem << "x" << after_sub << endl;
            starting_parenthesis_indices.pop();
            return solve_plusfirst(before_sub + solve_plusfirst(sub_problem) + after_sub);
        }
    }

    return "null";
}

unsigned long long solve_problem(const vector<string> &lines,
                                 function<string(string)> solver_function)
{

    // Accumulate and return
    return accumulate(lines.begin(),
                      lines.end(),
                      (unsigned long long)0,
                      [solver_function](unsigned long long start,
                                        string line) { return start + stoull(solver_function(line)); });
}

int main()
{
    // Read input data
    auto lines = read_file("day-18-input.txt");

    // Tests
    assert(solve_order("1 + 2 * 3 + 4 * 5 + 6") == "71");
    assert(solve_order("2 * 3 + (4 * 5)") == "26");
    assert(solve_order("5 + (8 * 3 + 9 + 3 * 4 * 3)") == "437");
    assert(solve_order("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))") == "12240");
    assert(solve_order("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2") == "13632");

    auto answer1 = solve_problem(lines, solve_order);
    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 209335026987);

    // Tests for part 2
    assert(solve_plusfirst("5 + (8 * 3 + 9 + 3 * 4 * 3)") == "1445");
    assert(solve_plusfirst("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))") == "669060");
    assert(solve_plusfirst("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2") == "23340");

    auto answer2 = solve_problem(lines, solve_plusfirst);
    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == 33331817392479);
}
