cmake_minimum_required(VERSION 3.19)
project(PaniCPacker LANGUAGES CXX)

include(GNUInstallDirs)

#Compile QT UI
set(CMAKE_AUTOUIC ON)

#Compile QT Signals, functions, etc.
set(CMAKE_AUTOMOC ON)

#Compile resources in  general
set(CMAKE_AUTORCC ON)


#Set executable icon
# --- Windows Configuration ---
if(WIN32)
    # Requires a .ico file
    set(WIN_ICON "${CMAKE_CURRENT_SOURCE_DIR}/icons/icon.ico")
    # Creates a basic .rc wrapper file dynamically
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/app.rc" "IDI_ICON1 ICON DISCARDABLE \"${WIN_ICON}\"\n")
    set(PLATFORM_ICON_SRC "${CMAKE_CURRENT_BINARY_DIR}/app.rc")

# --- macOS Configuration ---
elseif(APPLE)
    # Requires a .icns file
    set(MAC_ICON "${CMAKE_CURRENT_SOURCE_DIR}/icons/icon.icns")
    set_source_files_properties("${MAC_ICON}" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set(PLATFORM_ICON_SRC "${MAC_ICON}")
    set(MACOSX_BUNDLE_ICON_FILE icon.icns)

# --- Linux Configuration ---
elseif(UNIX)
    # Linux uses an installed PNG icon and a .desktop entry instead of embedding an icon in the executable.
    set(LINUX_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/icon_app.png")
endif()



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
    exportservice.h
    exportservice.cpp
    popup_about.ui
    popup_about.h
    popup_about.cpp
    ${PLATFORM_ICON_SRC}
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

if(UNIX AND NOT APPLE)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/PaniCPacker.desktop.in"
        "${CMAKE_CURRENT_BINARY_DIR}/PaniCPacker.desktop"
        @ONLY
    )

    install(
        FILES "${LINUX_ICON}"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/icons/hicolor/512x512/apps"
        RENAME "panicpacker.png"
    )
    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/PaniCPacker.desktop"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/applications"
    )
endif()

qt_generate_deploy_app_script(
    TARGET PaniCPacker
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
