# FetchContent_Declare( 
#   mavlink
#   GIT_REPOSITORY https://github.com/mavlink/mavlink.git
#   GIT_TAG        1.0.12
# )

# add_library(mavlink_common INTERFACE)

# target_compile_definitions(mavlink_common
#   INTERFACE
#     MAVLINK_DIALECT=common
#     MAVLINK_VERSION=2.0
# )

# FetchContent_MakeAvailable(mavlink)

add_compile_options(
  -Wno-address-of-packed-member
)
