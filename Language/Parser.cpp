//
// Created by sean on 17/06/2020.
//

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stack>
#include <algorithm>

#include "Parser.h"
#include "AST.h"
#include "../Utils/RandomNumberGenerator.h"

// ----------------------------------------------------------------------

Parser::Parser(Parser &rhs)
{
    abstractSyntaxTree = rhs.abstractSyntaxTree;
    for(int i = 0; i < rhs.abstractSyntaxTree.size(); i++)
    {
        CopyNodeAndChildren(rhs.abstractSyntaxTree[i], abstractSyntaxTree[i]);
    }
}

Parser::~Parser()
{
    for(auto child : abstractSyntaxTree)
    {
        DeleteNodeAndChildren(child);
        delete child;
    }
}

void Parser::DeleteNodeAndChildren(ASTNode* node)
{
    for(auto child : node->children)
    {
        DeleteNodeAndChildren(child);
        delete child;
    }
}

void Parser::ReportIssue(const std::string & typeStr,
                         const std::string & problemStr,
                         const std::string & contextStr,
                         const std::string & expectedStr) {

    std::cerr << "PARSER " << typeStr
              << " Line (" << std::get<2>(currentTokenWithLine) << ") - "
              << problemStr;

    if ( contextStr != "" ) {
        std::cerr << " in " << contextStr;
    }

    if ( std::get<1>(currentTokenWithLine) != "") {
        std::cerr << ". Near \"" << std::get<1>(currentTokenWithLine) << "\"";
    }

    if ( expectedStr != "" ) {
        std::cerr << ", expected " << expectedStr;
    }

    std::cerr << "." << std::endl;
}

void Parser::ReportError(const std::string & problemStr, const std::string & contextStr, const std::string & expectedStr ) {
    ReportIssue("ERROR", problemStr, contextStr, expectedStr);
    exit(0);
}

void Parser::ReportWarning(const std::string & problemStr, const std::string & contextStr, const std::string & expectedStr ) {
    ReportIssue("WARNING", problemStr, contextStr, expectedStr);
}


bool Parser::parse() {

    getNextToken();
    ASTNode *newNode = nullptr;

    do {
        newNode = this->ParseStatement();

        if (newNode != nullptr) {
            abstractSyntaxTree.push_back(newNode);
            getNextToken();
        }

    } while (newNode != nullptr);

    return true;
}

// ----------------------------------------------------------------------

ASTNode *Parser::ParseStatement() {

//    std::cout << "\nParser::ParseStatement "
//                << (int) std::get<0>(currentTokenWithLine) << " "
//                << std::get<1>(currentTokenWithLine) << " "
//                << (int) std::get<2>(currentTokenWithLine) << " "
//                << std::endl;

    switch (std::get<0>(currentTokenWithLine)) {

        case Token::KEYWORD_YIELD: {
            ASTNode* node = new YieldAST();
            getNextToken();
            return node;
        }

        case Token::KEYWORD_EXIT: {
//            return ExitAST();
        }

        case Token::KEYWORD_OUTPUT: {
            return ParseOutput();
        }

        case Token::KEYWORD_MOVE: {
            // I AM A PROCEDURE I RETURN NOTHING CANNOT BE IN AN EXPRESSION ETC
            return ParseMoveProc();
        }

        case Token::KEYWORD_VAR: {
            return ParseVariableDeclaration();
        }

        case Token::KEYWORD_WHILE: {
            return ParseWhileStatement();
        }

        case Token::KEYWORD_IF: {
            return ParseIfStatement();
        }

        case Token::IDENTIFIER: {
            Token aheadTokenType = std::get<0>(lookAhead(0));
            if (aheadTokenType == Token::SYM_ASSIGN) {
                return ParseAssignmentStatement();
            } else {
                ReportError("Identifier Outside Expression or Assignment" );
                return nullptr;
            }
        }

        case Token::SYM_RBRACES: {
            ReportWarning("Skipping Extra Braces" );
            return nullptr;
        }

//        case Token::SYM_SEMICOLON: {
//            std::cerr << "WARNING : Parser::ParseStatement() - Skipping Extra Semicolon "
//                      << (int) currentToken.first << " "
//                      << currentToken.second
//                      << std::endl;
//            return nullptr;
//        }

        case Token::END_OF_FILE: {
            ReportWarning("End Of File" );
            return nullptr;
        }

        default: {
            ReportError("Unrecognised Token" );
            return nullptr;
        }
    }
}

// ----------------------------------------------------------------------

ASTNode *Parser::ParseVariableDeclaration() {

    getNextToken();

//    Token tokenType = currentToken.first;
//    std::string tokenString = currentToken.second;

    Token tokenType = std::get<0>(currentTokenWithLine);
    std::string tokenString = std::get<1>(currentTokenWithLine);

//    std::cout << "Parser::ParseVariableDeclaration "
//              << (int) tokenType << " "
//              << tokenString << std::endl;

//    IdentifierAST *ident = nullptr;
    ASTNode *ident = nullptr;
    ASTNode *var = nullptr;

    if (tokenType == Token::IDENTIFIER) {
        ident = ParseIdentifier();
        var = new VariableDeclarationAST(ident);
    } else {
        ReportError("Unrecognised Token",  "Variable Declaration", "Only Alpha then Alpha Numeric allowed");

//        std::cerr << "ERROR : Parser::ParseVariableDeclaration() - Unrecognised Token in Variable Declaration Found "
//                  << std::get<1>(currentTokenWithLine) << " Expected Token::IDENTIFIER" << std::endl;
    }

    getNextToken();

    if (std::get<0>(currentTokenWithLine) != Token::SYM_SEMICOLON) {

        ReportError("Unrecognised Token",  "Variable Declaration", ";");

//        std::cerr << "ERROR Parser::ParseVariableDeclaration() - Unrecognised Token in Variable Declaration Found "
//                  << std::get<1>(currentTokenWithLine) << " Expected ;" << std::endl;
    }

    return var;
}

// ----------------------------------------------------------------------

ASTNode *Parser::ParseBlock() {

    getNextToken();

    ASTNode *newNode = nullptr;
    std::vector<ASTNode *> nodes;
    BlockAST *block = new BlockAST();

    do {
        newNode = this->ParseStatement();
        if (newNode != nullptr) {
            block->children.push_back(newNode);
            getNextToken();
        }
    } while (newNode != nullptr);
    return block;
}

// ----------------------------------------------------------------------

ASTNode *Parser::ParseOutput() {

    Token tokenType = std::get<0>(currentTokenWithLine);
    std::string tokenString = std::get<1>(currentTokenWithLine);

    getNextToken();

    if (std::get<0>(currentTokenWithLine) != Token::SYM_ASSIGN) {
        ReportError("Unrecognised Token",  "Before Statement Expression", "Expected Assign");
        return nullptr;
    }

    getNextToken();

    ASTNode *expr = this->ParseExpression();

//    getNextToken();

    OutputAST *outputStatement = new OutputAST(expr);

    return outputStatement;
}

// ----------------------------------------------------------------------

ASTNode *Parser::ParseMoveProc() {

    Token tokenType = std::get<0>(currentTokenWithLine);
    std::string tokenString = std::get<1>(currentTokenWithLine);

//    std::cout << "Parser::ParseMoveProc tokenType = " << (int) tokenType
//              << " " << tokenString << std::endl;

    getNextToken();

    if (std::get<0>(currentTokenWithLine) != Token::SYM_LPAREN) {
        ReportError("Unrecognised Token",  "Before Statement Expression, Left Parenthesis");
        return nullptr;
    }

    getNextToken();
    ASTNode *expr = this->ParseExpression();
    getNextToken();
    MoveAST *statement = new MoveAST(expr);
    return statement;
}

// ----------------------------------------------------------------------

ASTNode *Parser::ParseSenseFunc() {

    Token tokenType = std::get<0>(currentTokenWithLine);
    std::string tokenString = std::get<1>(currentTokenWithLine);

    getNextToken();

    if (std::get<0>(currentTokenWithLine) != Token::SYM_LPAREN) {
        ReportError("Unrecognised Token",  "Before Statement Expression, Left Parenthesis");
        return nullptr;
    }

    getNextToken();
    ASTNode *expr = this->ParseExpression();
    getNextToken();
    SenseFuncAST *statement = new SenseFuncAST(expr);
    return statement;
}

// ----------------------------------------------------------------------

ASTNode *Parser::ParseRandFunc() {

    Token tokenType = std::get<0>(currentTokenWithLine);
    std::string tokenString = std::get<1>(currentTokenWithLine);

//    std::cout << "Parser::ParseRandFunc tokenType = " << (int) tokenType
//              << " " << tokenString << std::endl;

    getNextToken();

    if (std::get<0>(currentTokenWithLine) != Token::SYM_LPAREN) {
        ReportError("Unrecognised Token",  "Function Call", "Expected \'(\'");
        return nullptr;
    }

    ASTNode *expr = ParseExpression();
    RandFuncAST *statement = new RandFuncAST(expr);
    return statement;
}


// ----------------------------------------------------------------------

ASTNode *Parser::ParseIfStatement() {

    Token tokenType = std::get<0>(currentTokenWithLine);
    std::string tokenString = std::get<1>(currentTokenWithLine);

//    std::cout << "Parser::ParseIfStatement tokenType = " << (int) tokenType
//              << " " << tokenString << std::endl;

    getNextToken();

    if (std::get<0>(currentTokenWithLine) != Token::SYM_LPAREN) {
        ReportError("Unrecognised Token",  "Before If Statement Expression", "Left Parenthesis");

        return nullptr;
    }

    getNextToken();

    ASTNode *cond = this->ParseExpression();

//    getNextToken();
//
//    if ( currentToken.first != Token::SYM_RPAREN) {
//        std::cerr << "ERROR Parser::ParseIfStatement() - Unrecognised Token After If Statement Expression " << (int)currentToken.first << currentToken.second << " Expected Right Parenthesis." << std::endl;
//        return nullptr;
//    }

    getNextToken();

    if (std::get<0>(currentTokenWithLine) != Token::SYM_LBRACES) {
        ReportError("Unrecognised Token",  "Before If Statement Block", "Left Braces");
        return nullptr;
    }

    ASTNode *block = this->ParseBlock();

    IfStatementAST *ifStatement = new IfStatementAST(cond, block);

    if (std::get<0>(currentTokenWithLine) != Token::SYM_RBRACES) {
        ReportError("Unrecognised Token",  "After If Statement Block", "Left Braces");
        return nullptr;
    }

    return ifStatement;
}

// ----------------------------------------------------------------------

ASTNode *Parser::ParseWhileStatement() {

    Token tokenType = std::get<0>(currentTokenWithLine);
    std::string tokenString = std::get<1>(currentTokenWithLine);

//    std::cout << "Parser::ParseWhileStatement tokenType = " << (int) tokenType
//              << " " << tokenString << std::endl;

    getNextToken();

    if (std::get<0>(currentTokenWithLine) != Token::SYM_LPAREN) {
        ReportError("Unrecognised Token",  "Before While Statement Expression", "Left Parenthesis");
        return nullptr;
    }

    getNextToken();

    ASTNode *cond = this->ParseExpression();

//    getNextToken();
//
//    if ( currentToken.first != Token::SYM_RPAREN) {
//        std::cerr << "ERROR Parser::ParseWhileStatement() - Unrecognised Token After While Statement Expression, Expected Right Parenthesis." << std::endl;
//        return nullptr;
//    }

    getNextToken();

    if (std::get<0>(currentTokenWithLine) != Token::SYM_LBRACES) {
        ReportError("Unrecognised Token",  "Before While Statement Block", "Left Braces '{'");
        return nullptr;
    }

    ASTNode *block = this->ParseBlock();

    WhileStatementAST *whileStatement = new WhileStatementAST(cond, block);

    if (std::get<0>(currentTokenWithLine) != Token::SYM_RBRACES) {
        ReportError("Unrecognised Token",  "After While Statement Block", "Right Braces '}'");
        return nullptr;
    }

    return whileStatement;
}

// ----------------------------------------------------------------------

ASTNode *Parser::ParseExpression() {

    std::cout << "Parser::ParseExpression() currentToken = "
            << (int) std::get<0>(currentTokenWithLine) << " "
            <<  std::get<1>(currentTokenWithLine) << " "
            << (int) std::get<2>(currentTokenWithLine)
              << std::endl;

    std::stack<ASTNode *> valueStack;
    std::stack<Token> opStack;

    ASTNode *expr = nullptr;

    bool done = false;
    while (!done) {

        switch (std::get<0>(currentTokenWithLine)) {

            case Token::KEYWORD_RANDFUN: {
                valueStack.push(ParseRandFunc());
                break;
            }

            case Token::KEYWORD_SENSEFUN: {
                valueStack.push(ParseSenseFunc());
                break;
            }

            case Token::NUMBER: {
                valueStack.push(ParseNumber());
                getNextToken();
                break;
            }

            case Token::IDENTIFIER: {
                valueStack.push(ParseIdentifier());
                // When identifiers are used as part of an expression we need to change its compatibility type
                valueStack.top()->SetCompType(CompatibilityType::expression);
                getNextToken();
                break;
            }

            case Token::SYM_LPAREN: {
                valueStack.push(ParseParenExpression());
//                getNextToken();
                break;
            }

            case Token::OP_GT:
            case Token::OP_GTE:
            case Token::OP_LT:
            case Token::OP_LTE:
            case Token::OP_EQ:
            case Token::OP_NE:
            case Token::OP_ADD:
            case Token::OP_SUB:
            case Token::OP_MUL:
            case Token::OP_DIV:
            case Token::OP_AND:
            case Token::OP_OR:
                {
                if (!opStack.empty()) {
                    // get the last operator
                    Token op = opStack.top();
                    opStack.pop();

                    // get the last two values
                    ASTNode* right = valueStack.top();
                    valueStack.pop();

                    ASTNode *left = valueStack.top();
                    valueStack.pop();

                    // construct a binary op ast node
                    // TODO construct operator Node here...
                    ASTNode *binOpNode = new BinOperandAST(op, left, right);
                    valueStack.push(binOpNode);
                }

                opStack.push(std::get<0>(currentTokenWithLine));
                getNextToken();
                break;
            }

            case Token::SYM_RPAREN:
            case Token::SYM_SEMICOLON:
            {
                // TODO endof the whole expression so unwind the stack and return?

                while (!opStack.empty()) {
                    // get the last operator
                    Token op = opStack.top();
                    opStack.pop();

                    // get the last two values
                    ASTNode *right = valueStack.top();
                    valueStack.pop();

                    ASTNode *left = valueStack.top();
                    valueStack.pop();

                    // construct a binary op ast node
                    // TODO construct operator Node here...
                    ASTNode *binOpNode = new BinOperandAST(op, left, right);
                    valueStack.push(binOpNode);
                }

                if (!valueStack.empty()) {
                    return valueStack.top();
                } else {
                    ReportWarning("Empty Expression. Which is weird, though possibly intentional");
                    return nullptr;
                }
                break;
            }

            default: {
                // anything else means an error of some kind, perhaps?
                ReportError("Unrecognised Token", "In Expression");
                return nullptr;
            }
        }
    }

    return expr;
}

// ----------------------------------------------------------------------

ASTNode *Parser::ParseParenExpression() {

    getNextToken();  // eat '('

    ASTNode *expr = nullptr;
    expr = ParseExpression();

//    if (std::get<0>(currentTokenWithLine) != Token::SYM_RPAREN) {
//        ReportError("Unrecognised Token", "after Expression", "Expected Right Parenthesis");
//    }

    getNextToken();  // eat ')'
    return expr;
}

// ----------------------------------------------------------------------

ASTNode *Parser::ParseAssignmentStatement() {

//    std::cout << "Parser::ParseAssignmentStatement ID currentToken = "
//              << (int) currentToken.first << " "
//              << currentToken.second << std::endl;

    // parse identifier
    ASTNode *identifierNode = ParseIdentifier();

    getNextToken();

    // parse assignment operator
    if (std::get<0>(currentTokenWithLine) != Token::SYM_ASSIGN) {
        ReportError("Unrecognised Token", "in Assignment Statement", "Expected Assignment '='.");
        return nullptr;
    }

//    std::cout << "Parser::ParseAssignmentStatement currentToken = " << (int) currentToken.first << " \'"
//              << currentToken.second << "\'" << std::endl;

    getNextToken();

    // parse expression
    ASTNode *expressionNode = ParseExpression();

    AssignmentStatementAST *assignmentStatementAST;
    assignmentStatementAST = new AssignmentStatementAST(identifierNode, expressionNode);
    return assignmentStatementAST;
}


// ----------------------------------------------------------------------

ASTNode *Parser::ParseIdentifier() {
//    std::cout << "Parser::ParseIdentifier currentToken = " << (int) currentToken.first << " '" << currentToken.second
//              << "' " << std::endl;
    IdentifierAST *identifierNode = new IdentifierAST(std::get<1>(currentTokenWithLine));
    return identifierNode;
}

ASTNode *Parser::ParseNumber() {
//    std::cout << "Parser::ParseNumber currentToken = " << (int) currentToken.first << " '" << currentToken.second
//              << "' " << std::endl;
    ASTNode *numberNode = new NumberAST(atoi(std::get<1>(currentTokenWithLine).c_str()));
    return numberNode;
}


// ----------------------------------------------------------------------

bool Parser::OutputTreeDiagram(std::string filename) {

    std::ofstream diagramFile;
    std::string diagramFilename = std::filesystem::path(filename);
    diagramFilename += std::string(".gv");
    diagramFile.open(diagramFilename);

    diagramFile << "digraph G {" << std::endl;
    for (auto N : abstractSyntaxTree) {
        if (N) {
            diagramFile << N->diag(0);
        }
    }
    diagramFile << std::endl << "}" << std::endl;
    diagramFile.close();
    return true;
}

// ----------------------------------------------------------------------

bool Parser::OutputASTJSON(std::string filename) {

    std::ofstream outFile;
    std::string outFilename = std::filesystem::path(filename);
    outFilename += std::string(".json");
    outFile.open(outFilename);

    unsigned int indent = 0;

    outFile << "[\n" << std::endl;
    for (auto N : abstractSyntaxTree) {
        if (N) {
            outFile << "\n{\n" << std::endl;
            outFile << N->print();
            outFile << "\n}\n" << std::endl;
            if (N != abstractSyntaxTree.back())
                outFile << ",\n" << std::endl;
        }
    }
    outFile << std::endl << "]\n" << std::endl;
    outFile.close();
    return true;
}


// ----------------------------------------------------------------------

bool Parser::OutputAsm(std::string filename) {

    std::ofstream outFile;
    std::string outFilename = std::filesystem::path(filename);
    outFilename += std::string(".asm");
    outFile.open(outFilename);

    outFile << GetAsm();
    outFile.close();

    return true;
}


std::string Parser::GetAsm() {

    std::string assemblyCode;

    for (auto N : abstractSyntaxTree) {
        if (N) {
            assemblyCode += N->eval();
        }
    }

    return  assemblyCode;
}

/// Recursive function used to create deep copies of ASTNodes
void Parser::CopyNodeAndChildren(ASTNode *&original, ASTNode *&copy)
{
    //copy = new ASTNode(*original);
    //Temp fix to allow for deep copies
    if(dynamic_cast<RandFuncAST*>(original))
        copy = new RandFuncAST(*dynamic_cast<RandFuncAST*>(original));
    else if(dynamic_cast<BinOperandAST*>(original))
        copy = new BinOperandAST(*dynamic_cast<BinOperandAST*>(original));
    else if(dynamic_cast<WhileStatementAST*>(original))
        copy = new WhileStatementAST(*dynamic_cast<WhileStatementAST*>(original));
    else if(dynamic_cast<IfStatementAST*>(original))
        copy = new IfStatementAST(*dynamic_cast<IfStatementAST*>(original));
    else if(dynamic_cast<AssignmentStatementAST*>(original))
        copy = new AssignmentStatementAST(*dynamic_cast<AssignmentStatementAST*>(original));
    else if(dynamic_cast<AlienVarAST*>(original))
        copy = new AlienVarAST(*dynamic_cast<AlienVarAST*>(original));
    else if(dynamic_cast<BlockAST*>(original))
        copy = new BlockAST(*dynamic_cast<BlockAST*>(original));
    else if(dynamic_cast<IdentifierAST*>(original))
        copy = new IdentifierAST(*dynamic_cast<IdentifierAST*>(original));
    else if(dynamic_cast<MoveAST*>(original))
        copy = new MoveAST(*dynamic_cast<MoveAST*>(original));
    else if(dynamic_cast<NumberAST*>(original))
        copy = new NumberAST(*dynamic_cast<NumberAST*>(original));
    else if(dynamic_cast<OutputAST*>(original))
        copy = new OutputAST(*dynamic_cast<OutputAST*>(original));
    else if(dynamic_cast<VariableDeclarationAST*>(original))
        copy = new VariableDeclarationAST(*dynamic_cast<VariableDeclarationAST*>(original));
    else if(dynamic_cast<YieldAST*>(original))
        copy = new YieldAST(*dynamic_cast<YieldAST*>(original));
    else if(dynamic_cast<SenseFuncAST*>(original))
        copy = new SenseFuncAST(*dynamic_cast<SenseFuncAST*>(original));
    //Temp fix to allow for deep copies

    copy->GenerateNewID();
    if(!original->children.empty())
    {
        for(int i = 0; i < original->children.size(); i++)
        {
            CopyNodeAndChildren(original->children[i], copy->children[i]);
        }
    }
}

/// Returns a random node within the AST
ASTNode ** Parser::GetRandomASTNode(CompatibilityType typeFilter)
{
    return GetASTNode(util::RandomNumberGenerator::RandNum(1, CompatibleASTCount(typeFilter)), typeFilter);
}

/// Returns the nth (index) node in the AST of the given typeFilter
ASTNode ** Parser::GetASTNode(int index, CompatibilityType typeFilter)
{
    int i = 0;
    for(auto& node : abstractSyntaxTree)
    {
        ASTNode** result = FindNodeInTree(i, node, index, typeFilter);
        if(result != nullptr)
            return result;
    }
    return nullptr;
}

/// Searches through all of a nodes children to find a given index
ASTNode ** Parser::FindNodeInTree(int &currentIndex, ASTNode*& node, int targetIndex, CompatibilityType typeFilter)
{
    if(node->GetCompType() == typeFilter ||
       (typeFilter == CompatibilityType::all && node->GetCompType() != CompatibilityType::none))
        currentIndex++;

    if(targetIndex == currentIndex)
        return &node;
    for(auto &n : node->children)
    {
        ASTNode** result = FindNodeInTree(currentIndex, n, targetIndex, typeFilter);
        if(result != nullptr)
            return result;
    }
    return nullptr;
}

/// Returns the number of nodes of a given typeFilter in the current AST
int Parser::CompatibleASTCount(CompatibilityType typeFilter)
{
    int total = 0;
    for(auto node : abstractSyntaxTree)
    {
        CountNodes(total, node, typeFilter);
    }
    return total;
}

/// Searches through all of a nodes children and adds that count to nodeTotal
void Parser::CountNodes(int &nodeTotal, ASTNode* node, CompatibilityType typeFilter)
{
    if(node->GetCompType() == typeFilter ||
      (typeFilter == CompatibilityType::all && node->GetCompType() != CompatibilityType::none))
        nodeTotal++;

    for(auto n : node->children)
        CountNodes(nodeTotal, n, typeFilter);
}


// ----------------------------------------------------------------------

/// Checks to see if the AST is valid (trees may sometimes be invalid after crossover)
void Parser::ValidateAST()
{
    bool valid = true;
    TreeState state;
    
    for(auto& node : abstractSyntaxTree)
    {
        // Record any variables that are in scope
        if(dynamic_cast<VariableDeclarationAST*>(node) || dynamic_cast<AssignmentStatementAST*>(node))
        {
            state.scopedVars.push_back(node->children[0]->getName());
        }
        ValidateBranch(&node, state);
    }
}

void Parser::ValidateBranch(ASTNode** node, TreeState state)
{
    bool valid = true;
    if(dynamic_cast<IdentifierAST*>(*node))
    {
        // If this identifier is used out of scope, replace it
        if(std::find(state.scopedVars.begin(), state.scopedVars.end(), (*node)->getName()) == state.scopedVars.end())
        {
            delete *node;
            ASTNode* num = new NumberAST(util::RandomNumberGenerator::RandNum(0,10));
            *node = num;
        }
    }
    for(auto& child : (*node)->children)
    {
        // Record any variables that are in scope
        if(dynamic_cast<VariableDeclarationAST*>(child) || dynamic_cast<AssignmentStatementAST*>(child))
        {
            state.scopedVars.push_back(child->children[0]->getName());
        }
        ValidateBranch(&child, state);
    }
}


/// Recursively generates a random AST. The tree will be at most maxTreeDepth nodes in depth, with each block
/// generating between 1 and maxBranchWidth branches. If full is ture the full method will be used to populate,
/// otherwise grow will be used.
void Parser::GenerateRandomAST(int maxTreeDepth, int maxBranchWidth, bool full)
{
    TreeState state;
    state.maxDepth = maxTreeDepth;
    state.maxBranchWidth = maxBranchWidth;
    state.full = full;
    for(int i = 0; i < util::RandomNumberGenerator::RandNum(1, maxBranchWidth); i++)
    {
        abstractSyntaxTree.push_back(GenerateBlockChild(state));
    }
}

/// Randomly creates a new if or while node and a new block node
ASTNode* Parser::GenerateBlock(TreeState state)
{
    state.currentDepth += 2;
    ASTNode* node = nullptr;
    if(util::RandomNumberGenerator::RandNum(0,1) == 0)
    {
        node = new IfStatementAST(GenerateExpr(state), new BlockAST());
    }
    else
    {
        node = new WhileStatementAST(GenerateExpr(state), new BlockAST());
    }

    for(int i = 0; i < util::RandomNumberGenerator::RandNum(0, state.maxBranchWidth); i++)
    {
        node->children[1]->children.push_back(GenerateBlockChild(state));
    }
    return node;
}

/// Creates a random binary node
ASTNode * Parser::GenerateBin(TreeState state)
{
    state.currentDepth++;
    switch (util::RandomNumberGenerator::RandNum(0,11))
    {
        case 0:
            return new BinOperandAST(Token::OP_GT, GenerateExpr(state), GenerateExpr(state));
        case 1:
            return new BinOperandAST(Token::OP_GTE, GenerateExpr(state), GenerateExpr(state));
        case 2:
            return new BinOperandAST(Token::OP_LT, GenerateExpr(state), GenerateExpr(state));
        case 3:
            return new BinOperandAST(Token::OP_LTE, GenerateExpr(state), GenerateExpr(state));
        case 4:
            return new BinOperandAST(Token::OP_EQ, GenerateExpr(state), GenerateExpr(state));
        case 5:
            return new BinOperandAST(Token::OP_NE, GenerateExpr(state), GenerateExpr(state));
        case 6:
            return new BinOperandAST(Token::OP_MUL, GenerateExpr(state), GenerateExpr(state));
        case 7:
            return new BinOperandAST(Token::OP_DIV, GenerateExpr(state), GenerateExpr(state));
        case 8:
            return new BinOperandAST(Token::OP_ADD, GenerateExpr(state), GenerateExpr(state));
        case 9:
            return new BinOperandAST(Token::OP_SUB, GenerateExpr(state), GenerateExpr(state));
        case 10:
            return new BinOperandAST(Token::OP_AND, GenerateExpr(state), GenerateExpr(state));
        case 11:
            return new BinOperandAST(Token::OP_OR, GenerateExpr(state), GenerateExpr(state));
        default: // Should never hit default
            return nullptr;
    }
}

/// Creates a new variable node. Note state is passed by ref here since it needs to add the variable to scopedVars
ASTNode * Parser::GenerateVar(TreeState& state)
{
    state.currentDepth++;
    std::string name = "v" + std::to_string(varCount++);
    auto node = new AssignmentStatementAST(new IdentifierAST(name), GenerateExpr(state));
    state.scopedVars.push_back(name);
    return node;
}

/// Creates a new variable assignment node, assigning a random number as defined by GenerateNum() to a random variable
/// that is in scope for this part of the tree
ASTNode * Parser::GenerateAssignment(TreeState state)
{
    state.currentDepth++;
    int randInd = util::RandomNumberGenerator::RandNum(0, (int)state.scopedVars.size()-1);
    return new AssignmentStatementAST(new IdentifierAST(state.scopedVars[randInd]), GenerateExpr(state));
}


/// Creates a random numeric value, either a static number, a variable, or a result of binary operators on numbers and
/// variables. Note this is also recursive, as each number used in the binary operators calls this function again.
ASTNode * Parser::GenerateExpr(TreeState state)
{
    bool depthReached = state.currentDepth > state.maxDepth;
    int randInd = state.scopedVars.empty() ? -1 : util::RandomNumberGenerator::RandNum(0, (int)state.scopedVars.size()-1);
    switch (util::RandomNumberGenerator::RandNum(state.full && !depthReached ? 3 : 0, depthReached? 2 : 3)) {
        case 0:
            state.currentDepth++;
            return new NumberAST(util::RandomNumberGenerator::RandNum(0, 100));
        case 1:
            if(state.scopedVars.empty())
                return GenerateExpr(state);
            return new IdentifierAST(state.scopedVars[randInd]);
        case 2:
            return GenerateExpr(state);
            //return new AlienVarAST();
        case 3: // Case 3 is ignored when depth limit is reached
            return GenerateBin(state);
        default: // Should never hit default
            return nullptr;
    }
}


/// Creates a random AST Node that is legal as a child of a block
ASTNode* Parser::GenerateBlockChild(TreeState& state)
{
    bool depthReached = state.currentDepth > state.maxDepth;
    switch (util::RandomNumberGenerator::RandNum(state.full && !depthReached ? 1 : 0, depthReached ? 2 : 3)) {
        case 0: // Skipped when using full method AND max depth has not been reached
            state.currentDepth++;
            return new MoveAST(GenerateExpr(state));
        case 1:
            if((int)state.scopedVars.size() > 0)
                return GenerateAssignment(state);
            else
                return GenerateBlockChild(state); // If no variables are in scope, then just re-roll
        case 2:
            return GenerateVar(state);
        case 3: // Case 3 is ignored when depth limit is reached
            return GenerateBlock(state);
        default: // Should never hit default
            return nullptr;

    }
}