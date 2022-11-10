// #include <jobber/jobber.h>
// #include <utils/logger.h>
// #include <utils/timer.h>
// #include <argparse.h>

// #include "sample_task.h"


// using namespace jobber;
// using TaskT = TestApp::TaskT;


// int main(int argc, char* argv[]) {
//     // srand(time(nullptr));

//     argparse::ArgumentParser program("jobber", "0.1");

//     program.add_argument("-w", "--workers")
//         .help("workers count")
//         .default_value(4)
//         .scan<'i', int>();

//     program.add_argument("-c", "--complexity")
//         .help("complexity cap for splitted tasks")
//         .default_value(100)
//         .scan<'i', int>();

//     program.add_argument("-t", "--task_count")
//         .help("forces complexity to be calculated n / task_count")
//         .default_value(-1)
//         .scan<'i', int>();

//     program.add_argument("-s", "--task_size")
//         .help("task_size")
//         .default_value(1000)
//         .scan<'i', int>();

//     program.add_argument("-n", "--n")
//         .help("n")
//         .default_value(1000000)
//         .scan<'i', int>();

//     program.parse_args(argc, argv);

//     size_t n = program.get<int>("--n");
//     size_t workers_cnt = program.get<int>("--workers");
//     size_t complexity_cap = program.get<int>("--complexity");
//     size_t task_size = program.get<int>("--task_size");

//     int task_count = program.get<int>("--task_count");
//     if (task_count > 0) {
//         complexity_cap = n / task_count;
//     }

//     logger.set_verb_level(Logger::Level::info);

//     storage::ComplexityCappedT<TaskT, splitter::LinearGreedy<TaskT>> capped_storage(
//         splitter::LinearGreedy<TaskT>{complexity_cap}
//     );

//     pipeline::WorkerThreadedT<
//         TaskT,
//         storage::ComplexityCappedT<TaskT, splitter::LinearGreedy<TaskT>>
//     > pipeline(
//         std::move(capped_storage),
//         workers_cnt
//     );

//     std::vector<int> task_vector(n, task_size);
//     for (size_t i = 0; i < n; ++i) {
//         task_vector[i] = i % task_size;
//     }
//     TaskT task(std::move(task_vector));

//     pipeline.put(std::move(task));

//     logger << "awaiting";

//     kctf::Timer timer;
//     auto result = pipeline.await();
//     timer.stop();

//     logger << "result = " << result;
//     logger.n();
//     logger << "workers: " << workers_cnt << ", complexity: " << complexity_cap << ", task_size: " << task_size;
//     logger << "time   = " << (double) timer.elapsed() / 1000 << "s";

//     pipeline.stop();

//     return 0;
// }

#include <utils/logger_2.h>

using LoggerT = kctf::LoggerT;

int main() {
    LoggerT logger(std::cout);

    logger.debug << "ABOBA ONE";

    logger.set_log_level(LoggerT::Level::debug);

    logger.debug("one")("two") << "ABOBA TWO";

    logger.info << "za" << " wardu";
    logger.info("aboba")("two") << "ihih" << " world";
    logger.info << "hello world";

    return 0;
}
