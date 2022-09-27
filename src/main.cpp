#include <jobber/jobber.h>
#include <utils/logger.h>

#include <utils/event_system/signal.h>

int main() {
    Struct s{1, 2};

    kctf::logger.print("a + b = %d\n", s.func());

    return 0;
}
