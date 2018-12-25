set(HEADER_FILES ${HEADER_FILES}
  ${CMAKE_CURRENT_SOURCE_DIR}/cameraManager.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/collisionCubeManager.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/meshPreviewHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/viewportManager.hpp
  PARENT_SCOPE)

set(SOURCE_FILES ${SOURCE_FILES}
  ${CMAKE_CURRENT_SOURCE_DIR}/cameraManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/collisionCubeManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/meshPreviewHandler.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/viewportManager.cpp
  PARENT_SCOPE)
