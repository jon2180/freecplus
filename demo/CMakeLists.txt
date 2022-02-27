project(freecplus-demo)

link_libraries(freecplus)

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/*.cpp)

foreach (pine_test_source ${sources})
    get_filename_component(pine_test_filename ${pine_test_source} NAME)
    string(REPLACE ".cpp" "" pine_test_name ${pine_test_filename})
    string(REPLACE ".c" "" pine_test_name_final ${pine_test_name})
    add_executable(${pine_test_name_final} ${pine_test_source})
endforeach(pine_test_source ${sources})
