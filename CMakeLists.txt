cmake_minimum_required(VERSION 3.15)
project(MyCrowApp LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Crow (system-installed)
find_package(Crow REQUIRED)

# Find other dependencies
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)  # For pthread

# Create executable
add_executable(${PROJECT_NAME} main.cpp)

# Link libraries (Linux-specific)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Crow::Crow          # System Crow installation
    OpenSSL::SSL        # HTTPS support
    OpenSSL::Crypto
    pthread             # POSIX threads
    ${CMAKE_THREAD_LIBS_INIT}
)

# Optional: Enable compiler warnings
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic
)

# Install target (optional)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)