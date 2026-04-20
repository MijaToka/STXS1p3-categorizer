cmake_minimum_required(VERSION 3.26.5)
project(STXS-Categorization-BDT VERSION 0.0.1)

add_executable(${PROJECT_NAME}-train 
  bin/train.cpp
)

add_executable(${PROJECT_NAME}-application 
  bin/application.cpp
)

install(TARGETS ${PROJECT_NAME}-train ${PROJECT_NAME}-application 
  RUNTIME DESTINATION bin
)

find_package(
  ROOT 6.38 CONFIG REQUIRED
)

#[[
add_library(STXS_BDT_Train 
  
)

add_library(STXS_BDT_Application

)

add_library(STXS_BDT_Common

)
]]
target_link_libraries(${PROJECT_NAME}-train
  PUBLIC
    ${ROOT_LIBRARIES}
  #[[
  PRIVATE
    STXS_BDT_Common
    STXS_BDT_Train
    ]]
)

#[[
target_include_directories(${PROJECT_NAME}-train
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_include_directories(${PROJECT_NAME}-application
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
]]

target_link_libraries(${PROJECT_NAME}-application
  PUBLIC
    ${ROOT_LIBRARIES}
  #[[
  PRIVATE
    STXS_BDT_Common
    STXS_BDT_Application
  ]]
)


