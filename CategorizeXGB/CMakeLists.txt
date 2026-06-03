add_executable(${PROJECT_NAME}-xdg-bdt
bin/ApplyBDT.cpp
)

install(TARGETS ${PROJECT_NAME}-xdg-bdt
  RUNTIME DESTINATION bin
)

#[[ 
add_library(XGBApply
  src/STXS_categorization0.cpp
  src/STXS_categorization1p2.cpp
  src/STXS_categorization1p3.cpp
  src/parseArgs.cpp
)
]]

target_link_libraries(${PROJECT_NAME}-xdg-bdt
  PUBLIC
    ${ROOT_LIBRARIES}
    ROOT::TMVA
  PRIVATE
    #Scripts
    STXS_CommonUtils
)

#[[target_include_directories(XGBApply
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
]]
