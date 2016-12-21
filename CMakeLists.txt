cmake_minimum_required(VERSION 2.8)
project(patch_match_opencv)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES
        include/patch_match_opencv/distances/euclidian.h
        include/patch_match_opencv/patch_servers/pixel_iterators.h
        include/patch_match_opencv/patch_servers/whole_image_patches.h
        include/patch_match_opencv/patch_servers/masked_patches.h
        include/patch_match_opencv/patch_servers/base_patch_server.h
        include/patch_match_opencv/maps/distance_map_2d.h
        include/patch_match_opencv/maps/offset_map_2d.h
        include/patch_match_opencv/adapters.h
        )

set(TEST_FILES
        test/test_utils.h
        test/patch_match_opencv/test_patch_distances.cpp
        test/patch_match_opencv/patch_servers/test_whole_image_patches.cpp
        test/patch_match_opencv/patch_servers/test_masked_patches.cpp
        test/patch_match_opencv/test_patch_matcher.cpp
        )


include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/test
        ${CMAKE_CURRENT_SOURCE_DIR}/core/include
)


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


TARGET_LINK_LIBRARIES(run_tests ${TARGET_LIBS})