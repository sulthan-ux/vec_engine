cmake_minimum_required(VERSION 3.20)
project(EmberVulkan)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

file(GLOB EMBER_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx)

set(EMBER_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(EmberVulkan ${EMBER_LIBRARIES})

target_include_directories( EmberVulkan
    PRIVATE
    ${GLFW_INCLUDE_DIRS}
    ${EMBER_INCLUDE_DIRS}

)

target_link_libraries(EmberVulkan
    PRIVATE
    Vulkan::Vulkan
    ${GLFW_LIBRARIES}
)
