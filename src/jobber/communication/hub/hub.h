#pragma once


#include <mpi.h>


namespace jobber::comm {

class Hub {
public:
    struct HubToken;
public:
    void register_server() {
    }

    void register_worker() {
        // TODO
    }

    template <typename T>
    void send_message(T &&message) {
        // TODO
    }
};

}; // namespace jobber::comm
