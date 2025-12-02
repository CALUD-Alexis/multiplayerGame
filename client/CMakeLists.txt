add_executable(client client.cpp)

find_package(Threads REQUIRED)
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

target_include_directories(client PRIVATE ${asio_SOURCE_DIR}/asio/include)
target_compile_definitions(client PRIVATE ASIO_STANDALONE)
target_link_libraries(client PRIVATE
    Threads::Threads
    SFML::Graphics
    SFML::Window
    SFML::System
)
