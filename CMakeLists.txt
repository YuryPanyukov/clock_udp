cmake_minimum_required(VERSION 3.16)

project(clock_udp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автогенерация moc/uic/rcc. Включаем ДО find_package(Qt6 ...).
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Автоматически добавляет текущую папку в include-пути — удобно для ui_*.h
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)
# profiles — это просто папка с JSON, отдельный target ей не нужен.