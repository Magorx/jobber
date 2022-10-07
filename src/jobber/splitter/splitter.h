#pragma once


#include <jobber/task/task.h>

#include <concepts>
#include <vector>

namespace jobber::splitter {

template<typename SplitterT, typename TaskT>
concept Concept = requires(
    SplitterT splitter,
    TaskT &&task
) {
    { splitter.split(task) } -> std::convertible_to<std::vector<TaskT>>;
};

} // namespace jobber::splitter
