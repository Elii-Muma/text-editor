cmake_minimum_required(VERSION 3.15)
project(text-editor)

set(CMAKE_CXX_STANDARD 17)

# SFML install path
set(SFML_ROOT "C:/SFML")

include_directories()
link_directories(${SFML_ROOT}/lib)

add_executable(text-editor
                main.cpp
                src/cursor.cpp
            )
# Add your custom include folder
target_include_directories( text-editor PRIVATE 
                            ${CMAKE_CURRENT_SOURCE_DIR}/include
                            ${SFML_ROOT}/include
                        )
target_link_libraries(text-editor
    sfml-graphics
    sfml-window
    sfml-system
    opengl32
    freetype
    winmm
    gdi32
)
# Automatically copy SFML DLLs after build (Windows only)
if(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE -mconsole)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${SFML_ROOT}/bin"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
endif()
