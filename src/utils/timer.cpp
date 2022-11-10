#include "timer.h"

namespace kctf {

Timer::Timer(const IdType &id, bool not_to_start, LoggerT &logger):
is_stopped(false),
logger_(logger),
cur_elapsed(0),
id(id)
{
    if (!not_to_start) start_timestamp = std::chrono::system_clock::now();
}

void Timer::start() {
    is_stopped = false;
    start_timestamp = std::chrono::system_clock::now();
}

uint64_t Timer::elapsed() {
    if (is_stopped) {
        return cur_elapsed;
    }

    auto cur_timestamp = std::chrono::system_clock::now();
    cur_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(cur_timestamp - start_timestamp).count();
    return cur_elapsed;
}

void Timer::restart() {
    is_stopped = false;
    start_timestamp = std::chrono::system_clock::now();
}

void Timer::stop() {
    is_stopped = true;

    stop_timestamp = std::chrono::system_clock::now();
    cur_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stop_timestamp - start_timestamp).count();
}

void Timer::print() {
    if (id.size()) {
        logger_.info("timer")(id) << " " << (double) elapsed() / 1000 << "s";
    } else {
        logger_.info("timer") << (double) elapsed() / 1000 << "s";
    }
}

} // namespace kctf
