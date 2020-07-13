project( pola )

# adapt for macports - mosquitto cmake not included...:
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  include_directories ("/opt/local/include/opencv4")
  link_directories("/opt/local/lib")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pola pola.cpp )
target_link_libraries (pola ${EXTRA_LIBS} opencv_videoio opencv_highgui opencv_core )

INSTALL(TARGETS pola DESTINATION bin)
