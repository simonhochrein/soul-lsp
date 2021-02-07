project(SOUL-LSP)

set(CMAKE_CXX_STANDARD 17)

include_directories(include vendor)

IF (WIN32)
    add_compile_options(-bigobj)
ENDIF()

add_executable(soul-lsp source/main.cpp source/Server.cpp)

add_executable(resolver source/resolver.cpp)
