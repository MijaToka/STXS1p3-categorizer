add_library(STXS_CommonUtils
  src/Preprocess.cpp
  src/STXS_common.cpp
  src/STXS_Categories.cpp
)

add_library(Analysis
  src/cConstants.cc 
  src/Discriminants.cc
)

target_include_directories(Analysis
  PUBLIC ${ZZANALYSIS_INCLUDE_DIR}
)

target_include_directories(STXS_CommonUtils
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(STXS_CommonUtils
  PUBLIC
    ${ROOT_LIBRARIES}
  PRIVATE
    Analysis
)
