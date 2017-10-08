set (SOURCE_FILES bazarek.cpp)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall")


add_executable (bazarek ${SOURCE_FILES})
