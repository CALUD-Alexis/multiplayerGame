add_executable(client client.cpp)

# Configuration pour Windows
if(WIN32)
    set_target_properties(client PROPERTIES
        WIN32_EXECUTABLE FALSE  # Permet d'avoir une console
        OUTPUT_NAME "multiGame-client"
        SUFFIX ".exe"
    )
endif()

# Configuration pour macOS
if(APPLE)
    set_target_properties(client PROPERTIES
        OUTPUT_NAME "multiGame-client"
        MACOSX_BUNDLE FALSE  # Génère un exécutable simple, pas un .app bundle
    )
endif()

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

# Options de compilation pour optimisation et compatibilité
target_compile_features(client PRIVATE cxx_std_17)
if(MSVC)
    target_compile_options(client PRIVATE /W4)
else()
    target_compile_options(client PRIVATE -Wall -Wextra)
endif()
