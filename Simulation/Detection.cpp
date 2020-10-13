//
// Created by sean on 08/08/2020.
//

#include "Agent.h"
#include "Detection.h"

// --------------------------------------------------------------------------------
std::vector<Agent*> CollisionComponent::agents = {};

unsigned int CollisionComponent::collisionTest(int x, int y) {

    for( int n = 0; n< CollisionComponent::agents.size(); n++) {

        int otherX = agents[n]->alienVars.get(0);
        int otherY = agents[n]->alienVars.get(1);

        if ( otherX == x ) {
            if (otherY == y ) {
                // there exists a collidable agent at these coords
                return true;
            }
        }
    }
    // there is no collidable agent at these coords
    return false;
}


// TODO lots of nasty code repetition here, do something about it

// --------------------------------------------------------------------------------

std::vector<HurtfulAgent*> DetectableComponent_Hurtful::detectableAgents = {};

unsigned int DetectableComponent_Hurtful::directionToNearest(int x, int y) {


    float shortestDistance = INT32_MAX;
    float distance;
    float finalX = 0;
    float finalY = 0;
    float deltaX = 0;
    float deltaY = 0;

    for( int n = 0; n< detectableAgents.size(); n++) {

        float otherX = detectableAgents[n]->alienVars.get(0);
        float otherY = detectableAgents[n]->alienVars.get(1);

        deltaX = otherX - x;
        deltaY = otherY - y;

        distance = abs(sqrt((deltaX * deltaX) + (deltaY * deltaY)));

        if (distance < shortestDistance) {
            shortestDistance = distance;
            finalX = deltaX;
            finalY = deltaY;
        }
    }

    if (abs(finalX) > abs(finalY)) {

        if (finalX > 0) {
            return 1;
        }
        if (finalX < 0) {
            return 3;
        }
    }
    else {

        if ( finalY > 0 ) {
            return 2;
        }
        if ( finalY < 0 ) {
            return 0;
        }
    }

    return -1;
}

// --------------------------------------------------------------------------------

std::vector<HealingAgent*> DetectableComponent_Healing::detectableAgents = {};

unsigned int DetectableComponent_Healing::directionToNearest(int x, int y) {


    float shortestDistance = INT32_MAX;
    float distance;
    float finalX = 0;
    float finalY = 0;
    float deltaX = 0;
    float deltaY = 0;

    for( int n = 0; n< detectableAgents.size(); n++) {

        float otherX = detectableAgents[n]->alienVars.get(0);
        float otherY = detectableAgents[n]->alienVars.get(1);

        deltaX = otherX - x;
        deltaY = otherY - y;

        distance = abs(sqrt((deltaX * deltaX) + (deltaY * deltaY)));

        if (distance < shortestDistance) {
            shortestDistance = distance;
            finalX = deltaX;
            finalY = deltaY;
        }
    }

    if (abs(finalX) > abs(finalY)) {

        if (finalX > 0) {
            return 1;
        }
        if (finalX < 0) {
            return 3;
        }
    }
    else {

        if ( finalY > 0 ) {
            return 2;
        }
        if ( finalY < 0 ) {
            return 0;
        }
    }

    return -1;
}

// --------------------------------------------------------------------------------

std::vector<PlayerAgent*> DetectableComponent_Player::detectableAgents = {};

unsigned int DetectableComponent_Player::directionToNearest(int x, int y) {


    float shortestDistance = INT32_MAX;
    float distance;
    float finalX = 0;
    float finalY = 0;
    float deltaX = 0;
    float deltaY = 0;

    for( int n = 0; n< detectableAgents.size(); n++) {

        float otherX = detectableAgents[n]->alienVars.get(0);
        float otherY = detectableAgents[n]->alienVars.get(1);

        deltaX = otherX - x;
        deltaY = otherY - y;

        distance = abs(sqrt((deltaX * deltaX) + (deltaY * deltaY)));

        if (distance < shortestDistance) {
            shortestDistance = distance;
            finalX = deltaX;
            finalY = deltaY;
        }
    }

    if (abs(finalX) > abs(finalY)) {

        if (finalX > 0) {
            return 1;
        }
        if (finalX < 0) {
            return 3;
        }
    }
    else {

        if ( finalY > 0 ) {
            return 2;
        }
        if ( finalY < 0 ) {
            return 0;
        }
    }

    return -1;
}

// --------------------------------------------------------------------------------


std::vector<GoalAgent*> DetectableComponent_Goal::detectableAgents = {};

unsigned int DetectableComponent_Goal::directionToNearest(int x, int y) {

    float shortestDistance = INT32_MAX;
    float distance;
    float finalX = 0;
    float finalY = 0;
    float deltaX = 0;
    float deltaY = 0;

    for( int n = 0; n< detectableAgents.size(); n++) {

        float otherX = detectableAgents[n]->alienVars.get(0);
        float otherY = detectableAgents[n]->alienVars.get(1);

        deltaX = otherX - x;
        deltaY = otherY - y;

        distance = abs(sqrt((deltaX * deltaX) + (deltaY * deltaY)));

        if (distance < shortestDistance) {
            shortestDistance = distance;
            finalX = deltaX;
            finalY = deltaY;
        }
    }

    if (abs(finalX) > abs(finalY)) {

        if (finalX > 0) {
            return 1;
        }
        if (finalX < 0) {
            return 3;
        }
    }
    else {

        if ( finalY > 0 ) {
            return 2;
        }
        if ( finalY < 0 ) {
            return 0;
        }
    }

    return -1;
}

// --------------------------------------------------------------------------------
