cmake_minimum_required(VERSION 3.12)

project(async

	CXX
)

### Compiler setup
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

### Target setup
file(GLOB_RECURSE HEADER_FILES

	"include/*.h"
	"include/*.hpp"
)

file(GLOB_RECURSE SOURCE_FILES

	"src/*.c"
	"src/*.cc"
	"src/*.cpp"
	"src/*.cxx"
)

add_executable(${PROJECT_NAME}

	${SOURCE_FILES}
	${HEADER_FILES}
)

target_include_directories(${PROJECT_NAME}

	PUBLIC
		"include/"
)

add_custom_target(run

	COMMAND				$<TARGET_FILE:${PROJECT_NAME}>
	DEPENDS				${PROJECT_NAME}
	WORKING_DIRECTORY	${PROJECT_SOURCE_DIR}
)