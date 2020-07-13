cmake_minimum_required(VERSION 2.8)

project( pola )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# adapt for macports:
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  include_directories ("/opt/local/include/opencv4")
  link_directories("/opt/local/lib")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pola pola.cpp )
target_link_libraries (pola ${EXTRA_LIBS} ${OpenCV_LIBS}  )

INSTALL(TARGETS pola DESTINATION bin)
