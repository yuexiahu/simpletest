cmake_minimum_required(VERSION 3.1)

project(main)

file(GLOB HEADER_FILES "*.h")
file(GLOB SOURCE_FILES "*.c")

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

add_custom_target(run ./${PROJECT_NAME} || echo "Abnormal exit"
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "run binaray"
    )
