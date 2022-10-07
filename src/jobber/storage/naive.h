#include "storage.h"


#include <queue>


namespace jobber::storage {

template <jobber::TaskConcept TaskT>
class Naive {
private:
    std::queue<TaskT> tasks;
public:
    void insert(std::vector<TaskT> &&tasks) {
        for (auto &&task : tasks) {
            this->tasks.push(std::move(task));
        }
    }

    std::vector<TaskT> take(size_t n_tasks) {
        std::vector<TaskT> result;
        for (size_t i = 0; i < n_tasks; ++i) {
            if (tasks.empty()) {
                break;
            }
            result.push_back(std::move(tasks.front()));
            tasks.pop();
        }
        return result;
    }
};


} // namespace jobber::storage
