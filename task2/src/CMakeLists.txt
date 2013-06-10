include_directories(
    ${PHEET_INCLUDE_DIR}
)

add_executable(cuckoo cuckoo.cpp)
target_link_libraries(cuckoo ${CMAKE_THREAD_LIBS_INIT} ${HWLOC_LIBRARIES})
