# The minimum version that is required to build the project. 
cmake_minimum_required(VERSION 3.16)
# Pulls in the rest of the CMake functionality to configure the project, discover all the components etc....
include($ENV{IDF_PATH}/tools/cmake/project.cmake)


set(EXTRA_COMPONENT_DIRS managed-components )
# Define the project name
project(CAN_APP)
