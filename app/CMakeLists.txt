# Build the app, using the HAL

include_directories(include)
file(GLOB MY_SOURCES "src/*.c")
add_executable(lighten_blinken ${MY_SOURCES})

# Make use of the HAL library
target_link_libraries(lighten_blinken LINK_PRIVATE hal)

# Copy executable to final location (change `hello_world` to project name as needed)
add_custom_command(TARGET lighten_blinken POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:lighten_blinken>"
     "~/cmpt433/public/myApps/lighten_blinken" 
  COMMENT "Copying ARM executable to public NFS directory")
