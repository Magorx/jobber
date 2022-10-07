#pragma once


#include <concepts>
#include <cstddef>
#include <vector>

namespace jobber {

template<typename TaskT>
concept TaskConcept = requires(
    TaskT task,
    size_t n_tasks,
    const typename TaskT::ComputeContextUpdateT &compute_context_update,
    const std::vector<typename TaskT::ResultT> &results
) {
    { task.run()             } -> std::convertible_to<typename TaskT::ResultT>;
    { task.complexity()      } -> std::convertible_to<size_t>;

    { task.split(n_tasks)    } -> std::convertible_to<std::vector<TaskT>>;
    { TaskT::reduce(results) } -> std::convertible_to<typename TaskT::ResultT>;

    { task.update_compute_context(compute_context_update) };
};

} // namespace jobber
