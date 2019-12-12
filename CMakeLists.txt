cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(librkdb LANGUAGES C)

set(DESCRIPTION "librkdb projects")

include(clang-format)
include(cppcheck)

if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux" OR NOT DEFINED CMAKE_SYSTEM_PROCESSOR)
    message(FATAL_ERROR "This project can only be built for linux")
endif()

find_package(Glib REQUIRED)

set(LIBRKDB_INC ${PKG_GLIB_INCLUDE_DIRS})
set(LIBRKDB_LIB glib-2.0 sqlite3 json-c ${CMAKE_THREAD_LIBS_INIT})
set(LIBRKDB_SRC rkdb.c)

add_library(rkdb SHARED ${LIBRKDB_SRC})
target_include_directories(rkdb PUBLIC ${LIBRKDB_INC})
target_link_libraries(rkdb ${LIBRKDB_LIB})

option(LIBRKDB_EXAMPLE_BUILD  "Build tests executables" ON)
if (${LIBRKDB_EXAMPLE_BUILD})
link_libraries(
	pthread m
	dbus-1 dbus-glib-1
	glib-2.0 gio-2.0 gobject-2.0
	readline json-c sqlite3 rkdb)
add_executable(rkdb_demo demo/main.c)
endif()


