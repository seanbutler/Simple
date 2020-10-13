//
// Created by sean on 17/06/2020.
//

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "Tokens.h"

static std::string getUniqueIdentifier();

enum class CompatibilityType
{
    none,
    all,
    blockStart,
    expression,
    block,
    identifier,
    blockChild,

};

// ------------------------------------------------------------------------

static unsigned int uid = 1;

class ASTNode {
public:
    ASTNode(const std::string & N="") : id(uid++), name(N), number(0){

    }

    virtual ~ASTNode() = default;

    unsigned int getID()            { return id; }
    void GenerateNewID()            {id = uid++;}
    std::string &getName()          { return name; }

    int getNumber() const           { return number; }
    void setNumber(int N)           { number = N; }
    CompatibilityType GetCompType() {return compatibility; };
    void SetCompType(CompatibilityType type) {compatibility = type;};

    virtual std::string print();                                // USED FOR SIMPLE SERIALIZATION
    virtual std::string diag(unsigned int parentID) {};        // USED FOR GENERATING THE GV FILE...
    virtual std::string eval() {return "";};                             // USED FOR EMITTING THE ACTUAL INSTRUCTIONS...

    std::vector<ASTNode*>children;

protected:
    CompatibilityType compatibility = CompatibilityType::none;

private:
    unsigned int id;
    std::string name;
    int number;

};

// ------------------------------------------------------------------------

class IdentifierAST : public ASTNode {

public:
    IdentifierAST() {};

    explicit IdentifierAST(const std::string &N) : ASTNode(N)
    {
        compatibility = CompatibilityType::identifier;
    }

    virtual std::string print();
    virtual std::string diag(unsigned int parentID);
    virtual std::string eval();
};

// ------------------------------------------------------------------------

class AlienVarAST : public ASTNode {

public:
    AlienVarAST()
    {
        compatibility = CompatibilityType::expression;
    }

    virtual std::string print();
    virtual std::string diag(unsigned int parentID);
    virtual std::string eval();
};

// ------------------------------------------------------------------------

class VariableDeclarationAST : public ASTNode {

public:
    explicit VariableDeclarationAST(ASTNode *I)
        : ASTNode()
        , identifier(I)
    {
        compatibility = CompatibilityType::blockChild;
        ASTNode::children.push_back(I);
    }


    virtual std::string print();
    virtual std::string diag(unsigned int parentID);
    virtual std::string eval();

private:
    ASTNode *identifier;
};


// ------------------------------------------------------------------------

class BlockAST : public ASTNode {

public:
    BlockAST()
        : ASTNode()
    {
        compatibility = CompatibilityType::block;
    }

    virtual std::string print() override;
    virtual std::string diag(unsigned int parentID) override;
    virtual std::string eval() override;

//    std::vector<ASTNode *> statements;
};

// ------------------------------------------------------------------------

class OutputAST : public ASTNode {

public:
    OutputAST(ASTNode *E)
        : ASTNode()
    {
        compatibility = CompatibilityType::blockChild;
        ASTNode::children.push_back(E);
    }

    virtual std::string print();
    virtual std::string diag(unsigned int parentID);
    virtual std::string eval();

    ASTNode * getExpression()   { return children[0];}

};

// ------------------------------------------------------------------------

class MoveAST : public ASTNode {

public:
    MoveAST(ASTNode *E)
        : ASTNode()
    {
        compatibility = CompatibilityType::blockChild;
        ASTNode::children.push_back(E);
    }

    virtual std::string print();
    virtual std::string diag(unsigned int parentID);
    virtual std::string eval();

    ASTNode * getExpression()   { return children[0];}
};

// ------------------------------------------------------------------------

class RandFuncAST : public ASTNode {

public:
    RandFuncAST(ASTNode *E)
        : ASTNode()
    {
        compatibility = CompatibilityType::expression;
        ASTNode::children.push_back(E);
    }

    virtual std::string print();
    virtual std::string diag(unsigned int parentID);
    virtual std::string eval();

    ASTNode * getExpression()   { return children[0];}
};

// ------------------------------------------------------------------------

class SenseFuncAST : public ASTNode {

public:
    SenseFuncAST(ASTNode *E)
            : ASTNode()
    {
        compatibility = CompatibilityType::expression;
        ASTNode::children.push_back(E);
    }

    virtual std::string print();
    virtual std::string diag(unsigned int parentID);
    virtual std::string eval();

    ASTNode * getExpression()   { return children[0];}
};


// ------------------------------------------------------------------------

class IfStatementAST : public ASTNode {

public:
    IfStatementAST(ASTNode *E, ASTNode *B)
        : ASTNode()
    {
        compatibility = CompatibilityType::blockStart;
        children.push_back(E);
        children.push_back(B);
    }

    virtual std::string print();
    virtual std::string diag(unsigned int parentID);
    virtual std::string eval();

    ASTNode * getExpression()   { return children[0];}
    ASTNode * getBlock()        { return children[1];}

private:


};

// ------------------------------------------------------------------------

class WhileStatementAST : public ASTNode {

public:
    WhileStatementAST(ASTNode *E, ASTNode *B)
    : ASTNode()
    {
        compatibility = CompatibilityType::blockStart;
        children.push_back(E);
        children.push_back(B);
    }

    virtual std::string print();
    virtual std::string diag(unsigned int parentID);
    virtual std::string eval();

    ASTNode * getExpression()   { return children[0];}
    ASTNode * getBlock()        { return children[1];}
};


// ------------------------------------------------------------------------

class AssignmentStatementAST : public ASTNode {

public:
    AssignmentStatementAST(ASTNode *I, ASTNode *E)
        : ASTNode()
    {
        compatibility = CompatibilityType::blockChild;
        children.push_back(I);
        children.push_back(E);
    }

    virtual std::string print();
    virtual std::string diag(unsigned int parentID);
    virtual std::string eval();

    ASTNode * getIdentifier(){return children[0];}
    ASTNode * getExpression(){return children[1];}

};

// ------------------------------------------------------------------------

// NumberAST -

class NumberAST : public ASTNode {

public:
    NumberAST(int V) : ASTNode() {
        compatibility = CompatibilityType::expression;
        setNumber(V);
    }

    virtual std::string print();
    virtual std::string diag(unsigned int parentID);
    virtual std::string eval();

};

// ------------------------------------------------------------------------

// OperandAST - operators involved in expressions like "A * B or A + C etc"

class BinOperandAST : public ASTNode {

public:
    BinOperandAST(Token O, ASTNode *L, ASTNode *R);

    virtual std::string print();
    virtual std::string diag(unsigned int parentID);
    virtual std::string eval();

    Token op;
private:
};

// ------------------------------------------------------------------------

// YieldAST -

class YieldAST : public ASTNode {

public:
    YieldAST() : ASTNode()
    {
        compatibility = CompatibilityType::blockChild;
    }

    virtual std::string print();
    virtual std::string diag(unsigned int parentID);
    virtual std::string eval();
};

// ------------------------------------------------------------------------

