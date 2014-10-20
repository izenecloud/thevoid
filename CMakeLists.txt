set(ENV_EXTRA_CMAKE_MODULES_DIRS $ENV{EXTRA_CMAKE_MODULES_DIRS})
if(ENV_EXTRA_CMAKE_MODULES_DIRS)
  set(EXTRA_CMAKE_MODULES_DIRS ${EXTRA_CMAKE_MODULES_DIRS} ${ENV_EXTRA_CMAKE_MODULES_DIRS})
endif(ENV_EXTRA_CMAKE_MODULES_DIRS)
if(NOT EXTRA_CMAKE_MODULES_DIRS)
  message(FATAL_ERROR
    "Cannot find EXTRA_CMAKE_MODULES_DIRS.
1. Please clone it from https://ssl.izenesoft.cn/cmake.
2. Then set EXTRA_CMAKE_MODULES_DIRS through environment variable

       export EXTRA_CMAKE_MODULES_DIRS=/path/to/cmake

   You can put it in your .bashrc so it is loaded automatically.

   or cmake options

       cmake  -DEXTRA_CMAKE_MODULES_DIRS=/path/to/cmake (other options)
")
endif(NOT EXTRA_CMAKE_MODULES_DIRS)
set(CMAKE_MODULE_PATH ${EXTRA_CMAKE_MODULES_DIRS})

project(thevoid)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -W -Wall -Wextra")

find_package(blackhole REQUIRED)
find_package(Boost COMPONENTS system thread program_options regex filesystem REQUIRED)

include(cmake/locate_library.cmake)

locate_library(LIBEV "ev++.h" "ev" "libev")
locate_library(CURL "curl.h" "curl" "curl" "libcurl")
locate_library(URIPARSER "Uri.h" "uriparser" "uriparser")
locate_library(LIBXML2 "libxml/xpath.h" "xml2" "libxml2")

include_directories(
    ${CURL_INCLUDE_DIRS}
    ${URIPARSER_INCLUDE_DIRS}
    ${LIBXML2_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${blackhole_INCLUDE_DIR}
)

link_directories(
    ${CURL_LIBRARY_DIRS}
    ${URIPARSER_LIBRARY_DIRS}
    ${LIBXML2_LIBRARY_DIRS}
    ${Boost_LIBRARY_DIRS}
)

aux_source_directory(. THEVOID_SRC_LIST)
add_library(thevoid SHARED ${THEVOID_SRC_LIST})

message("THEVOID_SRC_LIST: ${THEVOID_SRC_LIST}")
message("Boost_LIBRARIES: ${Boost_LIBRARIES}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(swarm)

target_link_libraries(thevoid ${Boost_LIBRARIES} pthread swarm)
set_target_properties(thevoid PROPERTIES
    VERSION ${DEBFULLVERSION}
    SOVERSION ${SWARM_VERSION_ABI}
    )

install(FILES
	server.hpp
	stream.hpp
	streamfactory.hpp
	http_request.hpp
	http_response.hpp
    DESTINATION include/thevoid/
    )

install(TARGETS thevoid
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib${LIB_SUFFIX}
    BUNDLE DESTINATION library
    )
