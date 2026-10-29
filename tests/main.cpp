#include "Channel.hpp"
#include "colours.hpp"

// maybe in the future have a header
void test_channel();

int main(void)
{
    // Test the Channel class
    test_channel();

    std::cout << colour::GREEN << "ALL TESTS PASSED!" << colour::RESET << std::endl;
}