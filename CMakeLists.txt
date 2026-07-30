cmake_minimum_required(VERSION 3.19)
project(PaniCPacker LANGUAGES CXX)

#Compile QT UI
set(CMAKE_AUTOUIC ON)

#Compile QT Signals, functions, etc.
set(CMAKE_AUTOMOC ON)

#Compile resources in  general
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(PaniCPacker
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resources.qrc
    channelstate.h
    textureprocessor.h
    textureprocessor.cpp
)

target_link_libraries(PaniCPacker
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS PaniCPacker
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET PaniCPacker
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
