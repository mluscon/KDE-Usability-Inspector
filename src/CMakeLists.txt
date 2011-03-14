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
                      KUI_ScreenShotLabel.cpp
                      KUI_CameraWidget.cpp
                      KUI_NewProjectDialog.cpp
                      KUI_record.cpp
                      model/domitem.cpp
                      model/dommodel.cpp
                      )

kde4_add_executable(KUI_project ${KUI_project_SRCS} ${KUI_model_SRCS})

target_link_libraries(KUI_project ${KDE4_KDEUI_LIBS}
                                  ${GSTREAMER_LIBRARIES} 
                                  ${GSTREAMER_INTERFACE_LIBRARY}
                                  ${KDE4_PHONON_LIBRARY}
                                  ${X11_LIBRARIES}
                                  ${GOBJECT_LIBRARIES}
                                  ${KDE4_KIO_LIBRARY}
                                  gstcontroller-0.10
                                  gobject-2.0
                                  )

