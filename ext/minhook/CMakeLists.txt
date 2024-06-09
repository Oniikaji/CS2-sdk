file(GLOB MINHOOK_INCLUDE CONFIGURE_DEPENDS
  ${MINHOOK_DIR}/*.h
  ${MINHOOK_DIR}/*.c
)

add_library(
  MINHOOK_LIB STATIC
  ${MINHOOK_INCLUDE}
)

target_link_libraries(
  MINHOOK_LIB
)

target_include_directories(
  MINHOOK_LIB PUBLIC
  ${THIRD_DIR}/minhook
  ${WINDOWS_INCLUDE}
  ${IMGUI_DIR}
)