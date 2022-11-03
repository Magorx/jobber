#include <jobber/jobber.h>
#include <utils/logger.h>

#include "sample_task.h"


using namespace jobber;
using TaskT = TestApp::TaskT;


int main() {
    storage::ComplexityCappedT<TaskT, splitter::LinearGreedy<TaskT>> capped_storage(
        splitter::LinearGreedy<TaskT>(3)
    );

    pipeline::ThreadLocalT<
        TaskT, 
        storage::ComplexityCappedT<TaskT, splitter::LinearGreedy<TaskT>>
    > pipeline(
        std::move(capped_storage)
    );

    // PipelineT<TaskT> pipeline(
    //     PipelineConfigT()
    //         .ip("0.0.0.0")
    //         .port(100010)
    //         .max_complexity(3)
    // );

    TaskT task({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    pipeline.put(std::move(task));
    auto result = pipeline.await();

    logger << "result = " << result;

    // logger << "initial task complexity [" << task.complexity() << ']';
    // capped_storage.put({task});

    // auto tasks = capped_storage.take(capped_storage.size());

    // logger << "tasks in storage (" << tasks.size() << ")\n";

    // for (const auto &task : tasks) {
    //     logger << "complexity [" << task.complexity() << ']';
    // }

    return 0;
}
