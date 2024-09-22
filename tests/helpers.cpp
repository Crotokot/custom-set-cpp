#include "helpers.h"
#include <random>
#include <algorithm>

namespace stl::unittests::datagen
{
    std::vector<int> make_random_int_data(int size,
                                          int low,
                                          int high,
                                          unsigned seed)
    {
        std::mt19937 rng(seed);
        std::uniform_int_distribution<int> dist(low, high);
        std::vector<int> data(size);
        std::generate(data.begin(), data.end(), [&dist, &rng](){ return dist(rng); });
        return data;
    }

    std::vector<char> make_random_char_data(int size, unsigned seed)
    {
        std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::mt19937 rng(seed);
        std::uniform_int_distribution<unsigned> dist(0, alphabet.size() - 1);
        std::vector<char> data(size);
        std::generate(data.begin(), data.end(), [&](){ return alphabet[dist(rng)]; });
        return data;
    }

    std::vector<std::string> make_random_string_data(int size, unsigned seed)
    {
        auto string_lens = make_random_int_data(size, 1, 10, seed);
        int chars_count(std::accumulate(string_lens.begin(), string_lens.end(), 0));
        auto chars_data = make_random_char_data(chars_count, seed);

        std::vector<std::string> data;
        unsigned offset(0);
        for (auto& len: string_lens) {
            std::string str(chars_data.begin() + offset, chars_data.begin() + offset + len);
            data.push_back(str);
            offset += len;
        }
        return data;
    }
}
