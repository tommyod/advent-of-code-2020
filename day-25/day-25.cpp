
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

template <typename T>
T power(T base, T exponent, T mod)
{

    T answer = 1;

    for (int i = 0; i < exponent; i++)
    {
        answer = (answer * base) % mod;
    }
    return answer;
}

int main()
{

    long long remainder = 20201227;
    long long subject_number = 7;
    long long start_value = 1;

    long long card_public_key = 9232416;  // 5764801;
    long long door_public_key = 14144084; // 17807724;

    long long card_public_key_attempt = 1;
    long long card_loop_size = 1;

    long long i = 1;
    while (true)
    {
        card_public_key_attempt = card_public_key_attempt * subject_number % remainder;
        if (card_public_key_attempt == card_public_key)
        {
            cout << i << endl;
            card_loop_size = i;
            break;
        }

        i++;
    }

    // The handshake used by the card and the door involves an operation that
    // transforms a subject number. To transform a subject number, start with the value 1.
    // Then, a number of times called the loop size, perform the following steps:
    long long answer = 1;
    for (long long i = 0; i < card_loop_size; i++)
    {
        // Set the value to itself multiplied by the subject number.
        // Set the value to the remainder after dividing the value by 20201227.
        answer = answer * door_public_key % remainder;
    }
    cout << answer << endl;

    return 0;
};