aux_source_directory(. JOBBER_SOURCES)

add_library(jobber ${JOBBER_SOURCES})

target_include_directories(jobber PUBLIC ..)
