
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

void print_cups(list<int> v)
{
    for (auto var : v)
    {
        cout << var << " ";
    }
    cout << endl;
}

list<int>::iterator index_of_destination_cup(list<int> &cups, int current_cup)
{
    int max_smaller_than = -99;
    list<int>::iterator max_smaller_than_i;
    int max_larger_than = -99;
    list<int>::iterator max_larger_than_i;
    // cout << " === Finding index ===" << endl;
    // print_vector(cups);

    for (auto it = cups.begin(); it != cups.end(); it++)
    {

        if ((*it < current_cup) && (*it > max_smaller_than))
        {
            max_smaller_than = *it;
            max_smaller_than_i = it;
            // cout << "New smaller than i: " << i << endl;
        }
        else if ((*it > current_cup) && (*it > max_larger_than))
        {
            max_larger_than = *it;
            max_larger_than_i = it;
            // cout << "New larger than i: " << i << endl;
        }
    }

    if (max_smaller_than != -99)
    {
        return max_smaller_than_i;
    }
    return max_larger_than_i;
}

vector<int> rotate_cups(const vector<int> &cups, int places = 1)
{
    vector<int> rotated;
    for (int i = 0; i < cups.size(); i++)
    {
        rotated.push_back(cups[(i + places) % cups.size()]);
    }
    return rotated;
}

string part1(const string &input, const int num_simulations)
{

    // string input = "712643589";
    list<int> cups;
    for (auto number : input)
    {
        // https://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
        cups.push_back(int(number - '0'));
    }

    print_cups(cups);

    auto current_cup_iter = cups.begin();

    for (int simulation = 1; simulation <= num_simulations; simulation++)
    {

        int current_cup = *current_cup_iter;
        cout << "=========== move " << simulation << " ===========" << endl;
        cout << "Cups: ";
        print_cups(cups);

        cout << "Current cup: " << current_cup << endl;

        // Pick up three cups
        list<int> picked_up_cups;
        set<int> picked_up_cups_set;
        for (size_t i = 1; i <= 3; i++)
        {
            advance(current_cup_iter, i);
            int element = *current_cup_iter;
            cups.erase(current_cup_iter);
            picked_up_cups.push_back(element);
            picked_up_cups_set.insert(element);
        }
        advance(current_cup_iter, -3);

        print_cups(picked_up_cups);
        print_cups(cups);

        // Find destination index
        auto destination_cup_index = index_of_destination_cup(cups, current_cup);
        int destination_cup = *destination_cup_index;
        cout << "Destination cup: " << destination_cup << endl;

        return "123";
    }

    return "123";
}
/*
long part2(const string &input, const int num_simulations)
{

    // string input = "712643589";
    vector<int> cups;
    for (auto number : input)
    {
        // https://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
        cups.push_back(int(number - '0'));
    }

    int largest = cups.size();
    while (largest < num_simulations)
    {
        cups.push_back(largest + 1);
        largest++;
    }

    int current_cup_index = 0;

    for (int simulation = 1; simulation <= num_simulations; simulation++)
    {

        int current_cup = cups[current_cup_index];
        // cout << "-- move " << simulation << " --" << endl;
        // cout << "Cups: ";
        // print_vector(cups);

        // cout << "Current cup: " << current_cup << endl;

        // Pick up three cups
        vector<int> picked_up_cups;
        set<int> picked_up_cups_set;
        for (size_t i = 0; i < 3; i++)
        {
            int element = cups[((i + 1 + current_cup_index) % cups.size())];
            picked_up_cups.push_back(element);
            picked_up_cups_set.insert(element);
        }
        // print_vector(picked_up_cups);

        // Remove the cups
        vector<int> cups_new;

        for (auto cup : cups)
        {
            if (picked_up_cups_set.contains(cup))
            {
                continue;
            }
            else
            {
                cups_new.push_back(cup);
            }
        }
        cups = cups_new;
        // print_vector(cups);

        // Find destination index
        int destination_cup_index = index_of_destination_cup(cups, current_cup);
        int destination_cup = cups[destination_cup_index];
        // cout << "Destination cup: " << destination_cup << endl;
        for (int i = 0; i < picked_up_cups.size(); i++)
        {
            cups.insert(cups.begin() + destination_cup_index + i + 1, picked_up_cups[i]);
            //break;
        }
        if (destination_cup_index < current_cup_index)
        {
            current_cup_index += 4;
        }
        else
        {
            current_cup_index += 1;
        }
        // print_vector(cups);
        //cups = rotate_cups(cups, 1);
        //current_cup_index = (current_cup_index + 1) % cups.size();
    }

    int index_of_one = distance(cups.begin(), find(cups.begin(), cups.end(), 1));

    long answer = 1;
    for (int i = 1; i <= 2; i++)
    {
        cout << i << endl;
        answer = answer * (long)cups[(i + index_of_one) % cups.size()];
    }
    return answer;
}



*/

int main()
{

    assert(part1("389125467", 10) == "92658374");
    //assert(part1("389125467", 100) == "67384529");

    //auto answer1 = part1("712643589", 100);
    //cout << "Answer to part 1: " << answer1 << endl;

    //auto answer2 = part2("389125467", 1000);
    //cout << "Answer to part 2: " << answer2 << endl;
};