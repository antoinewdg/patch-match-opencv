cmake_minimum_required(VERSION 2.8)
project(patch_match)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES
        src/patch_distances.h
        src/patch_servers/pixel_iterators.h
        src/patch_servers/whole_image_patches.h
        src/patch_servers/masked_patches.h
        src/patch_matcher.h
        src/patch_matcher_utils.h
        src/maps/maps_2d.h
        )

set(TEST_FILES
        test/test_utils.h
        test/test_patch_distances.cpp
        test/test_patch_matcher.cpp
        test/patch_servers/test_whole_image_patches.cpp
        test/patch_servers/test_masked_patches.cpp
        )


include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/test
)


add_executable(patch_match src/main.cpp ${SOURCE_FILES})
add_executable(run_tests test/main.cpp ${SOURCE_FILES} ${TEST_FILES})


# EXTERNAL LIBRAIRIES =====================

# OpenCV
FIND_PACKAGE(OpenCV 3.1 REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
set(TARGET_LIBS ${TARGET_LIBS} ${OpenCV_LIBS})

find_package(Boost 1.36.0 REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif ()


TARGET_LINK_LIBRARIES(patch_match ${TARGET_LIBS})
TARGET_LINK_LIBRARIES(run_tests ${TARGET_LIBS})