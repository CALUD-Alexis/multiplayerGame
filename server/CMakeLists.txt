add_executable(server server.cpp)

find_package(Threads REQUIRED)
target_include_directories(server PRIVATE ${asio_SOURCE_DIR}/asio/include)
target_compile_definitions(server PRIVATE ASIO_STANDALONE)
target_link_libraries(server PRIVATE Threads::Threads)
