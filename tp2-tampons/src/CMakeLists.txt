cmake_minimum_required(VERSION 3.14)
project(tp2_tampons)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

set(SOURCE_FILES
  Aquarium.h Camera.h Etat.h Pipeline.h Theiere.h inf2705-Singleton.h inf2705-fenetre.h inf2705-forme.h inf2705-matrice.h inf2705-nuanceur.h inf2705-theiere.h
  Etat.cpp main.cpp)

add_executable(tp2_tampons ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLEW_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})

target_link_libraries(tp2_tampons ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2_LIBRARIES})
