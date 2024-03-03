#
# Specify minimum CMake version and project name
#
cmake_minimum_required (VERSION 3.0)
project (MyProject)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(SHADERS_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")


# ______________DEBUG AND RELEASE CONFIGS______________

# for prod
# add_compile_definitions("NDEBUG")

# GLFW options

add_subdirectory (libraries/glfw)
add_subdirectory(libraries/glm)
# target_link_libraries(MyProject glfw)
# add_library(glfw SHARED IMPORTED GLOBAL)
find_package(Vulkan REQUIRED)

#
# Add files
#
file (GLOB_RECURSE PROJECT_HEADERS src/*.hpp
                                   src/*.h)
file (GLOB_RECURSE PROJECT_SOURCES src/*.cpp
                                   src/*.cxx
                                   src/*.cc
                                   src/*.c)

file(MAKE_DIRECTORY ${SHADERS_OUTPUT_DIR})
file (GLOB_RECURSE PROJECT_SHADERS "shaders/*.comp"
                                   "shaders/*.frag"
                                   "shaders/*.geom"
                                   "shaders/*.glsl"
                                   "shaders/*.vert")
# file (GLOB         PROJECT_CONFIGS CMakeLists.txt
#                                    README.rst
#                                   .gitignore
#                                   .gitmodules)

source_group ("headers" FILES ${PROJECT_HEADERS})
source_group ("shaders" FILES ${PROJECT_SHADERS})
source_group ("sources" FILES ${PROJECT_SOURCES})
# source_group ("vendors" FILES ${VENDORS_SOURCES})

add_custom_target(
    shaders
    COMMENT "Compiling shaders"
    VERBATIM
)

foreach(SHADER_SOURCE ${PROJECT_SHADERS})
  message(STATUS "Compiling shader: ${SHADER_FILENAME}"`)
  get_filename_component(SHADER_FILENAME ${SHADER_SOURCE} NAME)
  set(SHADER_OUTPUT_PATH "${SHADERS_OUTPUT_DIR}/${SHADER_FILENAME}.spv")

  # Add a custom command to compile the shader
  add_custom_command(
      TARGET shaders
      COMMAND glslc ${SHADER_SOURCE} -o ${SHADER_OUTPUT_PATH}
      COMMENT "Compiling ${SHADER_FILENAME}"
      DEPENDS ${SHADER_SOURCE}
      VERBATIM
  )
endforeach()


#
# Set executable and target link libraries
#
add_executable (${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                                ${PROJECT_CONFIGS}
                                ${VENDORS_SOURCES})
add_dependencies(${PROJECT_NAME} shaders)

target_link_libraries (${PROJECT_NAME}
                       glfw
                       glm
                       Vulkan::Vulkan
                       ${GLFW_LIBRARIES})

set_target_properties (${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)