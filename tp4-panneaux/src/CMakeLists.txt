cmake_minimum_required(VERSION 3.14)
project(tp4_panneaux)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

set(SOURCE_FILES
  Camera.h Etat.h Pipeline.h inf2705-Singleton.h inf2705-fenetre.h inf2705-forme.h inf2705-matrice.h inf2705-nuanceur.h inf2705-texture.h
  Etat.cpp main.cpp)

add_executable(tp4_panneaux ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLEW_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})

target_link_libraries(tp4_panneaux ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2_LIBRARIES})
