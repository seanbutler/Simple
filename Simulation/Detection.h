//
// Created by sean on 08/08/2020.
//

#pragma once

#include <cmath>

#include "../Engine/Entity.h"

#include "../Language/Lexer.h"
#include "../Language/Parser.h"
#include "../Language/Assembler.h"
#include "../Language/VirtualMachine.h"

#include "AlienVars.h"

// ----------------------------------------------------------------------

class Agent;
class HurtfulAgent;
class HealingAgent;
class GoalAgent;
class PlayerAgent;

// ----------------------------------------------------------------------

class CollisionComponent {
public:
    CollisionComponent(Agent* A) : agent(A) {
        CollisionComponent::agents.push_back(agent);
    }
    virtual ~CollisionComponent() {}
    static unsigned int collisionTest(int x, int y);

private:
    Agent* agent;
    static std::vector<Agent*> agents;
};


// ----------------------------------------------------------------------

// TODO should template these classes

class DetectableComponent_Hurtful {
public:
    DetectableComponent_Hurtful(HurtfulAgent* A) : agent(A) {DetectableComponent_Hurtful::detectableAgents.push_back(agent);}
    virtual ~DetectableComponent_Hurtful() {}
    static unsigned int directionToNearest(int x, int y);

private:
    HurtfulAgent* agent;
    static std::vector<HurtfulAgent*> detectableAgents;
};

// ----------------------------------------------------------------------

class DetectableComponent_Healing {
public:
    DetectableComponent_Healing(HealingAgent* A) : agent(A) {DetectableComponent_Healing::detectableAgents.push_back(agent);}
    virtual ~DetectableComponent_Healing() {}
    static unsigned int directionToNearest(int x, int y);

private:
    HealingAgent* agent;
    static std::vector<HealingAgent*> detectableAgents;
};

// ----------------------------------------------------------------------

class DetectableComponent_Goal {
public:
    DetectableComponent_Goal(GoalAgent* A) : agent(A) {DetectableComponent_Goal::detectableAgents.push_back(agent);}
    virtual ~DetectableComponent_Goal() {}
    static unsigned int directionToNearest(int x, int y);

private:
    GoalAgent* agent;
    static std::vector<GoalAgent*> detectableAgents;
};

// ----------------------------------------------------------------------

class DetectableComponent_Player {
public:
    DetectableComponent_Player(PlayerAgent* A) : agent(A) {DetectableComponent_Player::detectableAgents.push_back(agent);}
    virtual ~DetectableComponent_Player() {}
    static unsigned int directionToNearest(int x, int y);

private:
    PlayerAgent* agent;
    static std::vector<PlayerAgent*> detectableAgents;
};

// ----------------------------------------------------------------------

