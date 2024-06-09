file(GLOB IMGUI_INCLUDE CONFIGURE_DEPENDS 
  ${IMGUI_DIR}/*.h
  ${IMGUI_DIR}/*.cpp
)

add_library(
  IMGUI_LIB STATIC
  ${IMGUI_INCLUDE}
)

target_link_libraries(
  IMGUI_LIB
)

target_include_directories(
  IMGUI_LIB PUBLIC
  ${THIRD_DIR}/imgui
  ${WINDOWS_INCLUDE}
  ${IMGUI_DIR}
)