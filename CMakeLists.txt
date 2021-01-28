project(SOUL-LSP)

set(CMAKE_CXX_STANDARD 17)

include_directories(include vendor)

add_executable(soul-lsp source/main.cpp source/Server.cpp)