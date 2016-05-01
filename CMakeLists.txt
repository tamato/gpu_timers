###############################################################################
#
# A generalized cmake file for developing cross-platform games.
#
cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
#set (CMAKE_VERBOSE_MAKEFILE ON)

# Every project must have a name.
project ("gpu_timers")

################################################################################
# Add in support for c++11
list( APPEND CMAKE_CXX_FLAGS "-std=c++11")

################################################################################
# Set up the type of build to use. i.e. Debug or Release?
if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This messes up
  # differentiation between debug and release builds.               
  # use the following to the desired build type from the command line:
  #   cmake -D CMAKE_BUILD_TYPE=Release
  set (CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

###########################################
# Add GLAD
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/glad)
set(GLAD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/glad.c)

###########################################
# Add GLFW, and have it built with the project
set(BUILD_SHARED_LIBS OFF CACHE STRING "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE STRING "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE STRING "" FORCE)
set(GLFW_INSTALL OFF CACHE STRING "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE STRING "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include)

###########################################
# Add GLM
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/glm)

###########################################
# Add pnglite
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/pnglite)
set(PNGLITE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/external/pnglite/pnglite.c)
# CMAKE_REQUIRED_LIBRARIES("z") # pnglite requires libz

###########################################
# Add openCTM
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/openctm/OpenCTM-1.0.3/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/openctm/OpenCTM-1.0.3/lib/)

################################################################################
# The core project files
file (GLOB SRCS *.cpp )
file (GLOB COMMON_SOURCE common/*.cpp)

add_executable( 
  ${PROJECT_NAME} 
  ${SRCS} 
  ${COMMON_SOURCE} 
  ${GLAD_SOURCE}
  ${PNGLITE_SOURCE}
  )

target_link_libraries( ${PROJECT_NAME} glfw z openctm)

add_custom_target(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/data)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${PROJECT_NAME}>/data)
