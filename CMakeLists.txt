cmake_minimum_required(VERSION 3.1)

project(main)

set(HEADER_FILES simpletest.h)
set(SOURCE_FILES test.c
    test_demo.c
    )

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

add_custom_target(run ./${PROJECT_NAME} || echo "Abnormal exit"
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "run binaray"
    )
