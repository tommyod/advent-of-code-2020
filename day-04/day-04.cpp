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
    vector<string> lines = {};

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

vector<map<string, string>> lines_to_passports(const vector<string> &lines)
{

    vector<map<string, string>> passports;
    map<string, string> current_map;

    for (auto line : lines)
    {

        // Blank line
        if (line.size() == 0)
        {
            passports.push_back(current_map);
            current_map.clear();
        }

        // Raw strings: R(<content>)
        regex pattern{R"(\w{3}\:\S+)"};
        regex value_pattern{R"(\:\S+)"};
        smatch key_matches;
        smatch value_matches;

        auto search = sregex_iterator(line.begin(), line.end(), pattern);

        for (sregex_iterator p(line.begin(), line.end(), pattern); p != sregex_iterator{}; ++p)
        {
            string matched = (*p)[0];
            int i = matched.find(":");
            string key = matched.substr(0, i);
            string value = matched.substr(i + 1, matched.size() - i - 1);
            current_map[key] = value;
        }
    }
    // Final item
    passports.push_back(current_map);

    return passports;
}

int part1(const vector<map<string, string>> &passports)
{
    // 'cid' is optional, every other field is required
    int valid_passports = 0;

    for (auto &passport : passports)
    {
        // Either all 8 keys are present, or one is missing and it's "cid"
        if ((passport.size() == 8) || ((passport.size() == 7) && (!passport.contains("cid"))))
        {
            valid_passports++;
        }
    }
    return valid_passports;
}

int part2(const vector<map<string, string>> &passports)
{
    int valid_passports = 0;

    for (auto passport : passports)
    {

        // Will be ignored either way, so remove it if it exists
        if (passport.contains("cid"))
        {
            passport.erase("cid");
        }

        // Skip missing
        if (!passport.size() == 7)
            continue;

        /*
        byr (Birth Year) - four digits; at least 1920 and at most 2002.
        iyr (Issue Year) - four digits; at least 2010 and at most 2020.
        eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
        hgt (Height) - a number followed by either cm or in:
            If cm, the number must be at least 150 and at most 193.
            If in, the number must be at least 59 and at most 76.
        hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
        ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
        pid (Passport ID) - a nine-digit number, including leading zeroes.
        cid (Country ID) - ignored, missing or not.
        */

        // Perform regex validation
        regex four_digits{R"(\d{4})"};

        if (!(regex_match(passport["byr"], four_digits) &&
              regex_match(passport["iyr"], four_digits) &&
              regex_match(passport["eyr"], four_digits) &&
              regex_match(passport["pid"], regex{R"(\d{9})"}) &&
              regex_match(passport["hgt"], regex{R"(\d+(in|cm))"}) &&
              regex_match(passport["hcl"], regex{R"(#([a-f]|[0-9]){6})"}) &&
              regex_match(passport["ecl"], regex{R"((amb|blu|brn|gry|grn|hzl|oth))"})))
        {
            continue;
        }

        // Perform range validation on numbers

        if ((stoi(passport["byr"]) > 2002) || (stoi(passport["byr"]) < 1920))
            continue;

        if ((stoi(passport["iyr"]) > 2020) || (stoi(passport["iyr"]) < 2010))
            continue;

        if ((stoi(passport["eyr"]) > 2030) || (stoi(passport["eyr"]) < 2020))
            continue;

        if (passport["hgt"].ends_with("cm"))
        {
            int height = stoi(passport["hgt"].substr(0, passport["hgt"].size() - 2));
            if (height > 193 || height < 150)
                continue;
        }
        if (passport["hgt"].ends_with("in"))
        {
            int height = stoi(passport["hgt"].substr(0, passport["hgt"].size() - 2));
            if (height > 76 || height < 59)
                continue;
        }

        // No errors, so the passport was valid
        valid_passports++;
    }
    return valid_passports;
}

int main()
{
    // Read input data
    auto lines = read_file("day-04-input.txt");
    auto passports = lines_to_passports(lines);

    auto answer1 = part1(passports);
    cout << "Answer to part 1: " << answer1 << endl;
    assert(answer1 == 222);

    auto answer2 = part2(passports);
    cout << "Answer to part 2: " << answer2 << endl;
    assert(answer2 == 140);
}