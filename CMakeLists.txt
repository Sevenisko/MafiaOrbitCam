cmake_minimum_required(VERSION 3.8)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Bin)

project(MafiaOrbitCam VERSION "0.1.0")

configure_file("Config.tmpl" "${CMAKE_CURRENT_LIST_DIR}/Include/Config.h")

add_subdirectory(Vendors/MinHook)
add_subdirectory(Vendors/reMafia)

include_directories("Include" "Vendors/reMafia" "Vendors/MinHook" "Vendors/DirectX/Include")
link_directories("Vendors/DirectX/Lib")

add_library(MafiaOrbitCam SHARED
"Source/Main.cpp"
)

set_property(TARGET MafiaOrbitCam PROPERTY CXX_STANDARD 98)
set_property(TARGET MafiaOrbitCam PROPERTY CXX_STANDARD_REQUIRED ON)
set_target_properties(MafiaOrbitCam PROPERTIES SUFFIX ".asi")

target_link_libraries(MafiaOrbitCam MinHook reMafia d3d8)
target_link_options(MafiaOrbitCam PUBLIC "$<$<C_COMPILER_ID:MSVC>:-SAFESEH:NO>")