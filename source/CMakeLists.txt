add_subdirectory(events)
add_subdirectory(gui)
add_subdirectory(managers)
add_subdirectory(systems)
add_subdirectory(utils)

set(HEADER_FILES ${HEADER_FILES}
  ${CMAKE_CURRENT_SOURCE_DIR}/editorState.hpp
  PARENT_SCOPE)

set(SOURCE_FILES ${SOURCE_FILES}
  ${CMAKE_CURRENT_SOURCE_DIR}/editorState.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  PARENT_SCOPE)
