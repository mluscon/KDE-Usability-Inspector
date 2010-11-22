project (KUI_project)

cmake_minimum_required(VERSION 2.6)

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})

include(FindGStreamer.cmake) 
include_directories( ${GSTREAMER_INCLUDE_DIR}) 
add_definitions(${GSTREAMER_DEFINITIONS})


set( KUI_project_SRCS main.cpp 
                      KUI_project.cpp
                      KUI_MainToolBar.cpp
                      KUI_screenShotLabel.cpp
                      KUI_KuiCentralWidget.cpp
                      KUI_CameraWidget.cpp
                      KUI_NewProject.cpp
                      KUI_record.cpp )

kde4_add_executable(KUI_project ${KUI_project_SRCS})

target_link_libraries(KUI_project ${KDE4_KDEUI_LIBS} 
				  ${GSTREAMER_LIBRARIES} 
				  ${GSTREAMER_INTERFACE_LIBRARY}
				  ${KDE4_PHONON_LIBRARY}
				  gstcontroller-0.10
				  X11
				  gobject-2.0  )

