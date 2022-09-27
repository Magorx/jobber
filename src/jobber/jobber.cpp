#include "jobber/jobber.h"

#include "utils/logger.h"

int Struct::func() {
    kctf::logger.print("HELLO\n");
    return a + b;
}
