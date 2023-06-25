find_package(SDL2 REQUIRED)

add_executable(
    ex16
    main.c
    game.c
    algo.c
    record.c
)
target_compile_options(
    ex16
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -std=c11
)
target_include_directories(
    ex16
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2TTF_INCLUDE_DIRS}
)
target_link_libraries(
    ex16
    PRIVATE
    ${SDL2_LIBRARIES} 
    ${SDL2TTF_LIBRARY} 
    -lSDL2_mixer
    -lm
)    

FILE(COPY ${CMAKE_SOURCE_DIR}/res/ARCADE.TTF             DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
FILE(COPY ${CMAKE_SOURCE_DIR}/res/mixkit-game-action.wav DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
FILE(COPY ${CMAKE_SOURCE_DIR}/res/solve-the_puzzle.ogg   DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

