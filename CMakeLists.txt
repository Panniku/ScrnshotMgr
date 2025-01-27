cmake_minimum_required(VERSION 3.5)

project(ScrnshotMgr VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS MultimediaWidgets)

set(PROJECT_SOURCES
        src/main.cpp
	src/mainwindow.cpp
	src/mainwindow.h
	src/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    if(WIN32)
	set(APP_ICON_RESOURCE_WIN "${CMAKE_CURRENT_SOURCE_DIR}/scrnshotmgr.rc")
    endif()
    qt_add_executable(ScrnshotMgr
	MANUAL_FINALIZATION
	${PROJECT_SOURCES}
	${APP_ICON_RESOURCE_WIN}
	src/ui_components/capture/capturecontainer.h src/ui_components/capture/capturecontainer.cpp
	src/ui_components/capture/capturerenderer.h src/ui_components/capture/capturerenderer.cpp
	src/ui_components/logtextedit.h src/ui_components/logtextedit.cpp
	src/ui_components/presets/presetobject.h src/ui_components/presets/presetobject.cpp
	src/configmanager/configmanager.h src/configmanager/configmanager.cpp
	src/logcat/logcat.h src/logcat/logcat.cpp
	src/snapper/snapcrop.h src/snapper/snapcrop.cpp
	src/snapper/snapinterface.h src/snapper/snapinterface.cpp
	src/snapper/snapinterface.ui
	src/resources.qrc
	src/snapper/drawsnap.h src/snapper/drawsnap.cpp
	src/ui_components/capture/capturesimple.h src/ui_components/capture/capturesimple.cpp
	src/ui_components/capture/captureitem.h src/ui_components/capture/captureitem.cpp
	src/utils/utils.h src/utils/utils.cpp
	src/configmanager/settingsdialog.h src/configmanager/settingsdialog.cpp src/configmanager/settingsdialog.ui
	src/ui_components/presets/editor/preseteditor.h src/ui_components/presets/editor/preseteditor.cpp src/ui_components/presets/editor/preseteditor.ui

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ScrnshotMgr APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
	add_library(ScrnshotMgr SHARED
	    ${PROJECT_SOURCES}
	)
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
	add_executable(ScrnshotMgr
	    ${PROJECT_SOURCES}
	)
    endif()
endif()

target_link_libraries(ScrnshotMgr PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ScrnshotMgr PRIVATE Qt${QT_VERSION_MAJOR}::MultimediaWidgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ScrnshotMgr)
endif()
set_target_properties(ScrnshotMgr PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ScrnshotMgr
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ScrnshotMgr)
endif()
