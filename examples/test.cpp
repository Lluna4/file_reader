#include "../src/file_read.h"
#include <fcntl.h>
#include <unistd.h>
#include <print>

int main()
{
    file_reader a("test.txt");
    buffer t;
    t.data = nullptr;
    t.allocated = 0;
    t.size = 10;
    auto tup = std::make_tuple(t);
    auto tup2 = std::make_tuple(t);
    auto tup3 = std::make_tuple(t);
    auto res = a.read_from_tuple(tup);
    auto re2 = a.read_from_tuple(tup2);
    auto re3 = a.read_from_tuple(tup3);

    if (res.second == READ_CORRECT)
        std::println("{}", std::get<0>(tup).data);
    if (re2.second == READ_CORRECT)
        std::println("{}", std::get<0>(tup2).data);
    if (re3.second == READ_CORRECT)
        std::println("{}", std::get<0>(tup3).data);
    if (re3.second == READ_INCOMPLETE)
        std::println("incomplete read with size {} {}", re3.first, std::get<0>(tup3).data);
}