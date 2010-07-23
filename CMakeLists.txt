project(KUI_project)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)


include(${CMAKE_ROOT}/Modules/FindPkgConfig.cmake)
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(KUI_project_SRCS KUI_project.cpp main.cpp KUI_record.cpp)
qt4_automoc(${KUI_project_SRCS})
add_executable(KUI_project ${KUI_project_SRCS})
target_link_libraries(KUI_project ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} )

include(FindGStreamer.cmake) 
include_directories( ${GSTREAMER_INCLUDE_DIR}) 
add_definitions(${GSTREAMER_DEFINITIONS}) 
target_link_libraries(KUI_project ${GSTREAMER_LIBRARIES} ${GSTREAMER_INTERFACE_LIBRARY})

