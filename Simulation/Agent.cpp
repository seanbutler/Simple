//
// Created by sean on 06/08/2020.
//

#include "./Agent.h"
#include "../Utils/RandomNumberGenerator.h"
#include <vector>

//static std::vector<std::pair<int, int>> DetectableAgentType::positionTable;

/// Returns a new agent that is a copy of this, but with a random location in its AST replaced with
/// another compatible random location in agent.
Agent * Agent::Cross(Agent *agent)
{
    // Copy this agent
    auto newAgent = new Agent(*this);

    // Find the two slice points
    ASTNode** a = newAgent->parser.GetRandomASTNode(CompatibilityType::all);
    ASTNode* aP = *a;
    ASTNode** b = agent->parser.GetRandomASTNode((*a)->GetCompType());

    // Replace the nodes after the first slice point with the branch of the second
    if(b != nullptr)
    {
        newAgent->parser.CopyNodeAndChildren(*b, *a);
        parser.DeleteNodeAndChildren(aP);
        delete aP;
    }

    newAgent->parser.ValidateAST();

    return newAgent;
}

