# This is the beginning of the CMakeLists.txt file for the deribit-trading-system project

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.14)

# Define the project name and version
project(deribit-trading-system VERSION 1.0)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find and include the OpenSSL package
find_package(OpenSSL REQUIRED)

# Find and include the Threads package
find_package(Threads REQUIRED)

# Find and include the Boost package with the system component
find_package(Boost REQUIRED COMPONENTS system)

# Include the FetchContent module to add third-party libraries
include(FetchContent)

# Declare the nlohmann/json library to be fetched from GitHub
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)

# Make the nlohmann/json library available
FetchContent_MakeAvailable(json)

# Add the trading_system library target with source files
add_library(trading_system
    src/core/types.cpp
    src/core/constants.cpp
    src/exchange/deribit_client.cpp
    src/exchange/websocket_client.cpp
    src/trading/order_manager.cpp
    src/trading/position_manager.cpp
    src/market_data/orderbook.cpp
    src/market_data/market_data_handler.cpp
)

# Specify include directories for the trading_system target
target_include_directories(trading_system
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link the trading_system target with required libraries
target_link_libraries(trading_system
    PUBLIC
        OpenSSL::SSL
        OpenSSL::Crypto
        Threads::Threads
        Boost::system
        nlohmann_json::nlohmann_json
)

# Add an executable target for the basic_trading example
add_executable(basic_trading examples/basic_trading.cpp)

# Link the basic_trading executable with the trading_system library
target_link_libraries(basic_trading PRIVATE trading_system)

# Add an executable target for the market_data_subscription example
add_executable(market_data_subscription examples/market_data_subscription.cpp)

# Link the market_data_subscription executable with the trading_system library
target_link_libraries(market_data_subscription PRIVATE trading_system)

# Enable testing and add the tests subdirectory if BUILD_TESTING is set
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
cmake_minimum_required(VERSION 3.14)
project(deribit-trading-system VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

# Add third-party libraries
include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# Library targets
add_library(trading_system
    src/core/types.cpp
    src/core/constants.cpp
    src/exchange/deribit_client.cpp
    src/exchange/websocket_client.cpp
    src/trading/order_manager.cpp
    src/trading/position_manager.cpp
    src/market_data/orderbook.cpp
    src/market_data/market_data_handler.cpp
)

target_include_directories(trading_system
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(trading_system
    PUBLIC
        OpenSSL::SSL
        OpenSSL::Crypto
        Threads::Threads
        Boost::system
        nlohmann_json::nlohmann_json
)

# Examples
add_executable(basic_trading examples/basic_trading.cpp)
target_link_libraries(basic_trading PRIVATE trading_system)

add_executable(market_data_subscription examples/market_data_subscription.cpp)
target_link_libraries(market_data_subscription PRIVATE trading_system)

# Tests
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()