#include "progress_bar.h"


namespace kctf {

void ProgressBar::print_prefix() {
    stream_ << "\r";
    // logger_.print_log_prefix("prog", "bar", false, true, true);
}

void ProgressBar::on_start() {
    start_timestamp = std::chrono::system_clock::now();

    print_prefix();
    stream_ << "[>         ] |  0%| " << id_ << " [|]";
}

void ProgressBar::on_tick() {
    if ((int) (((double) cur_tick / capacity) * 100) <= cur_step) {
        return;
    } else {
        cur_step = (int) (((double) cur_tick / capacity) * 100);
    }

    auto prev_end = stream_.get_end();
    stream_.set_end("");

    auto prev_to_ignore_prefix = stream_.get_to_ignore_prefix();
    stream_.set_to_ignore_prefix(true);

    print_prefix();

    stream_.set_to_ignore_prefix(prev_to_ignore_prefix);

    stream_ << "[";

    stream_.set_to_ignore_prefix(true);

    int i = 0;
    for (; i < 10 && i < ((double) cur_tick / capacity) * 10 - 1; ++i) {
        stream_ << "=";
    }
    if (i < 10) {
        ++i;
        stream_ << ">";
    }
    for (; i < 10; ++i) {
        stream_ << " ";
    }
    
    std::string time_str(30, '\0');
    sprintf(&time_str[0], "] |% 3d%%| %s [|]", cur_step, id_.c_str());

    stream_ << time_str;

    stream_.set_end(prev_end);
    stream_.set_to_ignore_prefix(prev_to_ignore_prefix);
}

void ProgressBar::on_stop() {
    if (stopped_) {
        return;
    } else {
        stopped_ = true;
    }

    auto now = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_timestamp);

    auto prev_end = stream_.get_end();
    stream_.set_end("");

    print_prefix();

    stream_.set_end(prev_end);

    stream_ << "[==========] |100%| " << id_ << " [+] (" << (double) elapsed.count() / 1000 << "s)";
}

void ProgressBar::turn_wheele() const {
    auto prev_end = stream_.get_end();
    auto prev_to_ignore_prefix = stream_.get_to_ignore_prefix();
    stream_.set_end("");
    stream_.set_to_ignore_prefix(true);

    stream_ << "\b\b" << wheel_chars[cur_tick % wheel_chars_len] << "]";

    stream_.set_end(prev_end);
    stream_.set_to_ignore_prefix(prev_to_ignore_prefix);

    stream_.flush();
}

ProgressBar::ProgressBar(
    int capacity_,
    std::string id,
    LoggerT::LoggerStreamT &stream)
    : cur_tick(0)
    , capacity(capacity_)
    , cur_step(1)
    , stopped_(false)
    , stream_(stream)
    , id_(id)
{
    start_timestamp = std::chrono::system_clock::now();
}

bool ProgressBar::start(int new_capacity) {
    if (new_capacity > 0) {
        capacity = new_capacity;
    }

    if (capacity < 1) {
        return false;
    }

    cur_tick = 0;
    cur_step = 0;
    on_start();
    return true;
}

bool ProgressBar::tick(const int tick_step) {
    if (cur_step < 0 || stopped_) {
        return false;
    }

    cur_tick += tick_step;
    if (cur_tick < capacity) {
        on_tick();
        turn_wheele();
        return true;
    } else {
        on_stop();
        return false;
    }
}

bool ProgressBar::update(const double frac) {
    if (cur_step < 0 || stopped_) {
        return false;
    }

    cur_tick = (int) (frac * capacity);
    if (cur_tick < capacity) {
        on_tick();
        turn_wheele();
        return true;
    } else {
        on_stop();
        return false;
    }
}

} // namespace kctf
