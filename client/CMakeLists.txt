add_executable(ClockClient
    main.cpp
    clientwindow.cpp
    clientwindow.h
    clientwindow.ui
    clockface.cpp
    clockface.h
)

target_link_libraries(ClockClient PRIVATE
    common
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
)
