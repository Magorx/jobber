#include <jobber/jobber.h>
#include <utils/logger.h>

#include "sample_task.h"


using namespace jobber;
using TaskT = TestApp::TaskT;


int main() {
    storage::ComplexityCappedT<TaskT, splitter::LinearGreedy<TaskT>> capped_storage(
        splitter::LinearGreedy<TaskT>(3)
    );

    TaskT task({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    capped_storage.put({task});

    auto tasks = capped_storage.take(capped_storage.size());

    logger << "tasks: " << tasks.size() << '\n';

    for (const auto &task : tasks) {
        kctf::logger << task.complexity();
    }

    return 0;
}
