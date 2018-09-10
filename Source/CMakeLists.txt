
if(MAGMA_SHARED)
add_library(magma SHARED Backend.cc Console.cc SDLBackend.cc Error.cc)
target_compile_definitions(magma PRIVATE MAGMA_DLL_BUILD)
target_include_directories(magma PRIVATE .)
target_include_directories(magma PUBLIC ../Include)
target_include_directories(magma PUBLIC ../Dependencies/sdl2/include)
target_link_libraries(magma SDL2 lodepng fmt)
endif()

if(BUILD_STATIC_LIBS)
add_library(magma-static STATIC Backend.cc Console.cc SDLBackend.cc Error.cc)
target_compile_definitions(magma PRIVATE MAGMA_STATIC_BUILD)
target_include_directories(magma-static PRIVATE .)
target_include_directories(magma-static PUBLIC ../Include)
target_include_directories(magma-static PUBLIC ../Dependencies/sdl2/include)
target_link_libraries(magma-static SDL2 lodepng fmt)
endif()


