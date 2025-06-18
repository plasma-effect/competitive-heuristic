cmake_minimum_required(VERSION 3.17)
project(CompetitiveHeuristic LANGUAGES CXX)

set(HOME_DIR $ENV{HOME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main main.cpp)
target_include_directories(main PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/competitive-library/ac-library/custom
  ${HOME_DIR}/boost_1_82_0/include
)
target_compile_options(main PRIVATE
  -Wall
  -Wextra
  -Werror=return-type
)
if(COMPETITIVE_BUILD_MODE STREQUAL "Develop")
  target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
  )
  target_compile_options(main PRIVATE -g)
  target_compile_definitions(main PRIVATE LOCAL_DEBUG)
  target_link_libraries(main PRIVATE stdc++exp)
else()
  target_compile_options(main PRIVATE -O3)
  target_compile_definitions(main PRIVATE LOCAL_MEASURE)
endif()

add_executable(score_manager score_manager.cpp)
target_include_directories(score_manager PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/json/include
)
target_compile_options(score_manager PRIVATE
  -Wall
  -Wextra
  -Werror=return-type
)

add_subdirectory(grid_slider)
