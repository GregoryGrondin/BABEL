cmake_minimum_required(VERSION 3.10.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(Babel)

#COLORS FOR UNIX
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}IX)[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

#MODULES
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

#INCLUDES DIR
include_directories(./client/inc)
include_directories(./server/inc)
conan_basic_setup(TARGETS)

#LIBS
find_package(Qt5Widgets CONFIG REQUIRED)

#FLAGS FOR COMPILATION
if (UNIX)
  message("${BoldYellow}Unix system detected${ColourReset}")
  set(GCC_WFLAGS "-Wall -fPIC")
  add_definitions(${GCC_WFLAGS})
else (UNIX)
  message("${BoldYellow}Unix system detected${ColourReset}")
endif (UNIX)

#SOURCE FILES

add_executable(babel_client ./client/src/Audio.cpp
                            ./client/src/main.cpp
                            ./client/src/client.cpp
                            ./client/src/Encoder.cpp
                            ./client/src/Socket.cpp
                            ./client/src/mainwindow.ui
                            ./client/src/mainwindow.cpp ./client/src/mainwindowNetwork.cpp ./client/inc/mainwindow.hpp)

add_executable(babel_server ./server/src/ClientInfo.cpp
                            ./server/src/Server.cpp
			    ./server/src/DBManager.cpp
			    ./server/src/main.cpp
                            ./server/src/CoHandler.cpp)

#LIBRARIES
if (UNIX)
TARGET_LINK_LIBRARIES(babel_client CONAN_PKG::boost CONAN_PKG::opus CONAN_PKG::portaudio Qt5::Widgets)
TARGET_LINK_LIBRARIES(babel_server CONAN_PKG::boost CONAN_PKG::opus CONAN_PKG::portaudio CONAN_PKG::sqlite3)
endif (UNIX)
