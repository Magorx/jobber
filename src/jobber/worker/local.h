#pragma once


#include "worker.h"


namespace jobber::worker {

template <TaskConcept TaskT>
class LocalT : public WorkerT<TaskT> {
public:
    virtual ResultT execute(TaskT &&task) {
        return task.run();
    }
};


} // namespace jobber::worker