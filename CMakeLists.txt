set (SOURCE_FILES sortowanie-komorkowe.cpp)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall")


add_executable (sortowanie ${SOURCE_FILES})
