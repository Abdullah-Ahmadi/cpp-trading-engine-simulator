cmake_minimum_required(VERSION 3.16)
project(cpp_trading_engine_simulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(exchange_core
    src/Order.cpp
    src/CsvMarketLoader.cpp
    src/OrderBook.cpp
    src/Wallet.cpp
)
target_include_directories(exchange_core PUBLIC include)

if(MSVC)
    target_compile_options(exchange_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(exchange_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(trading_engine_sim app/main.cpp)
target_link_libraries(trading_engine_sim PRIVATE exchange_core)

include(CTest)
if(BUILD_TESTING)
    add_executable(exchange_tests tests/smoke_tests.cpp)
    target_link_libraries(exchange_tests PRIVATE exchange_core)
    add_test(NAME exchange_smoke_tests COMMAND exchange_tests)
endif()
