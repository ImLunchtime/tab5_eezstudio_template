# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

# Add eez-framework as an extra component directory
set(EXTRA_COMPONENT_DIRS eez-framework)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(m5stack-tab5-lvgl)
