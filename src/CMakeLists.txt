project (KUI_project)

cmake_minimum_required(VERSION 2.6)

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})

set(KUI_project_SRCS main.cpp KUI_project.cpp KUI_mainToolBar.cpp)

kde4_add_executable(KUI_project ${KUI_project_SRCS})

target_link_libraries(KUI_project ${KDE4_KDEUI_LIBS})