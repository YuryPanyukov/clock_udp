add_executable(ClockServer
    main.cpp
    serverwindow.cpp
    serverwindow.h
    serverwindow.ui
)

target_link_libraries(ClockServer PRIVATE
    common
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
)

qt_deploy_windows(ClockServer)