//
// Created by sean on 14/08/2020.
//

#include <stdio.h>
#include "TobyTest.h"
#include "../Engine/Engine.h"
#include "../Genetics/Evolution.h"

void TobyTest::TestPopulation()
{
    Engine::Engine engine(32, 32);
    Genetics::Evolution evolution;

    auto hurtfulRand = [](Agent* agent){
        agent->SetAlienVar(0, 8 + (int)rand()%16);
        agent->SetAlienVar(1, 8 + (int)rand()%16);
    };



    bool xAxis = true;
    auto axisFitness = [&xAxis](Agent* agent){

        auto copy = new Agent(*agent);
        copy->Assemble();

        std::vector<int> uniqueLocations;
        auto startPos = std::pair<int,int>(copy->alienVars.get(0), copy->alienVars.get(1));
        int lastPos = xAxis ? startPos.first : startPos.second;
        int furthestDist = 0;
        int moveCount = 0;

        for(int i = 0; i < 500; i++)
        {
            copy->Update(0);
            auto newPos = std::pair<int,int>(copy->alienVars.get(0), copy->alienVars.get(1));
            if((xAxis ? newPos.first : newPos.second) != lastPos)
                moveCount++;

            if((std::find(uniqueLocations.begin(), uniqueLocations.end(), xAxis ? newPos.first : newPos.second)) == uniqueLocations.end())
            {
                uniqueLocations.push_back(xAxis ? newPos.first : newPos.second);
            }

            float xDist = powf((float)fabs((double)startPos.first - (double)newPos.first),2.0f);
            float yDist = powf((float)fabs((double)startPos.second - (double)newPos.second),2.0f);
            int dist = (int)(xDist + (yDist));

            if(dist > furthestDist)
            {
                furthestDist = dist;
            }

            lastPos = xAxis ? newPos.first : newPos.second;
        }
        delete copy;
        return ((int)((uniqueLocations.size()-1)*5) +(moveCount*2) ) - furthestDist;
    };

    auto combinedFitness = [](Agent* agent){
        auto copy = new Agent(*agent);
        copy->Assemble();

        std::vector<std::pair<int,int>> uniqueLocations;
        auto startPos = std::pair<int,int>(copy->alienVars.get(0), copy->alienVars.get(1));
        auto lastPos = startPos;
        int furthestDist = 0;
        int moveCount = 0;

        for(int i = 0; i < 1000; i++)
        {
            copy->Update(0);
            auto newPos = std::pair<int,int>(copy->alienVars.get(0), copy->alienVars.get(1));
            if(newPos != lastPos)
                moveCount++;
            if((std::find(uniqueLocations.begin(), uniqueLocations.end(), newPos)) == uniqueLocations.end())
            {
                uniqueLocations.push_back(newPos);
                float xDist = powf((float)fabs((double)startPos.first - (double)newPos.first),2.0f);
                float yDist = powf((float)fabs((double)startPos.second - (double)newPos.second),2.0f);
                int dist = (int)powf(sqrtf(xDist + yDist),2);
                if(dist > furthestDist)
                {
                    furthestDist = dist;
                }
            }
        }
        delete copy;
        return ((int)((uniqueLocations.size()-1)*15) + (moveCount)) - furthestDist;
    };

    evolution.SetRandomiseFunction(hurtfulRand);
    evolution.SetFitnessFunction(axisFitness);
    evolution.InitialisePopulation(500,"../Assets/enemy_pattern.c");
    //evolution.InitialiseRandomPopulation(500, 6, 4);
    evolution.RandomisePopulation();

    int maxGenerations = 30;
    for(int i = 0; i <maxGenerations; i++)
    {
        std::cout << "xAxis Pop: Creating generation " << i+1 << "/" << maxGenerations << "..." << std::endl;
        evolution.AssessFitness();
        evolution.GenerateNewPopulation(0.7f, 0.1f, 0.1f, 0.1f);
    }
    evolution.AssessFitness();

    auto xPop = evolution.GetTopPopulationAgents(0.1f, true);

    xAxis = false;
    evolution.InitialisePopulation(500, "../Assets/enemy_pattern.c");
    //evolution.InitialiseRandomPopulation(500, 6, 4);
    evolution.RandomisePopulation();

    for(int i = 0; i <maxGenerations; i++)
    {
        std::cout << "yAxis Pop: Creating generation " << i+1 << "/" << maxGenerations << "..." << std::endl;
        evolution.AssessFitness();
        evolution.GenerateNewPopulation();
    }
    evolution.AssessFitness();

    auto yPop = evolution.GetTopPopulationAgents(0.1f, true);

    evolution.AddIndividuals(xPop);
    evolution.AddIndividuals(yPop);
    evolution.SetFitnessFunction(combinedFitness);

    for(int i = 0; i <maxGenerations; i++)
    {
        std::cout << "Combined Pop: Creating generation " << i+1 << "/" << maxGenerations << "..." << std::endl;
        evolution.AssessFitness();
        evolution.GenerateNewPopulation();
    }
    evolution.AssessFitness();

    int i =0;
    for(auto agent : evolution.GetTopPopulationAgents(0.1f, true))
    {
        agent->Assemble();
        agent->parser.OutputTreeDiagram("../output/agent" + std::to_string(i));
        engine.entityScheduler.entities.push_back((Engine::Entity*)agent);
        i++;
    }

    engine.MainLoop();
}

void TobyTest::TestRandomAST()
{
    Engine::Engine engine(32, 32);
    Genetics::Evolution evolution;

    auto randFunc = [](Agent* agent){
        agent->SetAlienVar(0, 8 + (int)rand()%16);
        agent->SetAlienVar(1, 8 + (int)rand()%16);
    };

    auto fitness = [](Agent* agent){
        return 1;
    };

    evolution.SetRandomiseFunction(randFunc);
    evolution.SetFitnessFunction(fitness);

    //evolution.InitialiseRandomPopulation(1000, 10, 4);
    evolution.InitialisePopulation(10, "../Assets/enemy_pattern.c");

    int i =0;
    for(auto agent : evolution.GetTopPopulationAgents(1.0f, true))
    {
        agent->Assemble();
        agent->parser.OutputTreeDiagram("../output/agent" + std::to_string(i));
        engine.entityScheduler.entities.push_back((Engine::Entity*)agent);
        i++;
    }


    engine.MainLoop();
}