include_directories(
    ${PHEET_INCLUDE_DIR}
)

add_executable(pcn pcn.cpp)
target_link_libraries(pcn ${CMAKE_THREAD_LIBS_INIT} ${HWLOC_LIBRARIES})
