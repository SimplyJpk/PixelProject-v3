set_property(DIRECTORY PROPERTY KEEP_EXTENSION_TARGETS_IN_NORMAL_TARGETS TRUE)

cmake_minimum_required(VERSION 3.5)
project(PixelProject)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

# Configure compiler flags for different build types
if(MSVC)
  # Visual Studio flags
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob2 /Oi /Ot /GL /Gy /MD /DNDEBUG")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
else()
  # GCC/Clang flags
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -DNDEBUG")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Bin Folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Exclude Some Windows Stuff
if (WIN32)
    add_compile_definitions(NOMINMAX)
endif()

# CPM
file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

set(LIBS)


## Dependencies

# ImGui
CPMAddPackage(
        NAME imgui
        GITHUB_REPOSITORY ocornut/imgui
        GIT_TAG f5befd2
        DOWNLOAD_ONLY
)
if (imgui_ADDED)
    add_library(imgui STATIC)
    target_sources(imgui PRIVATE
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_SOURCE_DIR}/imgui_widgets.cpp
            ${imgui_SOURCE_DIR}/imgui_demo.cpp
            # SDL 3
            ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
            # SDL 3 Renderer
            ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
            # OpenGL 3 backend - ADD THIS LINE
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}")
    target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}/backends")
    # Add OpenGL dependency to imgui
    target_link_libraries(imgui PRIVATE SDL3::SDL3 ${OPENGL_LIBRARIES})
    list(APPEND LIBS imgui)
endif ()

# ImPlot
CPMAddPackage(
        NAME implot
        GITHUB_REPOSITORY epezent/implot
        GIT_TAG master
        DOWNLOAD_ONLY
)

if (implot_ADDED)
    add_library(implot STATIC)
    target_sources(implot PRIVATE
        ${implot_SOURCE_DIR}/implot.cpp
        ${implot_SOURCE_DIR}/implot_items.cpp
        ${implot_SOURCE_DIR}/implot_demo.cpp
    )
    target_include_directories(implot PUBLIC ${implot_SOURCE_DIR})
    target_link_libraries(implot PUBLIC imgui)
    list(APPEND LIBS implot)
endif ()

# SDL
CPMAddPackage("gh:libsdl-org/SDL#877399b")
list(APPEND LIBS SDL3::SDL3)
if (imgui_ADDED)
    target_link_libraries(imgui PRIVATE SDL3::SDL3)
endif ()

# SDL_image
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE) # Requires Perl and Nasm for libaom (AVIF image format)
CPMAddPackage("gh:libsdl-org/SDL_image#11154af")
list(APPEND LIBS SDL3_image::SDL3_image)

# SDL3_ttf
set(SDLTTF_FREETYPE ON CACHE BOOL "" FORCE) # Requires FreeType
set(SDLTTF_HARFBUZZ ON CACHE BOOL "" FORCE) # Requires HarfBuzz
CPMAddPackage("gh:libsdl-org/SDL_ttf#a1ce367")
list(APPEND LIBS SDL3_ttf::SDL3_ttf)

# Boost
find_package(Boost 1.87.0 REQUIRED COMPONENTS thread system)

if(Boost_FOUND)
    list(APPEND LIBS Boost::thread Boost::system)
endif()

# cereal
CPMAddPackage("gh:uscilab/cereal#ebef1e9")
list(APPEND LIBS cereal)

# # SQLite
# CPMAddPackage("gh:SRombauts/SQLiteCpp#643b153")
# list(APPEND LIBS SQLiteCpp)

# Add GLEW package - modify this section
find_package(OpenGL REQUIRED)
CPMAddPackage(
    NAME GLEW
    GITHUB_REPOSITORY "Perlmint/glew-cmake"
    GIT_TAG "glew-cmake-2.2.0"
)
if(GLEW_ADDED)
    list(APPEND LIBS libglew_static)
endif()

# GLM
CPMAddPackage("gh:g-truc/glm#0af55cc")
list(APPEND LIBS glm::glm)

# STB
CPMAddPackage(
    NAME stb
    GITHUB_REPOSITORY nothings/stb
    GIT_TAG master
    DOWNLOAD_ONLY YES
)

if(stb_ADDED)
    add_library(stb INTERFACE)
    target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
    list(APPEND LIBS stb)
endif()

# FastNoise2
CPMAddPackage(
    NAME FastNoise2
    GITHUB_REPOSITORY Auburn/FastNoise2
    GIT_TAG master
    OPTIONS
        "FASTNOISE2_NOISETOOL OFF"
        "FASTNOISE2_TESTS OFF"
)

if(FastNoise2_ADDED)
    list(APPEND LIBS FastNoise2)
endif()

## Executables
set(PROJECT_NAME "PixelProject")
add_executable(${PROJECT_NAME} PixelProject/main.cpp)

file(GLOB_RECURSE SOURCES "PixelProject/*.cpp")
file(GLOB_RECURSE HEADERS "PixelProject/*.h")
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS} ${OPENGL_LIBRARIES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${LIBS}
)

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE 
        # Main Project
        ${CMAKE_CURRENT_SOURCE_DIR}/PixelProject/Scripts
        # GLM
        ${CMAKE_CURRENT_SOURCE_DIR}/include/glm
)

if (Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
endif()

# Definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)
target_compile_definitions(${PROJECT_NAME} PRIVATE PIXEL_PROJECT_DEBUG)

# After Building, copy shaders and textures folders into the bin folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/PixelProject/shaders
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/PixelProject/textures
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/textures
)