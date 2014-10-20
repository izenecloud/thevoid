include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)
add_executable(server server.cpp)
target_link_libraries(server thevoid)
