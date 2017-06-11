#define YATF_MAIN
#include <yatf.hpp>
#include <cstdio>

int main(int argc, const char *argv[]) {
    return yatf::main(printf, argc, argv);
}

