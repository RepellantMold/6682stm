cmake_minimum_required(VERSION 3.15)
project(c6682stm VERSION 1.0.0 DESCRIPTION "Composer 668 to Scream Tracker 2 module converter" LANGUAGES C)

include(FetchContent)
FetchContent_Declare(c89stringutils GIT_REPOSITORY https://github.com/offscale/c89stringutils.git)
FetchContent_MakeAvailable(c89stringutils)

option(GENERATE_FILENAMES "Use a random string of letters for every present samples file name" OFF)
option(ADVERTISE "Advertise that the converter was used" OFF)

if(ADVERTISE)
	add_compile_definitions(C6682STM_ADVERTISE=1)
endif()
if(GENERATE_FILENAMES)
	add_compile_definitions(C6682STM_GENERATE_RANDOM_FILENAME=1)
endif()

add_executable(${PROJECT_NAME}
			src/main.c
			src/log.c
			src/file.c
			src/pattern.c
			src/668.c
			src/stm.c)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 90 C_STANDARD_REQUIRED ON C_EXTENSIONS OFF)
target_link_libraries(${PROJECT_NAME} PRIVATE c89stringutils)
