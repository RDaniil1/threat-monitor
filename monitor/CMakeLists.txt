set(EXEC_MONITOR_NAME monitor)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DEVELOPER_MODE false)
set(ENV_PATH "${PROJECT_SOURCE_DIR}/monitor/.env")
configure_file (config.hpp.in "${PROJECT_SOURCE_DIR}/monitor/config.hpp" @ONLY)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql PrintSupport Charts)
find_package(laserpants_dotenv REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        login.cpp
        "${PROJECT_SOURCE_DIR}/external/qcustomplot-source/qcustomplot.cpp"

        mainwindow.h
        login.h
        "${PROJECT_SOURCE_DIR}/external/qcustomplot-source/qcustomplot.h"

        mainwindow.ui
        login.ui
)

add_compile_options(-Wno-deprecated-declarations)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${EXEC_MONITOR_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Monitor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${EXEC_MONITOR_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${EXEC_MONITOR_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(${EXEC_MONITOR_NAME} PRIVATE 
                                                # "/usr/include/postgresql"
                                                "${PROJECT_SOURCE_DIR}/external/qcustomplot-source")

# target_link_directories(${PROJECT_NAME} PRIVATE 
#                                                 "/usr/lib/postgresql/15/lib")

target_link_libraries(${EXEC_MONITOR_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core 
                                      Qt${QT_VERSION_MAJOR}::Widgets 
                                      Qt${QT_VERSION_MAJOR}::Sql 
                                      Qt${QT_VERSION_MAJOR}::PrintSupport 
                                      Qt${QT_VERSION_MAJOR}::Charts
                                      laserpants::dotenv)

set_target_properties(${EXEC_MONITOR_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${EXEC_MONITOR_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${EXEC_MONITOR_NAME})
endif()
