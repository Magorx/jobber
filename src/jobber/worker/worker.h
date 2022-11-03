#pragma once


#include <jobber/task/task.h>


namespace jobber::worker {

template <TaskConcept TaskT>
class WorkerT {
protected:
    using ResultT = typename TaskT::ResultT;

public:
    virtual ResultT execute(TaskT &&task) = 0;

    virtual ~WorkerT() = default;
};

} // namespace jobber::worker
