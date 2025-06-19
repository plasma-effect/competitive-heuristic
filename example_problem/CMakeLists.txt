add_executable(gen generator.cpp)
target_compile_options(gen PRIVATE
  -Wall
  -Wextra
  -Werror=return-type
)

add_executable(vis visualizer.cpp)
target_compile_options(vis PRIVATE
  -Wall
  -Wextra
  -Werror=return-type
)
