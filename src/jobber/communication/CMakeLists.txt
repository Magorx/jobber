aux_source_directory(. JOBBER_COMM_SOURCES)
aux_source_directory(./hub JOBBER_COMM_SOURCES)
aux_source_directory(./server JOBBER_COMM_SOURCES)
aux_source_directory(./client JOBBER_COMM_SOURCES)

set(PREV_CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER})
set(CMAKE_CXX_COMPILER mpic++)

add_library(jobber_communication ${JOBBER_COMM_SOURCES})

set(CMAKE_CXX_COMPILER ${PREV_CMAKE_CXX_COMPILER})

target_include_directories(jobber_communication PUBLIC ..)
