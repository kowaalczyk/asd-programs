set (MECZE src/asd-mecze.cpp)
set (SORTOWANIE_KOMORKOWE src/sortowanie-komorkowe.cpp)
set (MATRYCA src/matryca.cpp)
set (BAZAREK src/bazarek.cpp)
set (VAR_SIZE src/var-size.c)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall")

add_executable(mecze ${MECZE})
add_executable (sortowanie_komorkowe ${SORTOWANIE_KOMORKOWE})
add_executable(matryca ${MATRYCA})
add_executable(bazarek ${BAZAREK})
add_executable(check_variable_sizes ${VAR_SIZE})
