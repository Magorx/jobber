#include <jobber/jobber.h>
#include <utils/logger.h>

#include <utils/event_system/signal.h>

int main() {
    Struct s{1, 2};

    kctf::logger << "I'm ready, think of " << 5 << '\n';

    return 0;
}
