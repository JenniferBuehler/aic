# Generate the skill's protobuf files, protobuf library, and descriptor set file.
intrinsic_sdk_protobuf_generate(
  NAME echo_skill
  SOURCES src/echo_skill.proto
  TARGET echo_skill_protos
  DESCRIPTOR_SET_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/echo_skill_protos.desc"
)

# Generate the skill config file.
intrinsic_sdk_generate_skill_config(
  TARGET echo_skill_config
  SKILL_NAME echo_skill
  MANIFEST src/echo_skill.manifest.textproto
  PROTO_DESCRIPTOR_FILE "${CMAKE_CURRENT_BINARY_DIR}/echo_skill_protos.desc"
  SKILL_CONFIG_FILE_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/echo_skill_config.pbbin"
)

install(
  FILES
    src/echo_skill.manifest.textproto
    "${CMAKE_CURRENT_BINARY_DIR}/echo_skill_config.pbbin"
    "${CMAKE_CURRENT_BINARY_DIR}/echo_skill_protos.desc"
  DESTINATION share/${PROJECT_NAME}
)

# Generate the c++ main file for the skill.
intrinsic_sdk_generate_skill_main_cc(
  MANIFEST src/echo_skill.manifest.textproto
  HEADER_FILES src/echo_skill.h
  MAIN_FILE_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/echo_skill_main.cc"
)

add_executable(echo_skill_main
  src/echo_skill.cc
  ${CMAKE_CURRENT_BINARY_DIR}/echo_skill_main.cc
)

target_include_directories(echo_skill_main
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/src
)

# Pure-compute skill: only the SDK + generated protos are required.
# rclcpp is linked defensively (matches sibling skills / generated main);
# remove it if you want a strictly ROS-free target and the build still links.
target_link_libraries(echo_skill_main
  intrinsic_sdk_cmake::intrinsic_sdk_cmake
  echo_skill_protos
  rclcpp::rclcpp
)

add_dependencies(echo_skill_main echo_skill_config)
add_dependencies(echo_skill_main echo_skill_protos)

install(
  TARGETS echo_skill_main
  DESTINATION lib/${PROJECT_NAME}
)

# Offline unit test of the skill's core logic (echo_logic.h) — gtest only, no
# cloud / SDK test framework. (The SDK's C++ SkillTestFactory ships header-only
# in intrinsic_sdk_cmake here, so it isn't linkable; see echo_skill_test.cc.)
if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(echo_skill_test src/echo_skill_test.cc)
  target_include_directories(echo_skill_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(echo_skill_test absl::strings absl::time)
endif()
