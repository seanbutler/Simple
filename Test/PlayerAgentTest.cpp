//
// Created by sean on 14/08/2020.
//

#include <stdio.h>
#include "PlayerAgentTest.h"
#include "../Engine/Engine.h"
#include "../Genetics/Evolution.h"

void PlayerAgentTest::TestPopulation()
{
    Engine::Engine engine(32, 32);
    Genetics::Evolution evolution;

    auto initRandomAVars = [](Agent* agent){
        agent->SetAlienVar(0, 8 + (int)rand()%16);
        agent->SetAlienVar(1, 8 + (int)rand()%16);

//        agent->SetAlienVar(0, 31);
//        agent->SetAlienVar(1, 31);
    };

    auto fitnessFunc = [](Agent* agent){
        auto copy = new Agent(*agent);
        copy->Assemble();

        std::pair<int, int> goalPos(2, 2);
        float dist;
        for(int i = 0; i < 128; i++)
        {
            copy->Update(0);

            auto currentPos = std::pair<int,int>(copy->alienVars.get(0), copy->alienVars.get(1));
            dist = sqrt ( abs( (currentPos.first - goalPos.first) * (currentPos.first - goalPos.first) ) + abs( (currentPos.second - goalPos.second) * (currentPos.second - goalPos.second) ) );
        }
        delete copy;
        return 1.0f - ( dist / 32.0f );
};

    evolution.SetRandomiseFunction(initRandomAVars);
    evolution.SetFitnessFunction(fitnessFunc);
//    evolution.InitialisePopulation(64,"../Assets/enemy_pattern.c");
    evolution.InitialiseRandomPopulation(128, 6, 4);
    evolution.RandomisePopulation();

    int maxGenerations = 512;
    for(int i = 0; i <maxGenerations; i++)
    {
        std::cout << "Creating generation " << i+1 << "/" << maxGenerations << "..." << std::endl;
        evolution.AssessFitness();
        evolution.GenerateNewPopulation();
    }
    evolution.AssessFitness();

    auto fittestTopPop = evolution.GetTopPopulationAgents(0.1f, true);

    int i =0;
    for(auto agent : fittestTopPop)
    {
        agent->Assemble();
        agent->parser.OutputTreeDiagram("../output/agent" + std::to_string(i));
        engine.entityScheduler.entities.push_back((Engine::Entity*)agent);
        i++;
    }

    engine.MainLoop();
}
