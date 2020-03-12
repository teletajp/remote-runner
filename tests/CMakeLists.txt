#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests")
#if(NOT fmt::fmt)
#	find_package(fmt CONFIG REQUIRED)
#endif()
####### rrprotocol_test #################################
# set(RR_PROTO_SRC_FILES  rrprotocol_test.cpp
#						../../common/rrprotocol.h
#						../../common/rrprotocol.cpp
#						../../tools/hexutil/hexutil.cpp
#						../../tools/hexutil/hexutil.h)
# add_executable(rrprotocol_test ${RR_PROTO_SRC_FILES})
# target_link_libraries(rrprotocol_test GTest::GTest fmt::fmt)
# target_compile_definitions(rrprotocol_test PRIVATE -D_CRT_SECURE_NO_WARNINGS)
#
# add_test(rrprotocol_test "rrprotocol_test")
# #set_tests_properties( rrprotocol_test PROPERTIES WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
####### console_view_test #################################
#set(CONSOLE_VIEW_SRC_FILES  console_view_test.cpp
#						../src/gui/console_view.h
#						../src/gui/console_view.cpp)
# add_executable(console_view_test ${CONSOLE_VIEW_SRC_FILES})
# target_link_libraries(console_view_test GTest::GTest)
# target_compile_definitions(console_view_test PRIVATE -D_CRT_SECURE_NO_WARNINGS)
# target_compile_options(console_view_test
# PRIVATE 
#	$<$<CXX_COMPILER_ID:GNU>:-Werror> # Флаги для GNU компилятора
#	$<$<CXX_COMPILER_ID:MSVC>:>) # Флаги для MSVC компилятора
#
# add_test(console_view_test console_view_test)
# #set_tests_properties( console_view_test PROPERTIES WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
