add_library(render STATIC)
target_sources(render
  PRIVATE
    render.cpp
  PUBLIC
   FILE_SET HEADERS
   FILES
    render.h
)

target_link_libraries(render
  PRIVATE
    raylib
    module
    settings
)
