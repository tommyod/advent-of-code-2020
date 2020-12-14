#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <cassert>
#include <set>
#include <queue>
#include <numeric>
#include <regex>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <unistd.h>
#include <cstdlib>

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

// Read a file to a vector of strings
tuple<long long, vector<string>> read_file(const string filename)
{
    long long earliest_time;
    vector<string> buses;

    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        int line_number = 0;
        while (getline(file, line))
        {
            if (line_number == 0)
            {
                earliest_time = stoll(line);
            }
            else
            {
                buses = split_string(line, ",");
            }
            line_number++;
        }
        file.close();
    }

    return {earliest_time, buses};
}

long long part1(long long earliest_time, const vector<long long> &buses)
{
    long long best = __LONG_LONG_MAX__;
    long long best_idx;
    for (auto bus : buses)
    {
        long long possible = bus - earliest_time % bus;
        if (possible < best)
        {
            best = possible;
            best_idx = bus;
        }
    }
    return best_idx * best;
}

vector<long long> buses_to_vector(const vector<string> &buses)
{

    vector<long long> output;
    for (auto bus : buses)
    {
        if (!(bus == "x"))
        {
            output.push_back(stoll(bus));
        }
    }
    return output;
}

template <typename T>
vector<size_t> sort_indexes(const vector<T> &v)
{

    // initialize original index locations
    vector<size_t> idx(v.size());
    iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    // using std::stable_sort instead of std::sort
    // to avoid unnecessary index re-orderings
    // when v contains elements of equal values
    stable_sort(idx.begin(), idx.end(),
                [&v](size_t i1, size_t i2) { return v[i1] > v[i2]; });

    return idx;
}

// Solve a system x = rhs % mods using sieve. (see wikipedia)
// The mods must be pairwise coprime
template <typename T>
T solve_system_mod(vector<T> rhs, vector<T> mods)
{
    assert(rhs.size() == mods.size());

    // Sort inputs from largest to smallest
    vector<size_t> sorted_inds = sort_indexes(mods);
    vector<T> rhs_sorted(rhs.size());
    vector<T> mods_sorted(mods.size());

    for (size_t i = 0; i < mods.size(); i++)
    {
        mods_sorted[i] = mods[sorted_inds[i]];
        rhs_sorted[i] = rhs[sorted_inds[i]] % mods_sorted[i];
    }

    unsigned long long current_value = rhs_sorted[0];
    unsigned long long step_value = mods_sorted[0];
    int next_equation = 1;
    while (true)
    {
        if (current_value % mods_sorted[next_equation] == rhs_sorted[next_equation])
        {
            step_value = step_value * mods_sorted[next_equation];
            next_equation++;
            if (next_equation >= mods.size())
            {
                break;
            }
        }
        current_value += step_value;
    }

    for (size_t i = 0; i < mods.size(); i++)
    {
        //cout << "Test: " << current_value % mods[i] << " = " << rhs[i] % mods[i] << endl;
        assert(current_value % mods[i] == rhs[i] % mods[i]);
    }

    return current_value;
}

long long part2(const vector<string> &buses)
{

    // We solve a set of equations for a common value x such
    // that x = rhs_i (mod mods_i)
    vector<unsigned long long> rhs;
    vector<unsigned long long> mods;

    for (int i = 0; i < buses.size(); i++)
    {
        if (buses[i] == "x")
            continue;

        // If a bus leaves every 3 minutes, and it should depart 5 minutes
        // later, then the equation is x + 5 = 0 (mod 3)
        // We transform it into canonical form:
        // x     = -5     (mod 3)
        // x + 6 = -5 + 6 (mod 3)
        // x     =  1     (mod 3)
        long long bus_time = stoull(buses[i]);
        mods.push_back(bus_time);

        // Compute number of cycles to add to obtain positive right hand side (rhs)
        long long bus_number = (long long)i;
        int rounds = (int)ceil((float)bus_number / bus_time);
        rhs.push_back(rounds * bus_time - bus_number);
    }

    return solve_system_mod(rhs, mods);
}

int main()
{
    // Read input data
    auto [earliest_time, buses] = read_file("day-13-input.txt");
    auto buses_numbers = buses_to_vector(buses);

    auto answer1 = part1(earliest_time, buses_numbers);
    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 156);

    // Tests
    auto test1 = part2(vector<string>{"17", "x", "13", "19"});
    assert(test1 == 3417);

    auto test2 = part2(vector<string>{"67", "7", "59", "61"});
    assert(test2 == 754018);

    auto test3 = part2(vector<string>{"67", "x", "7", "59", "61"});
    assert(test3 == 779210);

    auto test4 = part2(vector<string>{"67", "7", "x", "59", "61"});
    assert(test4 == 1261476);

    auto example1 = part2(vector<string>{"7", "13", "x", "x", "59", "x", "31", "19"});
    assert(example1 == 1068781);

    auto answer2 = part2(buses);
    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == 404517869995362);
}