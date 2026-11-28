cmake_minimum_required(VERSION 3.16)

project(clock_udp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

function(qt_deploy_windows target)
    if(NOT WIN32)
        return()
    endif()

    get_target_property(_qmake_exe Qt6::qmake IMPORTED_LOCATION)
    if(NOT _qmake_exe)
        message(WARNING "qt_deploy_windows: Qt6::qmake not found, skip deploy")
        return()
    endif()
    get_filename_component(_qt_bin_dir "${_qmake_exe}" DIRECTORY)

    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
    if(NOT WINDEPLOYQT_EXECUTABLE)
        message(WARNING "qt_deploy_windows: windeployqt not found in ${_qt_bin_dir}")
        return()
    endif()

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --no-translations
                --no-compiler-runtime
                --dir "$<TARGET_FILE_DIR:${target}>"
                "$<TARGET_FILE:${target}>"
        COMMENT "Deploying Qt runtime for ${target}"
        VERBATIM
    )
endfunction()

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)