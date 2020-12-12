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


int row_id_from_string(string row_string){

    return 0;
}

// FBFBBFFRLR


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


int part1(const vector<string> &lines)
{

    return row_id_from_string("FBFBBFFRLR");
}



int main()
{

    // Read input data
    auto lines = read_file("day-05-input.txt");

    int answer1 = part1(matrix, 1, 3);
    cout << "Answer to part 1: " << answer1 << endl;

    //vector<tuple<int, int>> to_check = {{1, 1}, {1, 3}, {1, 5}, {1, 7}, {2, 1}};
    //auto answer2 = part2(matrix, to_check);
    //cout << "Answer to part 2: " << answer2 << endl;
}