set (SOURCE_FILES matryca.cpp)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall")


add_executable (matryca ${SOURCE_FILES})
