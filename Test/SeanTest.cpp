//
// Created by sean on 14/08/2020.
//

#include "SeanTest.h"

#include "../Simulation/Agent.h"
#include "../Engine/Engine.h"
#include "../Genetics/Evolution.h"

void SeanTest::Test1() {

//    srand(time(nullptr));

    Engine::Engine engine(32, 32);
    Genetics::Evolution evolution;

    srand(time(nullptr));

    Agent* tmpAgent1Ptr;
    for(int n=0;n<4;n++) {
        tmpAgent1Ptr = new HurtfulAgent();
        tmpAgent1Ptr->Compile("../Assets/enemy_homein.c");
        tmpAgent1Ptr->Assemble();
        tmpAgent1Ptr->SetAlienVar(0, 4 + rand()%24 );
        tmpAgent1Ptr->SetAlienVar(1, 4 + rand()%24 );
        engine.entityScheduler.entities.push_back((Engine::Entity*)tmpAgent1Ptr);
    }

//    for(int n=0;n<4;n++) {
//        tmpAgent1Ptr = new HurtfulAgent();
//        tmpAgent1Ptr->Compile("../Assets/enemy_pattern.c");
//        tmpAgent1Ptr->Assemble();
//        tmpAgent1Ptr->SetAlienVar(0, 8 + rand()%16 );
//        tmpAgent1Ptr->SetAlienVar(1, 8 + rand()%16 );
//        engine.entityScheduler.entities.push_back((Engine::Entity*)tmpAgent1Ptr);
//    }

    int off = rand()%24;
    for(int n=0;n<16;n++) {
        tmpAgent1Ptr = new WallAgent();
        tmpAgent1Ptr->Compile("../Assets/wall.c");
        tmpAgent1Ptr->Assemble();
        tmpAgent1Ptr->SetAlienVar(0, 8 + off );
        tmpAgent1Ptr->SetAlienVar(1, 8 + n );
        engine.entityScheduler.entities.push_back((Engine::Entity*)tmpAgent1Ptr);
    }

//    off = rand()%24;
//    for(int n=0;n<16;n++) {
//        tmpAgent1Ptr = new WallAgent();
//        tmpAgent1Ptr->Compile("../Assets/wall.c");
//        tmpAgent1Ptr->Assemble();
//        tmpAgent1Ptr->SetAlienVar(0, 8 + n );
//        tmpAgent1Ptr->SetAlienVar(1, 8 + off );
//        engine.entityScheduler.entities.push_back((Engine::Entity*)tmpAgent1Ptr);
//    }


    Agent* goalAgentPtr;
    goalAgentPtr = new GoalAgent();
    goalAgentPtr->Compile("../Assets/goal.c");
    goalAgentPtr->Assemble();
    goalAgentPtr->SetAlienVar(0, 8 + rand()%24 );
    goalAgentPtr->SetAlienVar(1, 8 + rand()%24 );
    engine.entityScheduler.entities.push_back((Engine::Entity*)goalAgentPtr);

    Agent* playerAgentPtr;
    playerAgentPtr = new PlayerAgent();
    playerAgentPtr->Compile("../Assets/player.c");
    playerAgentPtr->Assemble();
    playerAgentPtr->SetAlienVar(0, 8 + rand()%24 );
    playerAgentPtr->SetAlienVar(1, 8  + rand()%24 );
    engine.entityScheduler.entities.push_back((Engine::Entity*)playerAgentPtr);

    engine.MainLoop();
}