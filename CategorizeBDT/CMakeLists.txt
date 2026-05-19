add_executable(${PROJECT_NAME}-train 
  bin/train.cpp
)

add_executable(${PROJECT_NAME}-bdt 
  bin/application.cpp
)

install(TARGETS ${PROJECT_NAME}-train ${PROJECT_NAME}-bdt 
  RUNTIME DESTINATION bin
)

add_library(STXS_BDT_Train 
  src/Train2jHadParams.cpp 
  src/TrainStep.cpp
  src/parseArgsTrain.cpp
)

target_link_libraries(STXS_BDT_Train
  PRIVATE 
    STXS_CommonUtils
)

add_library(STXS_BDT_Application
  src/parseArgsApply.cpp
  src/BDTClassifier.cpp
)

#[[
add_library(STXS_BDT_Common

)
]]

target_link_libraries(${PROJECT_NAME}-train
  PUBLIC
    ${ROOT_LIBRARIES}
  PRIVATE
    STXS_CommonUtils
    # STXS_BDT_Common
    STXS_BDT_Train
)

target_link_libraries(${PROJECT_NAME}-bdt
  PUBLIC
    ${ROOT_LIBRARIES}
  PRIVATE
    STXS_BDT_Application
    STXS_CommonUtils
    # STXS_BDT_Common
)


