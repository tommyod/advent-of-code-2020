
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

template <typename T>
set<T> intersection(set<T> &a, set<T> &b)
{
    set<T> result;
    set_intersection(a.begin(), a.end(),
                     b.begin(), b.end(),
                     inserter(result, result.begin()));
    return result;
}

map<string, set<string>> lines_to_mapping(const vector<string> &lines)
{

    map<string, set<string>> mapping;

    for (auto line : lines)
    {
        int i = line.find("contains ") + 9;
        auto ingredients = split_string(line.substr(0, line.find(" (")), " ");
        auto allergens = split_string(line.substr(i, line.size() - i - 1), ", ");

        set<string> ingredients_set(ingredients.begin(), ingredients.end());
        for (auto allergen : allergens)
        {
            // Does not exist - add it
            if (!mapping.contains(allergen))
            {
                mapping[allergen] = ingredients_set;
                continue;
            }

            // Exists, take intersection
            mapping[allergen] = intersection(mapping[allergen], ingredients_set);
        }
    }

    return mapping;
}

vector<string> lines_to_ingredients(const vector<string> &lines)
{
    vector<string> all_ingredients;
    for (auto line : lines)
    {
        int i = line.find("contains ") + 9;
        auto ingredients = split_string(line.substr(0, line.find(" (")), " ");
        for (auto ingredient : ingredients)
        {
            all_ingredients.push_back(ingredient);
        }
    }
    return all_ingredients;
}

// Create a 1-1 mapping from 1-many mapping
map<string, string> allocate(map<string, set<string>> mapping)
{
    // The finaly 1-1 allocation
    map<string, string> allocation;

    // Sum of sizes of all value sets in the mapping
    int size = accumulate(mapping.begin(), mapping.end(), 0,
                          [](int initial, map<string, set<string>>::value_type &next) {
                              return initial + next.second.size();
                          });

    // While the mapping is not one-to-one
    while (size > mapping.size())
    {

        for (auto &[allergen, ingredients] : mapping)
        {

            if (ingredients.size() > 1)
                continue;

            auto ingredient_ptr = ingredients.begin();
            auto ingredient = *ingredient_ptr;
            allocation[allergen] = ingredient;
            //ingredients_with_allergens.insert(ingredient);

            for (auto &[allergen_other, ingredients_other] : mapping)
            {
                if (ingredients_other.contains(ingredient) && (allergen != allergen_other))
                    ingredients_other.erase(ingredient);
            }
        }

        size = accumulate(mapping.begin(), mapping.end(), 0,
                          [](int initial, map<string, set<string>>::value_type &next) {
                              return initial + next.second.size();
                          });
    }

    return allocation;
}

int main()
{
    auto lines = read_file("day-21-input.txt");

    // Get a mapping from allergens to the intersection of ingredients
    // contained in all lines where the allegen is present
    auto mapping = lines_to_mapping(lines);

    // Find the one-to-one allocation between ingredients and allergens
    // Returns a mapping allergen -> ingredient
    auto allocation = allocate(mapping);

    // ======================= Solve part 1 =======================

    // Create a set of all ingredients with allergens
    set<string> ingredients_with_allergen;
    for (auto [allergen, ingredient] : allocation)
    {
        ingredients_with_allergen.insert(ingredient);
    }

    // Count the occurence of ingredients that are not allergens
    int answer1 = 0;
    for (auto ingredient : lines_to_ingredients(lines))
    {
        if (!ingredients_with_allergen.contains(ingredient))
            answer1++;
    }

    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 2423);

    // ======================= Solve part 2 =======================
    string answer2;
    for (auto [allergen, ingredient] : allocation)
        answer2 = answer2 + ingredient + ",";

    answer2 = answer2.substr(0, answer2.size() - 1);
    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == "jzzjz,bxkrd,pllzxb,gjddl,xfqnss,dzkb,vspv,dxvsp");

    return 0;
};