add_subdirectory(modules)
add_subdirectory(setup)

set(HEADER_FILES ${HEADER_FILES}
  ${CMAKE_CURRENT_SOURCE_DIR}/gui.hpp
  PARENT_SCOPE)

set(SOURCE_FILES ${SOURCE_FILES}
  ${CMAKE_CURRENT_SOURCE_DIR}/gui.cpp
  PARENT_SCOPE)
