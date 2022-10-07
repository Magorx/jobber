#pragma once


#include <jobber/task/task.h>
#include <utils/event_system/signal.h>


namespace TestApp {

class TaskT {
public:
    using ResultT = int;

    struct ComputeContextT {
        int add_to_everything;
    };

    struct ComputeContextUpdateT {
        int add_to_everything;
    };

public:
    TaskT(std::vector<int> &&data = {})
        : data(std::move(data))
    {}

public:
    ResultT run() {
        int sum = 0;
        for (const auto &i : data) {
            sum += i + compute_context.add_to_everything;
        }
        return sum;
    }

    size_t complexity() const {
        return data.size();
    }

    std::vector<TaskT> split(size_t n_tasks) const {
        std::vector<TaskT> tasks;
        tasks.resize(n_tasks);

        size_t chunk_size = data.size() / n_tasks;
        size_t cur_pos = 0;
        for (size_t i = 0; i < n_tasks; ++i) {
            tasks[i].data.reserve(chunk_size);
            for (size_t j = 0; j < chunk_size; ++j) {
                tasks[i].data.push_back(data[cur_pos++]);
            }
        }

        if (cur_pos < data.size()) {
            tasks.push_back(TaskT(std::vector<int>(data.begin() + cur_pos, data.end())));
        }

        return tasks;
    }

    static void update_compute_context(const ComputeContextUpdateT &update) {
        compute_context.add_to_everything = update.add_to_everything;
    }

    static ResultT reduce(const std::vector<ResultT> &results) {
        ResultT sum = 0;
        for (const auto &i : results) {
            sum += i;
        }
        return sum;
    }

private:
    std::vector<int> data;

    static ComputeContextT compute_context;
};

} // namespace TestApp
