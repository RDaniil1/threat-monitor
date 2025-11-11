set(EXEC_SERV_NAME server)

find_package(fmt REQUIRED)
find_package(Armadillo REQUIRED)
find_package(cereal REQUIRED)
find_package(ensmallen REQUIRED)
find_package(laserpants_dotenv REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD 17)

set(MODEL_PATH "${PROJECT_SOURCE_DIR}/data/model.bin")
set(DEVELOPER_MODE false)
set(ENV_PATH "${PROJECT_SOURCE_DIR}/.env")
configure_file (config.hpp.in "${PROJECT_SOURCE_DIR}/include/config.hpp" @ONLY)

file(
    GLOB
        CPPS "main.cpp" 
        "src/**")

add_compile_options(-fprofile-instr-generate -fcoverage-mapping -DUSE_ARMA -Wno-deprecated -Wno-narrowing -Wno-conversion-null -fsanitize=address)
add_link_options(-fprofile-instr-generate -fcoverage-mapping -fsanitize=address)

add_executable(${EXEC_SERV_NAME} ${CPPS})

target_include_directories(${EXEC_SERV_NAME} PRIVATE 
                                                "include"
                                                "${PROJECT_SOURCE_DIR}/external/mlpack/src/mlpack/core"
                                                "${PROJECT_SOURCE_DIR}/external/ctsa/header"
                                                "${PROJECT_SOURCE_DIR}/external/ctsa/src"
                                                "${PROJECT_SOURCE_DIR}/external/dotenv-cpp/include/laserpants/dotenv")
target_link_directories(${EXEC_SERV_NAME} PRIVATE 
                                                "${PROJECT_SOURCE_DIR}/external/ctsa/Bin/")
target_link_libraries(${EXEC_SERV_NAME} gcov pqxx pq armadillo ctsalib.a fmt laserpants::dotenv)
