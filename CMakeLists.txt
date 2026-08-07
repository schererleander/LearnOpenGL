cmake_minimum_required(VERSION 3.25)

project(learnopengl LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

add_library(glad STATIC src/gl.c)
target_include_directories(glad PUBLIC include)

add_library(assimp SHARED IMPORTED)

if(APPLE)
    set_target_properties(assimp PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/macos/libassimp.dylib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include"
    )
elseif(UNIX)
    set_target_properties(assimp PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/linux/libassimp.so.6.0.5"
        IMPORTED_SONAME "libassimp.so.6"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include"
        INTERFACE_LINK_OPTIONS "-Wl,-rpath,${CMAKE_SOURCE_DIR}/lib/linux"
    )
endif()

function(add_example name source)
    add_executable(${name} ${source})
    target_compile_definitions(${name} PRIVATE GLFW_INCLUDE_NONE)
    target_link_libraries(${name} PRIVATE glad glfw assimp)
endfunction()

add_example(hello_window src/01_getting_started/hello_window/main.cpp)

add_example(hello_triangle src/01_getting_started/hello_triangle/main.cpp)

add_example(hello_triangle_exercise_01 src/01_getting_started/hello_triangle_exercise1/main.cpp)

add_example(hello_triangle_exercise_02 src/01_getting_started/hello_triangle_exercise2/main.cpp)

add_example(hello_triangle_exercise_03 src/01_getting_started/hello_triangle_exercise3/main.cpp)

add_example(shaders_uniform src/01_getting_started/shaders_uniform/main.cpp)

add_example(shaders_more_attributes src/01_getting_started/shaders_more_attributes/main.cpp)

add_example(shaders_class src/01_getting_started/shaders_class/main.cpp)

add_example(shaders_exercise1 src/01_getting_started/shaders_exercise1/main.cpp)

add_example(shaders_exercise2 src/01_getting_started/shaders_exercise2/main.cpp)

add_example(shaders_exercise3 src/01_getting_started/shaders_exercise3/main.cpp)

add_example(textures src/01_getting_started/textures/main.cpp)

add_example(textures_multiple src/01_getting_started/textures_multiple/main.cpp)

add_example(textures_exercise1 src/01_getting_started/textures_exercise1/main.cpp)

add_example(textures_exercise2 src/01_getting_started/textures_exercise2/main.cpp)

add_example(textures_exercise3 src/01_getting_started/textures_exercise3/main.cpp)

add_example(textures_exercise4 src/01_getting_started/textures_exercise4/main.cpp)

add_example(transformations src/01_getting_started/transformations/main.cpp)

add_example(transformations_exercise1 src/01_getting_started/transformations_exercise1/main.cpp)

add_example(transformations_exercise2 src/01_getting_started/transformations_exercise2/main.cpp)

add_example(coordinate_systems src/01_getting_started/coordinate_systems_going_3d/main.cpp)

add_example(coordinate_systems_cube src/01_getting_started/coordinate_systems_cube/main.cpp)

add_example(coordinate_systems_cube_multiple src/01_getting_started/coordinate_systems_cube_multiple/main.cpp)

add_example(coordinate_systems_exercise1 src/01_getting_started/coordinate_systems_exercise1/main.cpp)

add_example(coordinate_systems_exercise2 src/01_getting_started/coordinate_systems_exercise2/main.cpp)

add_example(coordinate_systems_exercise3 src/01_getting_started/coordinate_systems_exercise3/main.cpp)

add_example(camera src/01_getting_started/camera/main.cpp)

add_example(camera_movement src/01_getting_started/camera_movement/main.cpp)

add_example(camera_mouse_zoom src/01_getting_started/camera_mouse_zoom/main.cpp)

add_example(camera_class src/01_getting_started/camera_class/main.cpp)

add_example(camera_exercise1 src/01_getting_started/camera_exercise1/main.cpp)

add_example(camera_exercise2 src/01_getting_started/camera_exercise2/main.cpp)

add_example(colors src/02_lighting/lighting_colors/main.cpp)

add_example(basic_lighting_diffuse src/02_lighting/basic_lighting_diffuse/main.cpp)

add_example(basic_lighting_specular src/02_lighting/basic_lighting_specular/main.cpp)

add_example(basic_lighting_exercise1 src/02_lighting/basic_lighting_exercise1/main.cpp)

add_example(basic_lighting_exercise2 src/02_lighting/basic_lighting_exercise2/main.cpp)

add_example(materials src/02_lighting/materials/main.cpp)

add_example(materials_exercise1 src/02_lighting/materials_exercise1/main.cpp)

add_example(materials_exercise2 src/02_lighting/materials_exercise2/main.cpp)

add_example(lighting_maps_diffuse src/02_lighting/lighting_maps_diffuse/main.cpp)

add_example(lighting_maps_specular src/02_lighting/lighting_maps_specular/main.cpp)

add_example(lighting_maps_exercise2 src/02_lighting/lighting_maps_exercise2/main.cpp)

add_example(lighting_maps_exercise3 src/02_lighting/lighting_maps_exercise3/main.cpp)

add_example(lighting_maps_exercise4 src/02_lighting/lighting_maps_exercise4/main.cpp)

add_example(light_casters_directional src/02_lighting/light_casters_directional/main.cpp)

add_example(light_casters_point src/02_lighting/light_casters_point/main.cpp)

add_example(light_casters_spot src/02_lighting/light_casters_spot/main.cpp)

add_example(light_casters_spot_soft src/02_lighting/light_casters_spot_soft/main.cpp)

add_example(multiple_lights src/02_lighting/multiple_lights/main.cpp)

add_example(multiple_lights_exercise1 src/02_lighting/multiple_lights_exercise1/main.cpp)

add_example(model_loading src/03_model_loading/model_loading/main.cpp)

add_example(model_loading_lights src/03_model_loading/model_loading_lights/main.cpp)
