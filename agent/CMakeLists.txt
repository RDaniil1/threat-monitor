cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(Agent LANGUAGES CXX)
find_package(fmt REQUIRED)
find_package(libpqxx REQUIRED)
find_package(laserpants_dotenv REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

set(DEVELOPER_MODE false)
set(ENV_PATH "${PROJECT_SOURCE_DIR}/.env")
configure_file (config.hpp.in "${PROJECT_SOURCE_DIR}/include/config.hpp" @ONLY)

file(GLOB 
         CPPS "main.cpp"
              "${PROJECT_SOURCE_DIR}/src/**")

add_compile_options(-Wno-ignored-attributes -fsanitize=address)
add_link_options(-fsanitize=address)

add_executable(${PROJECT_NAME} ${CPPS})

target_include_directories(${PROJECT_NAME} PRIVATE 
                                                "${PROJECT_SOURCE_DIR}/include"
                                                "${PROJECT_SOURCE_DIR}/../external/dotenv-cpp/include/laserpants/dotenv")
                                                                                           
target_link_libraries(${PROJECT_NAME} pqxx pq fmt ssl crypto laserpants::dotenv)

