set(EXEC_AGENT_NAME agent)

find_package(fmt REQUIRED)
find_package(libpqxx REQUIRED)
find_package(laserpants_dotenv REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

set(DEVELOPER_MODE false)
set(ENV_PATH "${PEOJECT_SOURCE_DIR}/agent/.env")
configure_file (config.hpp.in "${PROJECT_SOURCE_DIR}/agent/include/config.hpp" @ONLY)

file(GLOB 
         CPPS "main.cpp"
              "src/**")

add_compile_options(-Wno-ignored-attributes -fprofile-instr-generate -fcoverage-mapping -fsanitize=address)
add_link_options(-fprofile-instr-generate -fcoverage-mapping -fsanitize=address)

add_executable(${EXEC_AGENT_NAME} ${CPPS})

target_include_directories(${EXEC_AGENT_NAME} PRIVATE 
                                                "include"
                                                "${PROJECT_SOURCE_DIR}/external/dotenv-cpp/include/laserpants/dotenv")
                                                                                           
target_link_libraries(${EXEC_AGENT_NAME} pq fmt ssl crypto laserpants::dotenv pqxx)
