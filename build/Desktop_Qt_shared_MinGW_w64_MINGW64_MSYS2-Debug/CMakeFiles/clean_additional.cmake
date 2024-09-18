# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\String_sort_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\String_sort_autogen.dir\\ParseCache.txt"
  "String_sort_autogen"
  )
endif()
