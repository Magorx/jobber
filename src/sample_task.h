#pragma once


#include <jobber/task/task.h>
#include <utils/event_system/signal.h>


namespace TestApp {

class Task {
public:
    using ResultT = int;

    struct ComputeContextT {
        int add_to_everything;
    };

    size_t complexity() const {
        return data.size();
    }

    ResultT run() {
        int sum = 0;
        for (const auto &i : data) {
            sum += i + compute_context.add_to_everything;
        }
        return sum;
    }

    std::vector<Task> split(size_t n_tasks) const {
        std::vector<Task> tasks;
        tasks.resize(n_tasks);

        size_t chunk_size = data.size() / n_tasks;
        size_t cur_pos = 0;
        for (size_t i = 0; i < n_tasks; ++i) {
            tasks[i].data.reserve(chunk_size);
            for (size_t j = 0; j < chunk_size; ++j) {
                tasks[i].data.push_back(data[cur_pos++]);
            }
        }

        tasks.push_back({});
        for (cur_pos; cur_pos < data.size(); ++cur_pos) {
            tasks.back().data.push_back(data[cur_pos]);
        }

        return tasks;
    }

    void update_compute_context(size_t new_add_to_everything) {
        compute_context.add_to_everything = new_add_to_everything;
    }

private:
    std::vector<int> data;

    static ComputeContextT compute_context;

};

} // namespace TestApp
