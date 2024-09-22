#include <vector>
#include <string>

namespace stl::unittests::datagen
{
    std::vector<int> make_random_int_data(int size,
                                          int low = 0,
                                          int high = 100,
                                          unsigned seed = 42);
    std::vector<char> make_random_char_data(int size, unsigned seed = 42);
    std::vector<std::string> make_random_string_data(int size, unsigned seed = 42);

}
