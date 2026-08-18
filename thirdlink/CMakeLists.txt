add_executable(thirdlink)
target_sources(thirdlink
  PRIVATE
    thirdlink.cpp
)

add_library(settings INTERFACE)
target_sources(settings
  INTERFACE
  FILE_SET HEADERS
  FILES
    settings.h
)

add_subdirectory(module)
add_subdirectory(render)

target_link_libraries(thirdlink
  PRIVATE
    settings
    module
    render
)
