cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(tests VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

set(SERVER_TEST ON)
set(SERVER_TEST_FILENAME server_test)

set(AGENT_TEST ON)
set(AGENT_TEST_FILENAME agent_test)

if(SERVER_TEST)
    file(GLOB CPPS
                "${PROJECT_SOURCE_DIR}/src/${SERVER_TEST_FILENAME}.cpp"
                "${PROJECT_SOURCE_DIR}/../server/src/*")

    add_compile_options(-DUSE_ARMA -Wno-deprecated -Wno-narrowing -Wno-conversion-null -Wdeprecated-declarations -Wignored-attributes)

    add_executable(${SERVER_TEST_FILENAME} ${CPPS})

    target_include_directories(${SERVER_TEST_FILENAME} PRIVATE 
                                                    "${PROJECT_SOURCE_DIR}/../server/include"     
                                                    "${PROJECT_SOURCE_DIR}/../external/mlpack/src"
                                                    "${PROJECT_SOURCE_DIR}/../external/ctsa/header"
                                                    "${PROJECT_SOURCE_DIR}/../external/fmt/include"
                                                    "${PROJECT_SOURCE_DIR}/../external/dotenv-cpp/include/laserpants/dotenv")
    target_link_directories(${SERVER_TEST_FILENAME} PRIVATE 
                                                    "${PROJECT_SOURCE_DIR}/../external/ctsa/Bin/"
                                                    "${PROJECT_SOURCE_DIR}/../external/fmt/build")
    target_link_libraries(${SERVER_TEST_FILENAME} PRIVATE pqxx armadillo ctsalib.a fmt gtest_main gtest)

    add_test(test_server ${SERVER_TEST_FILENAME})
endif()


if(AGENT_TEST)
    file(GLOB CPPS "${PROJECT_SOURCE_DIR}/src/${AGENT_TEST_FILENAME}.cpp"
                    "${PROJECT_SOURCE_DIR}/../agent/src/*")

    add_compile_options(-Wignored-attributes)

    add_executable(${AGENT_TEST_FILENAME} ${CPPS})

    target_include_directories(${AGENT_TEST_FILENAME} PRIVATE 
                                                    "${PROJECT_SOURCE_DIR}/../agent/include"
                                                    "${PROJECT_SOURCE_DIR}/../external/fmt/include"
                                                    "${PROJECT_SOURCE_DIR}/../external/dotenv-cpp/include/laserpants/dotenv")
    target_link_directories(${AGENT_TEST_FILENAME} PRIVATE 
                                                    "${PROJECT_SOURCE_DIR}/../external/fmt/build")                                              
    target_link_libraries(${AGENT_TEST_FILENAME} PRIVATE pqxx pq fmt ssl crypto gtest_main gtest gmock)

    add_test(test_agent ${AGENT_TEST_FILENAME})
endif()