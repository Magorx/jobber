#include <jobber/jobber.h>
#include <utils/logger.h>

#include "sample_task.h"


using namespace jobber;


int main() {
    splitter::LinearBisecting<TestApp::TaskT> linear_bisecting(10);
    storage::Naive<TestApp::TaskT> naive_storage;

    kctf::logger << "I'm ready, think of " << 5 << '\n';

    return 0;
}
