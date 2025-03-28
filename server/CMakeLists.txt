cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(Server LANGUAGES C CXX)

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
        "${PROJECT_SOURCE_DIR}/src/**"
        HEADERS "${PROJECT_SOURCE_DIR}/include")

add_compile_options(-DUSE_ARMA -Wno-deprecated -Wno-narrowing -Wno-conversion-null -fsanitize=address)
add_link_options(-fsanitize=address)

add_executable(${PROJECT_NAME} ${CPPS})

target_include_directories(${PROJECT_NAME} PRIVATE 
                                                "${PROJECT_SOURCE_DIR}/include"
                                                "${PROJECT_SOURCE_DIR}/../external/ctsa/header"
                                                "${PROJECT_SOURCE_DIR}/../external/ctsa/src"
                                                "${PROJECT_SOURCE_DIR}/../external/dotenv-cpp/include/laserpants/dotenv")
target_link_directories(${PROJECT_NAME} PRIVATE 
                                                "${PROJECT_SOURCE_DIR}/../external/ctsa/Bin/")
target_link_libraries(${PROJECT_NAME} pqxx pq armadillo ctsalib.a fmt laserpants::dotenv)
