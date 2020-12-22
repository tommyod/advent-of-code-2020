
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

tuple<queue<int>, queue<int>> lines_to_decks(const vector<string> lines)
{

    bool player1 = true;
    queue<int> p1_deck;
    queue<int> p2_deck;

    for (auto line : lines)
    {

        if (line.starts_with("Player 1"))
        {
            player1 = true;
        }
        else if (line.starts_with("Player 2"))
        {
            player1 = false;
        }
        else if (player1)
        {
            p1_deck.push(stoi(line));
        }
        else
        {
            p2_deck.push(stoi(line));
        }
    }
    return make_pair(p1_deck, p2_deck);
}

long score(queue<int> deck)
{
    // Compute the score
    int total_cards = deck.size();
    long answer = 0;
    for (int i = total_cards; i >= 1; i--)
    {
        answer += i * deck.front();
        deck.pop();
    }
    return answer;
}

// Solve part 1 of the problem
long part1(queue<int> p1_deck, queue<int> p2_deck)
{

    // While not all cards are given to one player
    while ((p1_deck.size() > 0) && (p2_deck.size() > 0))
    {
        auto p1_top = p1_deck.front();
        p1_deck.pop();

        auto p2_top = p2_deck.front();
        p2_deck.pop();

        // Check who won and apply logic
        if (p1_top > p2_top)
        {
            // Player 1 won the round
            p1_deck.push(p1_top); // Winner card goes on top
            p1_deck.push(p2_top);
        }
        else
        {
            // Player 2 won
            p2_deck.push(p2_top);
            p2_deck.push(p1_top);
        }
    }
    assert((p1_deck.size() == 0) || (p2_deck.size() == 0));

    // Determine the winning score
    queue<int> winner_deck;
    if (p1_deck.size() == 0)
    {
        winner_deck = p2_deck;
    }
    else
    {
        winner_deck = p1_deck;
    }
    return score(winner_deck);
}

// Return a new queue with the first n elements
queue<int> take_first_n(queue<int> deck, int n)
{
    assert(deck.size() >= n);
    queue<int> new_queue;
    for (int i = 0; i < n; i++)
    {
        new_queue.push(deck.front());
        deck.pop();
    }
    return new_queue;
}

// Print a deck (queue of ints)
template <typename T>
void print_deck(queue<T> deck)
{

    int n = deck.size();
    for (int i = 0; i < n; i++)
    {
        cout << deck.front() << " ";
        deck.pop();
    }
    cout << endl;
}

// Solve part 2 of the problem
tuple<bool, queue<int>, queue<int>> part2(queue<int> p1_deck, queue<int> p2_deck, int game_id = 1)
{

    set<pair<queue<int>, queue<int>>> previous_games;

    // While not all cards are given to one player
    while ((p1_deck.size() > 0) && (p2_deck.size() > 0))
    {

        // If the cards have been seen before
        if (previous_games.contains(make_pair(p1_deck, p2_deck)))
        {
            // Player 1 wins -- move all cards to player 1 and return
            int p2_deck_size = p2_deck.size();
            for (int i = 0; i < p2_deck_size; i++)
            {
                auto p2_top = p2_deck.front();
                p2_deck.pop();
                p1_deck.push(p2_top);
            }
            return make_tuple(true, p1_deck, p2_deck);
        }
        else
        {
            previous_games.insert(make_pair(p1_deck, p2_deck));
        }

        auto p1_top = p1_deck.front();
        p1_deck.pop();

        auto p2_top = p2_deck.front();
        p2_deck.pop();

        // cout << "===========================================" << endl;
        // cout << "Player 1 cards: " << p1_top << " ";
        // print_deck(p1_deck);
        // cout << "Player 2 cards: " << p2_top << " ";
        // print_deck(p2_deck);
        // printf("Player 1 plays: %i \nPlayer 2 plays: %i \n", p1_top, p2_top);

        // If both players have at least as many cards remaining in their deck as the
        // value of the card they just drew, the winner of the round is determined by
        // playing a new game of Recursive Combat (see below).
        bool p1_wins;
        if ((p1_deck.size() + 1 > p1_top) && (p2_deck.size() + 1 > p2_top))
        {
            auto p1_sub_deck = take_first_n(p1_deck, p1_top);
            auto p2_sub_deck = take_first_n(p2_deck, p2_top);
            // cout << "Recursing..." << endl;

            auto [winner_is_p1, a, b] = part2(p1_sub_deck, p2_sub_deck, game_id = game_id + 1);
            p1_wins = winner_is_p1; // This line is needed!
        }
        else
        {
            p1_wins = (p1_top > p2_top) ? true : false;
        }

        if (p1_wins)
        {
            // Player 1 won the round
            p1_deck.push(p1_top); // Winner card goes on top
            p1_deck.push(p2_top);
            // cout << "Game id: " << game_id << " Player 1 won" << endl;
        }
        else
        {
            // Player 2 won
            p2_deck.push(p2_top);
            p2_deck.push(p1_top);
            // cout << "Game id: " << game_id << " Player 2 won" << endl;
        }
    }
    assert((p1_deck.size() == 0) || (p2_deck.size() == 0));

    // Figure out who won and return
    if (p2_deck.size() == 0)
    {
        return make_tuple(true, p1_deck, p2_deck);
    }
    else
    {
        return make_tuple(false, p1_deck, p2_deck);
    }
}

int main()
{
    auto lines = read_file("day-22-input.txt");
    auto [p1_deck, p2_deck] = lines_to_decks(lines);

    auto answer1 = part1(p1_deck, p2_deck);
    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 31781);

    auto [p1_wins, deck1, deck2] = part2(p1_deck, p2_deck);
    long answer2;
    if (deck1.size() == 0)
    {
        answer2 = score(deck2);
    }
    else
    {
        answer2 = score(deck1);
    }

    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == 35154);
};