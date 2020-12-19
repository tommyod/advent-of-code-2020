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

vector<string> split_string(const string &line, const string delim)
{
    int start = 0;
    int delta = line.substr(start, line.size() - start).find(delim);
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

// Read the line
tuple<vector<vector<int>>, vector<int>, vector<vector<int>>> read_file(const string filename)
{

    // The files has three portions, we use an enum to represent them
    enum Line
    {
        attribute,
        my_ticket,
        nearby_tickets
    };

    Line line_state = attribute;

    // Return values
    vector<vector<int>> limits;
    vector<int> my_ticket_vect;
    vector<vector<int>> nearby_tickets_vect;

    fstream inputFile(filename, fstream::in);
    string file_line;

    // While there is data to read
    while (inputFile)
    {
        getline(inputFile, file_line, '\n');

        if (!file_line.size())
        {
            continue;
        }

        if (file_line == "your ticket:")
        {
            line_state = my_ticket;
            continue;
        }
        else if (file_line == "nearby tickets:")
        {
            line_state = nearby_tickets;
            continue;
        }

        if (line_state == attribute)
        {

            // Search for digits with a hyphen inbetween
            regex pattern{R"(\d{1,99}-\d{1,99})"};
            smatch matches;
            auto search = sregex_iterator(file_line.begin(), file_line.end(), pattern);

            // Limits for this line
            vector<int> these_limits;

            // For each match, add two limits to the answer
            for (sregex_iterator p(file_line.begin(), file_line.end(), pattern); p != sregex_iterator{}; ++p)
            {
                string matched = (*p)[0];
                int first = stoi(matched.substr(0, matched.find("-")));
                int second = stoi(matched.substr(matched.find("-") + 1, matched.size() - (matched.find("-") + 1)));
                these_limits.push_back(first);
                these_limits.push_back(second);
            }
            // Populate the limits vector
            limits.push_back(these_limits);
            continue;
        }

        // Add my ticket
        if ((line_state == my_ticket))
        {
            auto numbers_line = split_string(file_line, ",");
            for (auto number_str : numbers_line)
            {
                my_ticket_vect.push_back(stoi(number_str));
            }
            continue;
        }

        // Add a nearby ticket
        if ((line_state == nearby_tickets))
        {
            auto numbers_line = split_string(file_line, ",");
            vector<int> numbers_vect;
            for (auto number_str : numbers_line)
            {
                numbers_vect.push_back(stoi(number_str));
            }

            nearby_tickets_vect.push_back(numbers_vect);
            continue;
        }
    }
    inputFile.close();
    return make_tuple(limits, my_ticket_vect, nearby_tickets_vect);
}

// Solve part 1
pair<long long, set<int>> part1(const vector<vector<int>> &limits, const vector<vector<int>> &nearby_tickets)
{

    long long answer = 0;
    set<int> invalid_indices; // Keep track of invalid indices, needed for part 2
    int ticket_number = 0;

    // For every ticket
    for (auto ticket : nearby_tickets)
    {

        // cout << "==================================" << endl;

        // For every value in the ticket
        for (auto value : ticket)
        {

            // Assume invalid
            bool valid = false;

            for (auto limit : limits)
            {

                if ((value <= limit[1]) && (value >= limit[0]) ||
                    (value <= limit[3]) && (value >= limit[2]))
                {
                    valid = true;
                    break;
                }
            }

            // If the ticket is invalid, count it
            if (!valid)
            {
                answer += value;
                invalid_indices.insert(ticket_number);
            }
        }

        ticket_number++;
    }

    // Make a pair and return
    return make_pair(answer, invalid_indices);
}

// Solve the assignment problem in part 2
// The possible allocations is a vector of sets
// where v[i] represent possible allocations for i
vector<int> assign(vector<set<int>> possible_allocation)
{
    // Keep track of assigned
    vector<bool> assigned(possible_allocation.size(), false);

    // While not all are assigned (here I need an identity lambda function :/)
    while (!all_of(assigned.begin(), assigned.end(), [](bool i) { return i; }))
    {

        // Loop over every index
        for (int i = 0; i < possible_allocation.size(); i++)
        {

            // Skip assigned
            if (assigned[i])
                continue;

            // Skip if there are more than 1 possible allocation
            if (possible_allocation[i].size() > 1)
                continue;

            // At this point in the code there is only one possible allocation
            assert(possible_allocation[i].size() == 1);

            // Get the entry for the set
            auto entry = *possible_allocation[i].begin();
            assigned[i] = true; // Mark as assigned

            // Remove from all other possibilities
            for (int j = 0; j < possible_allocation.size(); j++)
            {
                // Skip assigned
                if (assigned[j])
                    continue;

                // If the entry is in the set, remove it
                if (possible_allocation[j].contains(entry))
                {
                    possible_allocation[j].erase(entry);
                }
            }
        }
    }

    // Transform the answer from a vector one 1-sets to a vector of ints
    vector<int> answer;
    for (auto allocation : possible_allocation)
    {
        auto entry = *allocation.begin();
        answer.push_back(entry);
    }

    return answer;
}

// Solve part 2
long part2(const vector<vector<int>> &limits,
           const vector<int> &my_ticket,
           const vector<vector<int>> &nearby_tickets)
{

    // Mapping pa[0] = {0, 1, 2}
    vector<set<int>> possible_allocation(limits.size());

    // Loop over every position (column)
    for (int position = 0; position < limits.size(); position++)
    {

        // Get values for every ticket in the column from the nearby tickets
        vector<int> tickets_in_position;
        for (auto &nearby_ticket : nearby_tickets)
        {
            tickets_in_position.push_back(nearby_ticket[position]);
        }

        // Loop through every limit, and see if the column could be a possibility
        for (int limit_num = 0; limit_num < limits.size(); limit_num++)
        {
            auto limit = limits[limit_num];

            // Assume it's within limits
            bool within_limits = true;
            for (auto ticket : tickets_in_position)
            {

                // If not within limits, break out
                if (((ticket < limit[0]) || (ticket < limit[2])) &&
                    ((ticket > limit[1]) || (ticket > limit[3])))
                {
                    within_limits = false;
                    break;
                }
            }

            // If it's within the limits, add to the vector of possible allocations
            if (within_limits)
            {
                possible_allocation[position].insert(limit_num);
            }
        }
    }

    // Solve the assignment problem
    auto assignment = assign(possible_allocation);
    long answer = 1;

    // For every column
    for (int slot = 0; slot < assignment.size(); slot++)
    {
        // Find the ticket attribute the column maps to
        int assigned = assignment[slot];
        // If it's within the first 6 'departure' attributes
        if (assigned <= 5)
            answer = answer * my_ticket[slot];
    }

    return answer;
}

int main()
{
    // Read input data
    auto [limits, my_ticket, nearby_tickets] = read_file("day-16-input.txt");
    auto [answer1, invalid_indices] = part1(limits, nearby_tickets);
    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 20060);

    // Remove invalid tickets before solving part 2
    vector<vector<int>> nearby_valid_tickets;
    for (int i = 0; i < nearby_tickets.size(); i++)
    {
        if (!invalid_indices.contains(i))
        {
            nearby_valid_tickets.push_back(nearby_tickets[i]);
        }
    }

    auto answer2 = part2(limits, my_ticket, nearby_valid_tickets);
    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == 2843534243843);
}