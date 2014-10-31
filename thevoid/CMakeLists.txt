project(src)
cmake_minimum_required(VERSION 2.6)

file(GLOB_RECURSE THEVOID_SRC_LIST "./*.cpp")
add_library(thevoid SHARED ${THEVOID_SRC_LIST})

message("Boost_LIBRARIES: ${Boost_LIBRARIES}")

find_package(PkgConfig REQUIRED)

pkg_search_module(LIBIDN REQUIRED libidn)

include_directories(${LIBIDN_INCLUDE_DIRS})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(thevoid ${Boost_LIBRARIES} pthread uriparser ${LIBIDN_LIBRARIES})

#install(FILES
#	server.hpp
#	stream.hpp
#	streamfactory.hpp
#	http_request.hpp
#  http_response.hpp
#  DESTINATION include/thevoid/
#  )
install(DIRECTORY ./ DESTINATION include/thevoid
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
    )

install(TARGETS thevoid
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib${LIB_SUFFIX}
    BUNDLE DESTINATION library
    )
