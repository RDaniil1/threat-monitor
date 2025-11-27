find_package(GTest REQUIRED)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

set(SERVER_TEST ON)
set(SERVER_TEST_FILENAME server_test)

set(AGENT_TEST ON)
set(AGENT_TEST_FILENAME agent_test)

if(SERVER_TEST)
    file(GLOB CPPS
                "${PROJECT_SOURCE_DIR}/tests/src/server_test.cpp"
                "${PROJECT_SOURCE_DIR}/server/src/*")

    add_compile_options(-Wno-ignored-attributes -fprofile-instr-generate -fcoverage-mapping -fsanitize=address -DUSE_ARMA 
                        -Wno-deprecated -Wno-narrowing -Wno-conversion-null -Wdeprecated-declarations -Wignored-attributes)
    add_link_options(-fprofile-instr-generate -fcoverage-mapping -fsanitize=address)
    
    enable_testing()
    add_executable(${SERVER_TEST_FILENAME} ${CPPS})

    target_include_directories(${SERVER_TEST_FILENAME} PRIVATE 
                                                    "${PROJECT_SOURCE_DIR}/server/include"     
                                                    "${PROJECT_SOURCE_DIR}/external/mlpack/src"
                                                    "${PROJECT_SOURCE_DIR}/external/ctsa/header"
                                                    "${PROJECT_SOURCE_DIR}/external/fmt/include"
                                                    "${PROJECT_SOURCE_DIR}/external/dotenv-cpp/include/laserpants/dotenv")
    target_link_directories(${SERVER_TEST_FILENAME} PRIVATE 
                                                    "${PROJECT_SOURCE_DIR}/external/ctsa/Bin/"
                                                    "${PROJECT_SOURCE_DIR}/external/fmt/build")
    target_link_libraries(${SERVER_TEST_FILENAME} PRIVATE pqxx pq armadillo ctsalib.a fmt gtest_main gtest)
    
    add_test(server_test ${SERVER_TEST_FILENAME})
endif()


if(AGENT_TEST)
    file(GLOB CPPS "${PROJECT_SOURCE_DIR}/tests/src/agent_test.cpp"
                    "${PROJECT_SOURCE_DIR}/agent/src/*")

    add_compile_options(-Wno-ignored-attributes -fprofile-instr-generate -fcoverage-mapping -fsanitize=address)
    add_link_options(-fprofile-instr-generate -fcoverage-mapping -fsanitize=address)

    add_executable(${AGENT_TEST_FILENAME} ${CPPS})

    target_include_directories(${AGENT_TEST_FILENAME} PRIVATE 
                                                    "${PROJECT_SOURCE_DIR}/agent/include"
                                                    "${PROJECT_SOURCE_DIR}/external/fmt/include"
                                                    "${PROJECT_SOURCE_DIR}/external/dotenv-cpp/include/laserpants/dotenv")
    target_link_directories(${AGENT_TEST_FILENAME} PRIVATE 
                                                    "${PROJECT_SOURCE_DIR}/external/fmt/build")                                              
    target_link_libraries(${AGENT_TEST_FILENAME} PRIVATE GTest::Main GTest::GTest GTest::gmock pqxx pq fmt ssl crypto)

    add_test(agent_test ${AGENT_TEST_FILENAME})
endif()