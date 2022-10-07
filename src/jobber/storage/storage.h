#pragma once


#include <jobber/task/task.h>

#include <concepts>
#include <vector>

namespace jobber::storage {

template<typename StorageT, typename TaskT>
concept Concept = requires(
    StorageT storage,
    std::vector<TaskT> &&tasks,
    size_t n_tasks
) {
    { storage.insert(tasks) };
    { storage.take(n_tasks) } -> std::convertible_to<std::vector<TaskT>>;
};

} // namespace jobber::storage
