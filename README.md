# Simple

**This code is in the process of being extracted from a genetic programming experiment.
An Experiment in genetic programming over a c-like language.**

Written for video game mechanics and gameplay generation, but generally usable for any realtime simulation where you want to investigate evolution of agents with movement and avoidance.

![Screenshot](./Media/Screenshot_1.png)

In this diagram: 
- Black: Impassible Wall.
- Red: Hurtful Enemy which can move
- Magenta: Player
- Blue: Goal/Exit for player to reach

## Overview & Architecture

Consists of a handful of subsections working together:

- A Simple Programming Language 
- Which compiles via A Tree Structure
- Genetic Algorithms which operate on the tree
- Assembler compiles tree into bytecode
- Simulation which executes the bytecode of the various entities in a shared virtual world. 
- Evaluation Loop and Fitness determination to see who exists for next generation

## Current Project Status

All the parts exist with some bugs, but the evolutionary loop needs some work.

## Project Files & Directories

### Documentation/

Automatically generate by Doxygen.
 - There is a Doxyfile in the main dir, best if you have graphviz/dot installed. Dun using doxygen Doxfile from the root dir of the project.
 - Todo integrate to the build somehow?
 - **Plz do not** submit output to the repo

### Assets/

Various source files in a c-like language that is compiled to a tree for genetic manipulation, mutation and crossover etc.

### Engine/

Simple SFML Entity Framework with Round Robin Scheduler for realtime applications like games/sims. 
Working but Limited in Scope. Probably Leaks. Does three things only.
 - Opens Windows
 - Visits Entities
 - Renders World

### Genetics/

Tree Operations Which can be applied to nodes or tree to adjust their contents.
 - Mutator
     - Applies a mutation over a tree according to a scheme
 - MutationVisitor
     - Specific Change, e.g. adjust a number up or down by 1
 - Scheme
     - Probability and other constraints which limit the application of a mutation

 ### Language/

Compiler which outputs bytecode and various intermediary files for a simple virtual machine.
 - c-like language
 - separate and clear lexer, parser, ast and assembler
 - simple enough to understand and easy to extend for GP as needed
 - no real types
 - outputs graphviz diagrams of AST during compilation
 - TODO : save ast and reload it for use with tree mutation code

 ### Media/
 - Screenshots for docs etc

 ### Notes/

 - notes on system architecture
 - discussion of criteria for GP and gameplay
 - notes on language design to facilitate GP

 ### Simulation/

 - Interface between Language and Engine
 - e.g Agent is an Entity (from Engine) whcih also has a VM (from Language) with some alienvars as an interface between
    - Inc Global/Alien Variables
    - Factory Classes
 - Specific Game Entities necessary for GP for games


## Language Style

You can program it in a familiar way because its a c-like language with if, while { } ; etc Limited though, many special system unix features are gone, naturally we are integrated.

To achieve compatability of resulting programs after mutation and or crossovers the lanuage needed to be constrained in several special ways.

### Minimise Types
 - Reduce Cartesian Vectors to Cardinal Directions
 - All Parameters are Range Safe and Wrap by means of Modulus

### Variable Syntax Issues 
 - Remove Variable Declarations
 - Within an Entity All Local Variables are Always In Scope

### Functions and Parameters
 - All Functions take 1 param and return 1 param.
 - This way they become interchangable in the AST during crossover.

The code looks like this:
![](./Media/Screenshot_3.png)

Which compiles into the following AST and eventual byte code. 
![](./Media/Screenshot_4.png)

## Agents Interrogate the World
We are not exposing the internals of other agents to each other. So some builtin functions exist to provide minimal and robust flexible way to find out about other agents.
