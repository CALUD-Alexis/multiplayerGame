cmake_minimum_required(VERSION 3.10)
project(MultiplayerGame)

# Fetch ASIO
include(FetchContent)
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-30-2
)
FetchContent_MakeAvailable(asio)

add_subdirectory(server)
add_subdirectory(client)
