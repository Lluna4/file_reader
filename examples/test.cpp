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
    auto res = a.read_from_tuple(tup);

    if (res.second == READ_CORRECT)
        std::println("{}", std::get<0>(tup).data);
}