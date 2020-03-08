cmake_minimum_required(VERSION 2.6)
set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Escolha o tipo de build (Debug ou Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Nome do projeto
project(graphODA)
# Versionamento
set (graphODA_VERSION_MAJOR 1)
set (graphODA_VERSION_MINOR 0)

# Headers
include_directories(include)
include_directories(include/views)
include_directories(include/models/graph)
include_directories(include/utils)
include_directories(include/controllers/repl)
include_directories(include/models/dataStructures)

## Dependências
if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    # Se estamos rodando localmente para desenvolvimento, relaxamos as dependências
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_debug_packages" ${CMAKE_MODULE_PATH})
endif()
find_package(SFML 2.4 COMPONENTS graphics window system REQUIRED)
find_package(TGUI 0.8 REQUIRED)

set(SRC_FILES
        "${PROJECT_SOURCE_DIR}/src/views/Buttons.cpp"
        "${PROJECT_SOURCE_DIR}/src/views/GraphDisplay.cpp"
        "${PROJECT_SOURCE_DIR}/src/views/GraphCanvas.cpp"
        "${PROJECT_SOURCE_DIR}/src/views/Interface.cpp"
        "${PROJECT_SOURCE_DIR}/src/models/graph/GraphGen.cpp"
        "${PROJECT_SOURCE_DIR}/src/models/graph/Graph.cpp"
        "${PROJECT_SOURCE_DIR}/src/models/graph/Bipartite.cpp"
        "${PROJECT_SOURCE_DIR}/src/models/graph/Chordal.cpp"
        "${PROJECT_SOURCE_DIR}/src/models/graph/Dag.cpp"
        "${PROJECT_SOURCE_DIR}/src/models/graph/Tree.cpp"
        "${PROJECT_SOURCE_DIR}/src/controllers/repl/Repl.cpp"
        "${PROJECT_SOURCE_DIR}/src/models/dataStructures/Vector.cpp"
        "${PROJECT_SOURCE_DIR}/src/models/dataStructures/DSU.cpp")

# Objetos comuns a todos os targets
add_library(common OBJECT ${SRC_FILES})

## Configuração dos testes
add_subdirectory (tests)

## Configuração para compilação do executável final
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
add_executable(graphODA $<TARGET_OBJECTS:common> "${PROJECT_SOURCE_DIR}/src/main.cpp")
target_link_libraries(graphODA tgui sfml-graphics sfml-window sfml-system)

add_custom_target(run
	DEPENDS graphODA
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/graphODA"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(repl
	DEPENDS graphODA
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/graphODA" "-r"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Gera toda a documentação do projeto
add_custom_target(docs
	DEPENDS tests
    COMMAND "./build_docs.sh"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
