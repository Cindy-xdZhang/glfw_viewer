# glad
add_subdirectory(./glad/ ./glad/)
set(INCLUDE_DIRS ${INCLUDE_DIRS};${CMAKE_CURRENT_SOURCE_DIR}/glad/include/)

# glfw
add_subdirectory(./glfw/ ./glfw/)
set(INCLUDE_DIRS ${INCLUDE_DIRS};${CMAKE_CURRENT_SOURCE_DIR}/glfw/include/)

# imgui
#add_subdirectory(./imgui/ ./imgui/)
#set(INCLUDE_DIRS ${INCLUDE_DIRS};${CMAKE_CURRENT_SOURCE_DIR}/imgui/)

# Eigen
set(INCLUDE_DIRS ${INCLUDE_DIRS};${CMAKE_CURRENT_SOURCE_DIR}/eigen/)

# stb
#set(INCLUDE_DIRS ${INCLUDE_DIRS};${CMAKE_CURRENT_SOURCE_DIR}/stb/)

# OpenMesh
#add_subdirectory(./OpenMesh/ ./OpenMesh/)
#set(INCLUDE_DIRS ${INCLUDE_DIRS};${CMAKE_CURRENT_SOURCE_DIR}/OpenMesh/src/)

# include
set(INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
