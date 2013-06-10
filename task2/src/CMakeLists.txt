include_directories(
    ${PHEET_INCLUDE_DIR}
)

add_library(cuckooset SHARED cuckooset.cpp)
target_link_libraries(cuckooset ${CMAKE_THREAD_LIBS_INIT} ${HWLOC_LIBRARIES})
