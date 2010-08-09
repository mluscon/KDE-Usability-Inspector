project(KUI_project)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)


include(${CMAKE_ROOT}/Modules/FindPkgConfig.cmake)
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(KUI_project_SRCS KUI_project.cpp main.cpp KUI_record.cpp KUI_select.cpp)
set(KUI_project_RCCS KUI_resource.qrc)

qt4_automoc(${KUI_project_SRCS})
add_executable(KUI_project ${KUI_project_SRCS} ${QtApp_RCCS} )
target_link_libraries(KUI_project ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} )

QT4_ADD_RESOURCES(KUI_project_RCC_SRCS ${QtApp_RCCS})
QT4_WRAP_CPP(KUI_project_MOC_SRCS ${QtApp_MOCS})

include(FindGStreamer.cmake) 
include_directories( ${GSTREAMER_INCLUDE_DIR}) 
add_definitions(${GSTREAMER_DEFINITIONS}) 
target_link_libraries(KUI_project ${GSTREAMER_LIBRARIES} ${GSTREAMER_INTERFACE_LIBRARY})

