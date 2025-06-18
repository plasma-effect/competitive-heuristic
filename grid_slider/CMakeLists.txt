add_executable(create create_grid_data.cpp)
target_compile_options(create PRIVATE
  -Wall
  -Wextra
  -Werror=return-type
)
