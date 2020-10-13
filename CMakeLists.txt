cmake_minimum_required(VERSION 3.16)
project(Simple)

set(CMAKE_CXX_STANDARD 17)

add_executable(Simple main.cpp
        Engine/Engine.cpp
        Engine/Engine.h
        Engine/Entity.h
        Engine/Input.h
        Engine/Scheduler.h

        Language/Instructions.cpp
        Language/Instructions.h
        Language/Lexer.cpp
        Language/Lexer.h
        Language/AST.cpp
        Language/AST.h
        Language/Parser.cpp
        Language/Parser.h
        Language/Tokens.h
        Language/Assembler.cpp
        Language/Assembler.h
        Language/VirtualMachine.cpp
        Language/VirtualMachine.h

        Simulation/Agent.h
        Simulation/Agent.cpp
        Simulation/Detection.h
        Simulation/Detection.cpp

        Utils/Maze.h
        Utils/RandomNumberGenerator.cpp
        Utils/RandomNumberGenerator.h
        )


find_package(SFML REQUIRED COMPONENTS system graphics window network audio)

set(SFML_LIBRARIES "-lsfml-graphics -lsfml-window -lsfml-system")

TARGET_LINK_LIBRARIES(Simple pthread)

if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()