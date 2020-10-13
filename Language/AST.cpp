//
// Created by sean on 17/06/2020.
//

#include "Tokens.h"
#include "AST.h"
#include "metainstructions.h"

#include <iostream>
#include <iomanip>

// ---------------------------------------------------------------------------

unsigned long guid = 0;

static std::string getUniqueIdentifier(){

    std::stringstream res;
    res << "LBL_" << std::setfill ('0') << std::setw (4);
    res << guid++;
    return  res.str();
}


// ---------------------------------------------------------------------------

std::string ASTNode::print() {
    std::string str;
    return str;
}

// ---------------------------------------------------------------------------


std::string VariableDeclarationAST::print() {
    std::string str = "declaration: {";
    str += ASTNode::print();
    str += "\n\"type\" = \"defvar\"";
    str += this->identifier->print();
    return str;
}

std::string VariableDeclarationAST::diag(unsigned int parentID) {
    std::string str = "";
    str += "node" + std::to_string(ASTNode::getID()) + " [ label = \"defvar:\" ];\n";
    str += "node" + std::to_string(parentID) + " -> node" + std::to_string(ASTNode::getID()) + ";\n";
    str += this->identifier->diag(ASTNode::getID());
    return str;
}

std::string VariableDeclarationAST::eval() {
    std::string str;
    str += "\n";
    str += COMMENT + " DECLARE " + this->identifier->getName() + "\n";
    return str;
}

// ---------------------------------------------------------------------------


std::string IdentifierAST::print() {
    std::string str = "\"identifier\" : ";
    str += "\"" + getName() + "\"";
    return str;
}

std::string IdentifierAST::diag(unsigned int parentID) {
    std::string str = "";
    str += "node" + std::to_string(ASTNode::getID()) + " [ label = \"" + getName() + "\"];\n";
    str += "node" + std::to_string(parentID) + " -> node" + std::to_string(ASTNode::getID()) + ";\n";
    return str;
}

std::string IdentifierAST::eval() {
    std::string str = "";
    str += "\tLOAD " + VARIABLE + getName() + "\n";
    return str;
}

// ---------------------------------------------------------------------------

std::string AlienVarAST::print() {
    std::string str = "alienvar: {";
    str += "alienvar: \"" + getName() + "\"";
    str += "}\n";
    return str;
}

std::string AlienVarAST::diag(unsigned int parentID) {
    std::string str = "";
    str += "node" + std::to_string(ASTNode::getID()) + " [ label = \"" + getName() + "\"];\n";
    str += "node" + std::to_string(parentID) + " -> node" + std::to_string(ASTNode::getID()) + ";\n";
    return str;
}

std::string AlienVarAST::eval() {
    std::string str = "";
    str += "\tLOADALIEN " + VARIABLE + getName() + "\n";
    return str;
}

// ---------------------------------------------------------------------------

std::string BlockAST::print() {
    std::string str;

    for (auto S : children) {
        str += S->print();
        if ( S != children.back())
        {
            str += ",\n";
        }
    }
    return str;
}

std::string BlockAST::diag(unsigned int parentID) {
    std::string str = "";
    str += "node" + std::to_string(ASTNode::getID()) + " [ label = \"block:\" ];\n";
    str += "node" + std::to_string(parentID) + " -> node" + std::to_string(ASTNode::getID()) + ";\n";
    for (auto S : children) {
        str += S->diag(ASTNode::getID());
    }
    return str;
}

std::string BlockAST::eval() {
    std::string str = COMMENT + " BEGIN BLOCK\n";
    for (auto S  : children) {
        str += S->eval();
    }
    str += "\n" + COMMENT +  " END BLOCK\n";
    return str;
}

// ---------------------------------------------------------------------------


std::string OutputAST::print() {
    std::string str;
    str = ASTNode::print();
    str +=  "output: ";
    str += this->getExpression()->print();
    str += "}\n";
    return str;
}

std::string OutputAST::diag(unsigned int parentID) {
    std::string str;
    str += "node" + std::to_string(ASTNode::getID()) + " [ label = \"output:\"];\n";
    str += "node" + std::to_string(parentID) + " -> node" + std::to_string(ASTNode::getID()) + ";\n";
    str += this->getExpression()->diag(ASTNode::getID());
    return str;
}

std::string OutputAST::eval() {
    std::string str = "";
    str += "\n" + COMMENT + " OUTPUT EXPRESSION\n";
    str += this->getExpression()->eval();
    str += "\tOUTPUT \n";
    return str;
}

// ---------------------------------------------------------------------------


std::string MoveAST::print() {
    std::string str;
    str = ASTNode::print();
    str +=  "\"move\" : {\n";
    str += getExpression()->print();
    str += "}\n";
    return str;
}

std::string MoveAST::diag(unsigned int parentID) {
    std::string str;
    str += "node" + std::to_string(ASTNode::getID()) + " [ label = \"move:\"];\n";
    str += "node" + std::to_string(parentID) + " -> node" + std::to_string(ASTNode::getID()) + ";\n";
    str += getExpression()->diag(ASTNode::getID());
    return str;
}

std::string MoveAST::eval() {
    std::string str = "";
    str += "\n" + COMMENT + " MOVE FUNC (builtin)\n";
    str += getExpression()->eval();
    str += "\tMOVE \n";
    return str;
}


// ---------------------------------------------------------------------------

std::string RandFuncAST::print() {
    std::string str;
    str = ASTNode::print();
    str +=  "\"rand\" : {\n";
    str += getExpression()->print();
    str += "}\n";
    return str;

}

std::string RandFuncAST::diag(unsigned int parentID) {
    std::string str;
    str += "node" + std::to_string(ASTNode::getID()) + " [ label = \"rand:\"];\n";
    str += "node" + std::to_string(parentID) + " -> node" + std::to_string(ASTNode::getID()) + ";\n";
    str += getExpression()->diag(ASTNode::getID());
    return str;
}

std::string RandFuncAST::eval() {
    std::string str = "";
    str += "\n" + COMMENT + " RAND FUNC (builtin)\n";
    str += getExpression()->eval();
    str += "\tRAND \n";
    return str;
}

// ---------------------------------------------------------------------------


std::string SenseFuncAST::print() {
    std::string str;
    str = ASTNode::print();
    str +=  "\"sense\" : {\n";
    str += getExpression()->print();
    str += "}\n";
    return str;
}

std::string SenseFuncAST::diag(unsigned int parentID) {
    std::string str;
    str += "node" + std::to_string(ASTNode::getID()) + " [ label = \"move:\"];\n";
    str += "node" + std::to_string(parentID) + " -> node" + std::to_string(ASTNode::getID()) + ";\n";
    str += getExpression()->diag(ASTNode::getID());
    return str;
}

std::string SenseFuncAST::eval() {
    std::string str = "";
    str += "\n" + COMMENT + " SENSE FUNC (builtin)\n";
    str += getExpression()->eval();
    str += "\tSENSE \n";
    return str;
}

// ---------------------------------------------------------------------------

std::string IfStatementAST::print() {
    std::string str;
    str = ASTNode::print();
    str +=  "if: \"" + getName() + "\"";
//    str += this->expression->print();
//    str += this->block->print();

    str += getExpression()->print();
    str += getBlock()->print();

    str += "}\n";
    return str;
}

std::string IfStatementAST::diag(unsigned int parentID) {
    std::string str;
    str += "node" + std::to_string(getID()) + " [ label = \"if:\"];\n";
    str += "node" + std::to_string(parentID) + " -> node" + std::to_string(getID()) + ";\n";
    str += getExpression()->diag(getID());
    str += getBlock()->diag(getID());
    return str;
}

std::string IfStatementAST::eval() {
    std::string str = "";
    str += "\n" + COMMENT + " IF EXPRESSION ""\n";
    str += getExpression()->eval();
    std::string label = getUniqueIdentifier();
    str += "\tBRF @" + label + "\n";
    str += "\n" + COMMENT + "IF BLOCK\n";
    str += getBlock()->eval();
    str += LOCATION + label + "\n";
    return str;
}

// ---------------------------------------------------------------------------

std::string WhileStatementAST::print() {
    std::string str;
    str = ASTNode::print();

    str +=  "\"while\" : {\n" ;
    str += getExpression()->print();
    str +=  "},\n\"block\": {\n" ;
    str += getBlock()->print();
    str += "}\n";

    return str;
}

std::string WhileStatementAST::diag(unsigned int parentID) {
    std::string str;
    str += "node" + std::to_string(getID()) + " [ label = \"while:\"];\n";
    str += "node" + std::to_string(parentID) + " -> node" + std::to_string(getID()) + ";\n";

    str += getExpression()->diag(getID());
    str += getBlock()->diag(getID());

    return str;
}

std::string WhileStatementAST::eval() {

    std::string label1 = getUniqueIdentifier();
    std::string label2 = getUniqueIdentifier();

    std::string str = "";
    str += "\n" + COMMENT + " WHILE \n";

    str += LOCATION + label1 + "\n";
    str += getExpression()->eval();
    str += "\tBRF " + DESTINATION + label2 + "\n";
    str += "\n" + COMMENT + " WHILE BLOCK\n";
    str += getBlock()->eval();
    str += "\tJMP " + DESTINATION + label1 + "\n";
    str += LOCATION + label2 + "\n";

    return str;
}

// ---------------------------------------------------------------------------


std::string AssignmentStatementAST::print() {
    std::string str;
    str += "\"assignment\" : { \n";

    str += getIdentifier()->print();
    str += ",\n";
    str += getExpression()->print();
    str += "\n}\n";
    return str;
}

std::string AssignmentStatementAST::diag(unsigned int parentID) {
    std::string str;
    str += "node" + std::to_string(getID()) + " [ label = \"assign:\"];\n";
    str += "node" + std::to_string(parentID) + " -> node" + std::to_string(getID()) + ";\n";
    str += getIdentifier()->diag(getID());
    str += getExpression()->diag(getID());
    return str;
}

std::string AssignmentStatementAST::eval() {
    std::string str = "\n" + COMMENT + " Assignment Statement \n";
    str += getExpression()->eval();

    // TODO - NASTY HACK HERE - we edit out the LOAD and insert a SAVE instead!!!
    str += "\tSAVE " + getIdentifier()->eval().substr(6);
    return str;
}

// ---------------------------------------------------------------------------

std::string NumberAST::print() {
    std::string str;
    str =  "\"number\" : " + std::to_string(getNumber()) ;
    return str;
}

std::string NumberAST::diag(unsigned int parentID) {
    std::string str;
    str = "node" + std::to_string(getID()) + " [ label = \"" + std::to_string(getNumber()) + "\"];\n";
    str += "node" + std::to_string(parentID) + " -> node" + std::to_string(getID()) + ";\n";
    return str;
}

std::string NumberAST::eval() {
    std::string str;
    str = "\tPUSH " + std::to_string(getNumber());
    str += "\n";
    return str;
}

// ---------------------------------------------------------------------------

BinOperandAST::BinOperandAST(Token O, ASTNode *L, ASTNode *R) :
    ASTNode(), op(O)
{
    children.push_back(L);
    children.push_back(R);

    compatibility = CompatibilityType::expression;
}

std::string BinOperandAST::print() {

    std::string str;

    str = ASTNode::print();
    str += "\"expression\" : { \n";

    std::string opStr;

    switch (op) {
        case Token::OP_GT:
            opStr = ">";
            break;

        case Token::OP_GTE:
            opStr = ">=";
            break;

        case Token::OP_LT:
            opStr = "<";
            break;

        case Token::OP_LTE:
            opStr = "<=";
            break;

        case Token::OP_EQ:
            opStr = "==";
            break;

        case Token::OP_NE:
            opStr = "!=";
            break;

        case Token::OP_ADD:
            opStr = "+";
            break;

        case Token::OP_SUB:
            opStr = "-";
            break;

        case Token::OP_MUL:
            opStr = "*";
            break;

        case Token::OP_DIV:
            opStr = "/";
            break;

        case Token::OP_AND:
            opStr = "&& (and)";
            break;

        case Token::OP_OR:
            opStr = "|| (or)";
            break;

    }

    str +=  "\"operand\" : \"" + opStr + "\",\n" ;

    str += "\"lhs\" : {\n";
    str += children[0]->print();
    str += "\n},\n";

    str += "\"rhs\" : {\n";
    str += children[1]->print();
    str += "\n}\n";

    str += "}\n";


    return str;
}

std::string BinOperandAST::diag(unsigned int parentID) {

    std::string str;
    str += children[0]->diag(getID());

    std::string opStr;

    switch (op) {
        case Token::OP_GT:
            opStr = ">";
            break;

        case Token::OP_GTE:
            opStr = ">=";
            break;

        case Token::OP_LT:
            opStr = "<";
            break;

        case Token::OP_LTE:
            opStr = "<=";
            break;

        case Token::OP_EQ:
            opStr = "==";
            break;

        case Token::OP_NE:
            opStr = "!=";
            break;

        case Token::OP_ADD:
            opStr = "+";
            break;

        case Token::OP_SUB:
            opStr = "-";
            break;

        case Token::OP_MUL:
            opStr = "*";
            break;

        case Token::OP_DIV:
            opStr = "/";
            break;

        case Token::OP_AND:
            opStr = "&& (and)";
            break;

        case Token::OP_OR:
            opStr = "|| (or)";
            break;
    }

    str += "node" + std::to_string(getID()) + " [ label = \"op: " + opStr + "\"];\n";
    str += children[1]->diag(getID());
    str += "node" + std::to_string(parentID) + " -> node" + std::to_string(getID()) + ";\n";
    return str;
}

std::string BinOperandAST::eval() {

    std::string str = "";
    switch (op) {
        case Token::OP_ADD: {
            str += children[0]->eval() + children[1]->eval() + "\tADD";
            str += "\n";
            break;
        }
        case Token::OP_SUB: {
            str += children[0]->eval() + children[1]->eval() + "\tSUB";
            str += "\n";
            break;
        }
        case Token::OP_MUL: {
            str += children[0]->eval() + children[1]->eval() + "\tMUL";
            str += "\n";
            break;
        }
        case Token::OP_DIV: {
            str += children[0]->eval() + children[1]->eval() + "\tDIV";
            str += "\n";
            break;
        }
        case Token::OP_AND: {
            str += children[0]->eval() + children[1]->eval() + "\tAND";
            str += "\n";
            break;
        }

        case Token::OP_OR: {
            str += children[0]->eval() + children[1]->eval() + "\tOR";
            str += "\n";
            break;
        }

        case Token::OP_GT: {
            str += children[0]->eval() + children[1]->eval() + "\tGT";
            str += "\n";
            break;
        }
        case Token::OP_LT: {
            str += children[0]->eval() + children[1]->eval() + "\tLT";
            str += "\n";
            break;
        }
        case Token::OP_GTE: {
            str += children[0]->eval() + children[1]->eval() + "\tGTE";
            str += "\n";
            break;
        }
        case Token::OP_LTE: {
            str += children[0]->eval() + children[1]->eval() + "\tLTE";
            str += "\n";
            break;
        }
        case Token::OP_EQ: {
            str += children[0]->eval() + children[1]->eval();
            str += "\tEQU\n";
            break;
        }
        case Token::OP_NE: {
            str += children[0]->eval() + children[1]->eval() + "\tNE";
            str += "\n";
            break;
        }
        default: {
            std::cout << "Unknown BinaryExprAST opr: " << (int) op << std::endl;
            str += "; Unknown BinaryExprAST \n";
            break;
        }
    }
    return str;
}

// ---------------------------------------------------------------------------


std::string YieldAST::print() {
    std::string str;
    str = ASTNode::print();

    str += "yield";
    return str;
}

std::string YieldAST::diag(unsigned int parentID) {
    std::string str;
    str += "node" + std::to_string(getID()) + " [ label = \"yield: \"];\n";
    str += "node" + std::to_string(parentID) + " -> node" + std::to_string(getID()) + ";\n";
    return str;
}

std::string YieldAST::eval() {
    std::string str = "\tYIELD\n";
    return str;
}

// ---------------------------------------------------------------------------
