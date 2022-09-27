aux_source_directory(. UTILS_SOURCES)

add_library(utils ${UTILS_SOURCES})

target_include_directories(utils PUBLIC ..)
