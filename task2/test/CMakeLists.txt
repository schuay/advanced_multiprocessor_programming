include_directories(
    ${CHECK_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
    ${PHEET_INCLUDE_DIR}
)

add_executable(cuckoo_test cuckooset.cpp)
target_link_libraries(cuckoo_test ${CHECK_LIBRARIES} cuckooset)
add_test(cuckoo_test ${CMAKE_CURRENT_BINARY_DIR}/cuckoo_test)
