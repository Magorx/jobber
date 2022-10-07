#pragma once


#include <concepts>
#include <cstddef>
#include <vector>

namespace jobber {

template<typename TaskT>
concept TaksConcept = requires(
    TaskT task,
    size_t n_tasks
) {
    { task.run()          } -> std::convertible_to<typename TaskT::ResultT>;
    { task.complexity()   } -> std::convertible_to<size_t>;
    { task.split(n_tasks) } -> std::convertible_to<std::vector<TaskT>>;
};

} // namespace jobber
