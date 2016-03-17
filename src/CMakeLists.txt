set(CMAKE_CXX_FLAGS "-std=c++1y -Wall -pedantic -flto")

file(GLOB Chess_SRC
    "*.h"
    "*.cpp")

add_executable(Chess ${Chess_SRC})

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

install(TARGETS Chess RUNTIME DESTINATION bin)
target_link_libraries(Chess SDL2 SDL2_image pthread) 
