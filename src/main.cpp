#include <jobber/jobber.h>
#include <utils/logger.h>

#include "sample_task.h"


using namespace jobber;
using TaskT = TestApp::TaskT;

#include <asio.hpp>
#include <iostream>


int main() {
    logger.set_verb_level(Logger::Level::none);

    storage::ComplexityCappedT<TaskT, splitter::LinearGreedy<TaskT>> capped_storage(
        splitter::LinearGreedy<TaskT>(1000)
    );

    pipeline::WorkerDrivenT<
        TaskT, 
        storage::ComplexityCappedT<TaskT, splitter::LinearGreedy<TaskT>>
    > pipeline(
        std::move(capped_storage)
    );

    std::vector<jobber::worker::LocalT<TaskT>> workers(4);
    for (auto& worker : workers) {
        pipeline.add_worker(worker);
    }

    std::vector<int> task_vector(100000000, 1);
    TaskT task(std::move(task_vector));

    pipeline.put(std::move(task));

    logger << "awaiting";
    auto result = pipeline.await();

    logger.set_verb_level(Logger::Level::debug);

    logger << "result = " << result;

    pipeline.stop();

    return 0;
}
