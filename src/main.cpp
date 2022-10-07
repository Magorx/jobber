#include <jobber/jobber.h>
#include <utils/logger.h>

#include <sample_task.h>

int main() {
    Struct s{1, 2};
    s.func();

    kctf::logger << "I'm ready, think of " << 5 << '\n';

    return 0;
}
