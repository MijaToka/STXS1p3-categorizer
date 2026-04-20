cmake_minimum_required(VERSION 3.26.5)
project(STXSCategorizer VERSION 1.0.0)

option(BUILD_CATEGORIZE "Build Categorize SubModule" OFF)
option(BUILD_CATEGORIZEBDT "Build CategorizeBDT SubModule" OFF)

if(BUILD_CATEGORIZE)
  add_subdirectory(Categorize)
endif()

if(BUILD_CATEGORIZEBDT)
  add_subdirectory(CategorizeBDT)
endif()


