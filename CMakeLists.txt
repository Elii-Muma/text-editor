cmake_minimum_required(VERSION 3.15)
project(text-ctrl)

set(CMAKE_CXX_STANDARD 17)

# SFML install path
set(SFML_ROOT "C:/SFML")

include_directories()
link_directories(${SFML_ROOT}/lib)

add_executable(text-ctrl
                main.cpp
                src/camera.cpp
                src/Cursor.cpp
                src/controller.cpp
                src/view.cpp
                src/bottom_view.cpp
                src/bottom_view_controller.cpp
                src/main_view.cpp
                src/view_controller.cpp
                src/main_view_controller.cpp
                src/text_buffer.cpp
                src/Renderer.cpp
                src/document.cpp
            )
# Add your custom include folder
target_include_directories( text-ctrl PRIVATE 
                            ${CMAKE_CURRENT_SOURCE_DIR}/include
                            ${SFML_ROOT}/include
                        )
target_link_libraries(text-ctrl
    sfml-graphics
    sfml-window
    sfml-system
    opengl32
    freetype
    winmm
    gdi32
)
if(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE -mconsole)
 # Automatically copy SFML DLLs after build (Windows only)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${SFML_ROOT}/bin"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
endif()
