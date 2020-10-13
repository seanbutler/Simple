//
// Created by sean on 23/07/2020.
//

#pragma once

#include <cmath>
#include "../Engine/Entity.h"

#include "../Language/Lexer.h"
#include "../Language/Parser.h"
#include "../Language/Assembler.h"
#include "../Language/VirtualMachine.h"
#include "../Utils/RandomNumberGenerator.h"

#include "Detection.h"
#include "AlienVars.h"

//
// a specific version of a game entity which has AI from an external file
//

// ----------------------------------------------------------------------

class Agent : public Engine::Entity {
public:
    Agent(sf::RectangleShape R)
            : Entity()
            , rectangle(R)
    {
//        std::cout << "Agent::Agent ()" << std::endl;
    }

    Agent(Agent &rhs)
        : srcFilename(rhs.srcFilename),
        rectangle(rhs.rectangle),
        lexer(rhs.lexer),
        parser(rhs.parser),
        assembler(rhs.assembler),
        virtualMachine(rhs.virtualMachine),
        alienVars(rhs.alienVars)
    {
        virtualMachine.SetAliensVarPtr(&alienVars);
    }

    virtual void Compile(std::string F) {
        srcFilename = F;
        std::ifstream sourceFile(srcFilename);
        std::string sourceString((std::istreambuf_iterator<char>(sourceFile)), std::istreambuf_iterator<char>());

        lexer.SetSourceString(sourceString, 0);
        lexer.Scan();
        lexer.OutputTokenList(srcFilename);
        lexer.OutputTokenListWithLines(srcFilename);
        //        parser.SetTokens(lexer.GetTokenList());
        parser.SetTokensWithLines(lexer.GetTokenListWithLines());
        parser.parse();

        parser.OutputTreeDiagram(srcFilename);
        parser.OutputAsm(srcFilename);
        parser.OutputASTJSON(srcFilename);
    }

    virtual void Assemble()
    {
        assembler.Scan(parser.GetAsm());
        assembler.OutputInstructionList(srcFilename);

//        assembler.GenerateTestBinaryInstructions();
        virtualMachine.Initialise(assembler.GetInstructions(), &alienVars);
    }

    virtual void Render(sf::RenderWindow *W){
        if (!virtualMachine.done){
            rectangle.setPosition(alienVars.get(0), alienVars.get(1));
            W->draw(rectangle);
        }
    }

    void SetAlienVar(int id, int value) {
        alienVars.set(id, value);
    }

    int GetAlienVar(int id) {
        return alienVars.get(id);
    }

    int CallAlienFunction(int id, int parameter){
        return alienVars.call(id, parameter);
    }

    virtual void Update(float deltaTime) {
        if (!virtualMachine.done) {
            virtualMachine.Execute(8);
        }
    }

    Agent* Cross(Agent* agent);

    std::string srcFilename;
    sf::RectangleShape rectangle;

    Lexer lexer;
    Parser parser;
    Assembler assembler;
    VM virtualMachine;
    AlienVars alienVars;

};

// ----------------------------------------------------------------------

class WallAgent : public Agent {

public:
    WallAgent()
            : Agent(sf::RectangleShape(sf::Vector2f(1, 1)))
            ,collidable(this)
    {
        rectangle.setFillColor(sf::Color(0, 0, 0));
    }

    CollisionComponent collidable;
};

// ----------------------------------------------------------------------

class HurtfulAgent : public Agent {

public:
    HurtfulAgent()
            : Agent(sf::RectangleShape(sf::Vector2f(1, 1)))
            , detectable(this)
            , collidable(this)

    {
        rectangle.setFillColor(sf::Color(255, 32, 32));
    }

    DetectableComponent_Hurtful detectable;
    CollisionComponent collidable;


};

// ----------------------------------------------------------------------

class HealingAgent : public Agent {

public:
    HealingAgent()
            : Agent(sf::RectangleShape(sf::Vector2f(1, 1)))
            , detectable(this)

    {
        rectangle.setFillColor(sf::Color(32, 255, 32));
    }

    DetectableComponent_Healing detectable;

};

// ----------------------------------------------------------------------

class GoalAgent : public Agent {

public:
    GoalAgent()
            : Agent(sf::RectangleShape(sf::Vector2f(1, 1)))
            , detectable(this)

    {
        rectangle.setFillColor(sf::Color(128, 128, 255));
    }

    DetectableComponent_Goal detectable;

};

// ----------------------------------------------------------------------

class PlayerAgent : public Agent {

public:
    PlayerAgent()
            : Agent(sf::RectangleShape(sf::Vector2f(1, 1)))
            , detectable(this)
    {
        rectangle.setFillColor(sf::Color(255, 0, 255));
    }

    DetectableComponent_Player detectable;
};

// ----------------------------------------------------------------------
