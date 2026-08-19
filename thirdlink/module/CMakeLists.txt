add_library(module STATIC)
target_sources(module
  PRIVATE
    module.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES
      module.h
)
