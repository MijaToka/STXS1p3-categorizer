cmake_minimum_required(VERSION 3.26.5)
project(STXS1p3-categorization VERSION 0.0.1)

add_executable(${PROJECT_NAME}
  bin/main.cpp
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
)

add_library(Scripts
  plugins/STXS_script.cpp
  plugins/Preprocess.cpp
  plugins/STXS_common.cpp
  plugins/STXS_categorization0.cpp
  plugins/STXS_categorization1p2.cpp
  plugins/STXS_categorization1p3.cpp
  plugins/parseArgs.cpp
)

add_library(Analysis
  src/cConstants.cc 
  src/Discriminants.cc
)

find_package(
  ROOT 6.38 CONFIG REQUIRED
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ${ROOT_LIBRARIES}
  PRIVATE
    Scripts
    Analysis
)

target_include_directories(Analysis
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_include_directories(Scripts
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
