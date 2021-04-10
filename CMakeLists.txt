cmake_minimum_required(VERSION 3.1.0)

project(TwitchWindow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS OpenGL REQUIRED)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src/)
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/thirdparty/)

set(SRC_COMPONENTS
    ${SRC_DIR}Components/DataType.h
    ${SRC_DIR}Components/Application.h
    ${SRC_DIR}Components/Application.cpp
    ${SRC_DIR}Components/Component.h
    ${SRC_DIR}Components/Capture.h
    ${SRC_DIR}Components/Capture.cpp
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

include_directories(${SRC_DIR})
include_directories(${THIRDPARTY_DIR}ffmpeg/include)
include_directories(${THIRDPARTY_DIR}opencv/include)
include_directories(${THIRDPARTY_DIR}GL/include)

add_Definitions("-D_XKEYCHECK_H")
add_Definitions("-D_USE_MATH_DEFINES")

add_executable(TwitchWindow
    ${SRC_DIR}main.cpp

    ${SRC_DIR}TwitchWindow.ui
    ${SRC_DIR}TwitchWindow.qrc

    ${SRC_DIR}TwitchWindow.h
    ${SRC_DIR}TwitchWindow.cpp

    ${SRC_DIR}MyGLWidget.h
    ${SRC_DIR}MyGLWidget.cpp

    ${SRC_COMPONENTS}
)

source_group("Components" FILES ${SRC_COMPONENTS})

target_link_libraries(TwitchWindow 
    debug ${THIRDPARTY_DIR}opencv/lib/opencv_world451d.lib optimized ${THIRDPARTY_DIR}opencv/lib/opencv_world451.lib
    ${THIRDPARTY_DIR}ffmpeg/lib/avcodec.lib
    ${THIRDPARTY_DIR}ffmpeg/lib/avdevice.lib
    ${THIRDPARTY_DIR}ffmpeg/lib/avfilter.lib
    ${THIRDPARTY_DIR}ffmpeg/lib/avformat.lib
    ${THIRDPARTY_DIR}ffmpeg/lib/avutil.lib
    ${THIRDPARTY_DIR}ffmpeg/lib/postproc.lib
    ${THIRDPARTY_DIR}ffmpeg/lib/swresample.lib
    ${THIRDPARTY_DIR}ffmpeg/lib/swscale.lib
    ${THIRDPARTY_DIR}GL/lib/OpenGL32.lib
    wsock32.lib
    )

target_link_libraries(TwitchWindow 
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::OpenGL
    )
    