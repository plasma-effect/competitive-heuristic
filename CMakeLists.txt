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
  target_compile_options(main PRIVATE -g -O3 -mavx512f -mfma)
  target_compile_definitions(main PRIVATE LOCAL_DEBUG)
  target_link_libraries(main PRIVATE stdc++exp)
else()
  target_compile_options(main PRIVATE -O3 -mavx512f -mfma)
  target_compile_definitions(main PRIVATE LOCAL_MEASURE)
endif()

add_executable(main_debug main.cpp)
target_include_directories(main_debug PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}  
  ${CMAKE_CURRENT_SOURCE_DIR}/competitive-library/ac-library/custom
  ${HOME_DIR}/boost_1_82_0/include
)
target_compile_options(main_debug PRIVATE -g -mavx512f -mfma
  -Wall
  -Wextra
  -Wshadow
  -Wconversion
  -Wno-sign-conversion
  -Werror
  -fsanitize=undefined,address
  -fno-sanitize-recover=all
)
target_compile_definitions(main_debug PRIVATE LOCAL_DEBUG)
target_link_libraries(main_debug PRIVATE stdc++exp)
target_link_options(main_debug PRIVATE
  -fsanitize=undefined,address
)

add_executable(experiment experiment.cpp)
target_include_directories(experiment PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/competitive-library/ac-library/custom
  ${CMAKE_CURRENT_SOURCE_DIR}/competitive-library/include
  ${HOME_DIR}/boost_1_82_0/include
)
target_compile_options(experiment PRIVATE
  -Wall
  -Wextra
  -Werror=return-type
)
if(COMPETITIVE_BUILD_MODE STREQUAL "Develop")
  target_include_directories(experiment PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
  )
  target_compile_options(experiment PRIVATE -g -O3 -mavx512f -mfma)
  target_compile_definitions(experiment PRIVATE LOCAL_DEBUG)
  target_link_libraries(experiment PRIVATE stdc++exp)
else()
  target_compile_options(experiment PRIVATE -O3 -mavx512f -mfma)
  target_compile_definitions(experiment PRIVATE LOCAL_MEASURE)
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
add_subdirectory(example_problem)
