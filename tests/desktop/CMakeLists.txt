cmake_minimum_required(VERSION 3.16)
project(pocketmeteo_domain_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

set(DOMAIN_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../pocketmeteo/src)

add_executable(domain_tests
  test_main.cpp
  test_pressure_history.cpp
  test_sea_level.cpp
  test_trend.cpp
  test_zambretti.cpp
  ${DOMAIN_ROOT}/domain/PressureHistory.cpp
  ${DOMAIN_ROOT}/domain/SeaLevelReducer.cpp
  ${DOMAIN_ROOT}/domain/TrendAnalyzer.cpp
  ${DOMAIN_ROOT}/domain/ZambrettiForecast.cpp
)

target_include_directories(domain_tests PRIVATE ${DOMAIN_ROOT})

add_test(NAME domain_tests COMMAND domain_tests)
