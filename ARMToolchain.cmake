set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(COMPILER_PREFIX arm-none-eabi-)

set(CMAKE_C_COMPILER    ${COMPILER_PREFIX}gcc)
set(CMAKE_CXX_COMPILER  ${COMPILER_PREFIX}g++)
set(CMAKE_LINKER        ${COMPILER_PREFIX}ld CACHE FILEPATH "")
set(CMAKE_GCC_AR        ${COMPILER_PREFIX}gcc-ar CACHE FILEPATH "")
set(CMAKE_GCC_RANLIB    ${COMPILER_PREFIX}gcc-ranlib CACHE FILEPATH "")
set(CMAKE_NM            ${COMPILER_PREFIX}gcc-nm CACHE FILEPATH "")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
