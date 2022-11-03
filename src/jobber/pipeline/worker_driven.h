#pragma once


#include "pipeline.h"

#include <jobber/worker/worker.h>

#include <asio.hpp>

#include <vector>
#include <queue>
#include <mutex>


namespace jobber::pipeline {

template <
    TaskConcept TaskT,
    StorageConcept<TaskT> StorageT
>
class WorkerDrivenT {
private:
    using ResultT = typename TaskT::ResultT;
    using WorkerT = worker::WorkerT<TaskT>;

    using GivenTaskT = TaskT;

private:
    struct StatusT {
        bool is_online_;
    };
    

private:
    StatusT status_;

    StorageT storage_;
    std::mutex storage_mutex_;

    std::vector<ResultT> results_;
    std::mutex results_mutex_;

    asio::io_context io_;

private:
    GivenTaskT get_task() {
        std::lock_guard lock(storage_mutex_);

        while (is_online()) {
            if (storage_.size() > 0) {
                auto tasks = storage_.take(1);
                return std::move(tasks[0]);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        return TaskT::empty();
    }

    void push_result(ResultT &&result) {
        std::lock_guard lock(results_mutex_);
        results_.push_back(std::move(result));
    }

    bool is_online() {
        return status_.is_online_;
    }

    void worker_routine(WorkerT &worker) {
        while (is_online()) {
            auto task = get_task();
            if (task.is_empty()) {
                break;
            }

            auto result = task.run();
            push_result(std::move(result));
        }
    }

public:
    WorkerDrivenT(StorageT storage)
        : storage_(std::move(storage))
    {}

    void put(TaskT &&task) {
        storage_.put(std::move(std::vector{task}));
    }

    void add_worker(WorkerT &&worker) {
        // asio::post(io_, [this, &worker]() {
        //     worker_routine(worker);
        // });
    }

    ResultT await() {
        auto tasks = storage_.take(storage_.size());
        std::vector<ResultT> results;

        for (auto &&task : tasks) {
            results.push_back(task.run());
        }

        return TaskT::reduce(std::move(results_));
    }
};

} // namespace jobber::pipeline
