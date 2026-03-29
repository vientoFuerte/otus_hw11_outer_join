cmake_minimum_required(VERSION 3.12)

project(join_server)

option(WITH_BOOST_TEST "Whether to build Boost test" ON)
option(WITH_DOXYGEN "Build Doxygen documentation" OFF)

set(Boost_NO_BOOST_CMAKE ON)

# Ищем Boost для основной программы
find_package(Boost REQUIRED COMPONENTS filesystem system program_options regex)

add_executable(join_server main.cpp join_server.cpp table.cpp)

set_target_properties(join_server PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(join_server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(join_server PRIVATE 
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_REGEX_LIBRARY}
)

# Boost.Test
if(WITH_BOOST_TEST AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)
    # Для тестов нужен unit_test_framework
    find_package(Boost REQUIRED COMPONENTS unit_test_framework filesystem)
    
    add_executable(join_server_tests test.cpp join_server.cpp table.cpp)
    
    set_target_properties(join_server_tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    
    target_include_directories(join_server_tests PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(join_server_tests PRIVATE ${Boost_LIBRARIES})
    
    target_compile_definitions(join_server_tests PRIVATE BOOST_TEST_NO_LIB)
    
    enable_testing()
    add_test(NAME AllTests COMMAND join_server_tests)
    
    message(STATUS "Boost.Test enabled")
endif()

if (MSVC)
    target_compile_options(join_server PRIVATE /W4)
else ()
    target_compile_options(join_server PRIVATE
        -Wall -Wextra -pedantic
    )
endif()

install(TARGETS join_server RUNTIME DESTINATION bin)

include(CPack)
