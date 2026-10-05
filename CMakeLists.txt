cmake_minimum_required(VERSION 3.16)

project(GPXReader
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)

add_subdirectory(glfw/glfw-3.5.1)


add_executable(GPXReader
    main.cpp

    tinyxml2.cpp

    # Dear ImGui
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp

    # Dear ImGui GLFW/OpenGL3 backends
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp

    # ImPlot
    implot/implot.cpp
    implot/implot_items.cpp
)

target_include_directories(GPXReader PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/implot
)

target_link_libraries(GPXReader PRIVATE
    glfw
    OpenGL::GL
)