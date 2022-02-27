
project(freecplus)

include_directories(${PROJECT_SOURCE_DIR}/)

file(GLOB_RECURSE lib_sources ${
  PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.cc)
add_library(${PROJECT_NAME} STATIC ${lib_sources})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/)
