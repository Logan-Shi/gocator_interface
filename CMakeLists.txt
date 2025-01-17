cmake_minimum_required(VERSION 2.8.3)

# Cmake policy
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0005 NEW) 
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# Project name and the type of project
PROJECT(gocator_3200)
message(STATUS "Executig CMakeLists of project  ${PROJECT_NAME}.")

# DEBUG/RELEASE
IF (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "RELEASE") 
ENDIF (NOT CMAKE_BUILD_TYPE)
message(STATUS "Configured to compile in ${CMAKE_BUILD_TYPE} mode.")

#Set Flags
SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -D_REENTRANT")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -D_REENTRANT")

## Find catkin macros and libraries
find_package(
    catkin REQUIRED 
    COMPONENTS 
        roscpp 
        rospy 
        dynamic_reconfigure 
#         message_generation
        std_msgs
        sensor_msgs 
        pcl_ros
        pcl_conversions
)

# Find point cloud Library
FIND_PACKAGE(PCL 1.9 REQUIRED COMPONENTS)
INCLUDE_DIRECTORIES(${PCL_INCLUDE_DIRS})
LINK_DIRECTORIES(${PCL_LIBRARY_DIRS})
ADD_DEFINITIONS(${PCL_DEFINITIONS})

# Set GO_SDK include paths (That could be part of a FindGocator.cmake)
SET(GO_SDK_4 /home/logan/Documents/measuring/GO_SDK) # TODO: Should be an env variable!!
FIND_PATH(
    GOCATOR_INCLUDES
    NAMES GoSdk/GoSdk.h
    PATHS ${GO_SDK_4}/Gocator/GoSdk)
FIND_PATH(
    KAPI_INCLUDES
    NAMES kApi/kApi.h
    PATHS ${GO_SDK_4}/Platform/kApi)
INCLUDE_DIRECTORIES(${GOCATOR_INCLUDES} ${KAPI_INCLUDES})

# Set GO_SDK libs (That could be part of a FindGocator.cmake)
FIND_LIBRARY(
    GOCATOR_LIBRARIES
    NAMES GoSdk
    PATHS ${GO_SDK_4}/lib/linux_x64/) 
FIND_LIBRARY(
    KAPI_LIBRARIES
    NAMES kApi
    PATHS ${GO_SDK_4}/lib/linux_x64/) 
message(STATUS "Gocator Headers found at ${GOCATOR_INCLUDES}.")
message(STATUS "Gocator Libraries found at ${GOCATOR_LIBRARIES}.")

# add other necessary include directories
include_directories(src/ src/lib/src/ ${catkin_INCLUDE_DIRS})

# Set source files for the library
SET(SRCS_LIB 
    src/lib/src/gocator3200.cpp)
    
# Set header files for the library
SET(HDRS_LIB 
    src/lib/src/gocator3200.h)

# Set source files for the ROS node
SET(SRCS_NODE 
    src/gocator_3200_node.cpp
    src/gocator_3200_node_main.cpp
)
    
# add_service_files(
#   FILES
#   PointCloudAsService.srv
# )

## Generate added messages and services
# generate_messages(
#       DEPENDENCIES 
#       std_msgs 
#       sensor_msgs
# )

#dynamic reconfigure cfg file
generate_dynamic_reconfigure_options(cfg/gocator_3200_params.cfg)

## The catkin_package macro generates cmake config files for your package
catkin_package(
    CATKIN_DEPENDS 
        roscpp 
        rospy 
        dynamic_reconfigure 
#         message_runtime 
        std_msgs
        sensor_msgs
        pcl_ros
        pcl_conversions
)

#Build library (TODO: Should call a CMakeLists.txt at src/lib/)
ADD_LIBRARY(${PROJECT_NAME}_LIB SHARED ${SRCS_LIB})
TARGET_LINK_LIBRARIES(${PROJECT_NAME}_LIB ${GOCATOR_LIBRARIES} ${KAPI_LIBRARIES} ${PCL_LIBRARIES})

#Build executable
ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS_NODE} ${SRCS_LIB})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${GOCATOR_LIBRARIES} ${KAPI_LIBRARIES} ${PCL_LIBRARIES} ${catkin_LIBRARIES})
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_gencpp ${PROJECT_NAME}_gencfg)

#Build examples TODO: Should be called from a CMakeLists at at src/lib/
ADD_SUBDIRECTORY(src/lib/src/examples)

## Declare a cpp executable
# add_executable(btr_point_tracker_node ${SRCS} src/btr_point_tracker_main.cpp)
# add_dependencies(btr_point_tracker_node ${PROJECT_NAME}_EXPORTED_TARGETS)
# target_link_libraries(btr_point_tracker_node ${OpenCV_LIBS} ${catkin_LIBRARIES})


