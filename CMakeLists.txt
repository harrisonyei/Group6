cmake_minimum_required(VERSION 3.1.0)

project(StreamWindow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_package(Qt5 COMPONENTS Core Widgets Gui OpenGL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

IF (WIN32)
    find_package(LibAV REQUIRED)
ELSE (WIN32)
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    find_path(LIBAV_INCLUDE_DIR libavformat/avformat.h REQUIRED)
    find_library(AVFORMAT_LIBRARIES avformat REQUIRED)
    find_library(AVCODEC_LIBRARIES avcodec REQUIRED)
    find_library(AVUTIL_LIBRARIES avutil REQUIRED)
    set(LIBAV_LIBRARIES
        ${AVCODEC_LIBRARIES}
        ${AVFORMAT_LIBRARIES}
        ${AVUTIL_LIBRARIES}
    )
ENDIF (WIN32)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src/)

set(SRC_GUI
    ${SRC_DIR}GUI/StreamMainWindow.qrc
    ${SRC_DIR}GUI/StreamMainWindow.h
    ${SRC_DIR}GUI/StreamMainWindow.cpp
    ${SRC_DIR}GUI/StreamMainWindow_ui.h
    ${SRC_DIR}GUI/StreamMainWindow_ui.cpp
    ${SRC_DIR}GUI/DisplayGLWidget.h
    ${SRC_DIR}GUI/DisplayGLWidget.cpp
    )

set(SRC_COMPONENTS
    ${SRC_DIR}Components/DataType.h
    ${SRC_DIR}Components/Component.h
    ${SRC_DIR}Components/Component.cpp
    ${SRC_DIR}Components/Capture.h
    ${SRC_DIR}Components/Capture.cpp
    ${SRC_DIR}Components/ScreenCapture.h
    ${SRC_DIR}Components/ScreenCapture.cpp
    ${SRC_DIR}Components/Controller.h
    ${SRC_DIR}Components/Controller.cpp
    ${SRC_DIR}Components/Encoder.h
    ${SRC_DIR}Components/Encoder.cpp
    ${SRC_DIR}Components/Decoder.h
    ${SRC_DIR}Components/Decoder.cpp
    ${SRC_DIR}Components/Render.h
    ${SRC_DIR}Components/Render.cpp
    ${SRC_DIR}Components/Router.h
    ${SRC_DIR}Components/Router.cpp
    )

set(SRC_TEST
    ${SRC_DIR}Test/test.cpp
    ${SRC_DIR}Components/Component.h
    ${SRC_DIR}Components/Component.cpp
    ${SRC_DIR}Test/ComponentTest.cpp
    )

include_directories(${SRC_DIR})
include_directories(${LIBAV_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})

add_Definitions("-D_XKEYCHECK_H")
add_Definitions("-D_USE_MATH_DEFINES")

add_executable(StreamWindow
    ${SRC_DIR}main.cpp
    ${SRC_GUI}
    ${SRC_COMPONENTS}
)

source_group("GUI" FILES ${SRC_GUI})
source_group("Components" FILES ${SRC_COMPONENTS})

target_link_libraries(StreamWindow 
    ${OpenCV_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    ${LIBAV_LIBRARIES}
    )

target_link_libraries(StreamWindow 
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::OpenGL
    Threads::Threads
    )

IF (WIN32)
    target_link_libraries(StreamWindow 
        wsock32.lib
    )
    find_path(OpenCV_BIN 
        NAMES opencv_world451.dll
        HINTS ${OpenCV_DIR}
        PATH_SUFFIXES x64/vc15/bin
        REQUIRED)

    find_path(LIBAV_BIN
        NAMES avcodec-58.dll
        HINTS ${LIBAV_DIR}
        PATH_SUFFIXES bin)

    set(RUNTIME_DLLS
        ${OpenCV_BIN}/opencv_world451.dll
        ${LIBAV_BIN}/avcodec-58.dll
        ${LIBAV_BIN}/avdevice-58.dll
        ${LIBAV_BIN}/avfilter-7.dll
        ${LIBAV_BIN}/avformat-58.dll
        ${LIBAV_BIN}/avutil-56.dll
        ${LIBAV_BIN}/postproc-55.dll
        ${LIBAV_BIN}/swresample-3.dll
        ${LIBAV_BIN}/swscale-5.dll)

    foreach(DLL ${RUNTIME_DLLS})
        add_custom_command(TARGET StreamWindow POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${DLL}
            $<TARGET_FILE_DIR:StreamWindow>)
    endforeach(DLL $RUNTIME_DLLS)
ELSE (WIN32)
    target_compile_options(StreamWindow PUBLIC "-pthread")
    add_executable(StreamWindowTest ${SRC_TEST})
    target_link_libraries(StreamWindowTest
        ${OpenCV_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
        ${LIBAV_LIBRARIES}
        ${GTEST_LIBRARY}
        ${GTEST_MAIN_LIBRARY}
        pthread)
    enable_testing()
    add_test(NAME GTest COMMAND StreamWindowTest)
ENDIF (WIN32)
