aux_source_directory(. JOBBER_TESTAPP_SOURCES)
aux_source_directory(event_system JOBBER_TESTAPP_SOURCES)

include_directories(extern)

add_subdirectory(utils)
add_subdirectory(jobber)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(jobber_testapp ${JOBBER_TESTAPP_SOURCES})

target_link_libraries(jobber_testapp
    jobber
    utils
)
