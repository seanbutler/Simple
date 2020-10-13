//
// Created by sean on 27/06/2020.
//

#include "Instructions.h"
#include "VirtualMachine.h"
#include "../Simulation/Agent.h"

//#define DEBUG

void VM::Execute(unsigned int slice) {
    running = slice;
    if ( ! done ) {
        while (running > 0) {
            running--;
#ifdef DEBUG
            std::cout << "pc = " << programCounter << " instr = " << instructionNames[getCurrentInstruction()] << std::endl;

            std::cout << "instr = ";
            for(auto E : this->instructions) {
                std::cout << E << " ";
            }
            std::cout << std::endl;

            std::cout << "data = ";
            for(auto E : this->data) {
                std::cout << E << " ";
            }
            std::cout << std::endl;

            std::cout << "stacktop = " ;

            if ( stack.size() )
                std::cout << stack.top();
            else
                std::cout << "(unknown)";

            std::cout << std::endl;
#endif
            switch ((INS)getCurrentInstruction()) {

                case INS::NOP : {
                    // Do Nothing
                    break;
                }

                case INS::HALT : {
                    // Finish Running
                    running = false;
                    done = true;
                    break;
                }

                case INS::YIELD : {
                    // Stop Running to Restart again next iteration
                    running = false;
                    done = false;
                    break;
                }

                case INS::PUSH : {
                    incrProgramCounter();
                    stack.push(getCurrentInstruction());
                    break;
                }

                case INS::POP : {
                    stack.pop();
                    break;
                }

                case INS::ADD: {
                    int a = stack.top();
                    stack.pop();
                    int b = stack.top();
                    stack.pop();
                    stack.push(b + a);
                    break;
                }

                case INS::SUB: {
                    int a = stack.top();
                    this->stack.pop();
                    int b = stack.top();
                    stack.pop();
                    stack.push(b - a);
                    break;
                }

                case INS::MUL: {
                    int a = stack.top();
                    this->stack.pop();
                    int b = stack.top();
                    stack.pop();
                    stack.push(b * a);
                    break;
                }

                case INS::DIV: {
                    int a = stack.top();
                    if(a==0) a = 1;
                    stack.pop();
                    int b = stack.top();
                    stack.pop();
                    stack.push(b / a);
                    break;
                }

                case INS::AND: {
                    int a = stack.top();
                    stack.pop();
                    int b = stack.top();
                    stack.pop();
                    stack.push(b && a);
                    break;
                }

                case INS::OR: {
                    int a = stack.top();
                    stack.pop();
                    int b = stack.top();
                    stack.pop();
                    stack.push(b || a);
                    break;
                }

                case INS::EQU: {
                    int a = stack.top();
                    this->stack.pop();
                    int b = stack.top();
                    this->stack.pop();
                    if (b == a)
                        this->stack.push(1);
                    else
                        this->stack.push(0);
                    break;
                }

                case INS::NE: {
                    int a = stack.top();
                    this->stack.pop();
                    int b = stack.top();
                    this->stack.pop();
                    if (b != a)
                        this->stack.push(1);
                    else
                        this->stack.push(0);
                    break;
                }

                case INS::LT: {
                    int a = stack.top();
                    this->stack.pop();
                    int b = stack.top();
                    this->stack.pop();
                    if (b < a)
                        this->stack.push(1);
                    else
                        this->stack.push(0);
                    break;
                }

                case INS::LTE: {
                    int a = stack.top();
                    this->stack.pop();
                    int b = stack.top();
                    this->stack.pop();
                    if (b <= a)
                        this->stack.push(1);
                    else
                        this->stack.push(0);
                    break;
                }

                case INS::GT: {
                    int a = stack.top();
                    this->stack.pop();
                    int b = stack.top();
                    this->stack.pop();
                    if (b > a)
                        this->stack.push(1);
                    else
                        this->stack.push(0);
                    break;
                }

                case INS::GTE: {
                    int a = stack.top();
                    this->stack.pop();
                    int b = stack.top();
                    this->stack.pop();
                    if (b >= a)
                        this->stack.push(1);
                    else
                        this->stack.push(0);
                    break;
                }

                case INS::OUTPUT : {
                    std::cout << "out... " << this->stack.top() << std::endl;
                    this->stack.pop();
                    break;
                }

                case INS::INPUT : {
                    std::cout << "INPUT - not implemented" << std::endl;
                    break;
                }

                case INS::RAND : {
                    // NOTE takes one parameter and returns one parameter
                    // todo roll this out to a generic form with identifier
                    int param = this->stack.top();
                    param = param <= 0 ? 1 : param;
                    this->stack.pop();
                    int result = (int) (rand() % param);
                    this->stack.push(result);
                    break;
                }

                case INS::SENSE : {
                    // NOTE takes one parameter and returns one parameter
                    int param = this->stack.top();
                    this->stack.pop();
                    int result =-1;
                    int x = alienVars->get(0);
                    int y = alienVars->get(1);

                    switch(param%4){
                        case 0: {
                            result = DetectableComponent_Hurtful::directionToNearest(x, y);
                            break;
                        }
                        case 1: {
                            result = DetectableComponent_Healing::directionToNearest(x, y);
                            break;
                        }
                        case 2: {
                            result = DetectableComponent_Goal::directionToNearest(x, y);
                            break;
                        }
                        case 3: {
                            result = DetectableComponent_Player::directionToNearest(x, y);
                            break;
                        }
                    }

                    this->stack.push(result);
                    break;
                }

                case INS::MOVE : {
                    // NOTE takes one parameter and returns nothing
//                    std::pair<int, int> oldPosition(alienVars->get(0), alienVars->get(1));
                    std::pair<int, int> tmpPosition(alienVars->get(0), alienVars->get(1));

                    int dir = this->stack.top() % 4;
                    if (dir == 0) {
                        tmpPosition.second = alienVars->get(1) - 1;
//                        alienVars->set(1, alienVars->get(1) - 1);
                    }
                    else if (dir == 1) {
                        tmpPosition.first = alienVars->get(0) + 1;
//                        alienVars->set(0, alienVars->get(0) + 1);
                    }
                    else if (dir == 2) {
                        tmpPosition.second = alienVars->get(1) + 1;
//                        alienVars->set(1, alienVars->get(1) + 1);
                    }
                    else if (dir == 3) {
                        tmpPosition.first = alienVars->get(0) - 1;
//                        alienVars->set(0, alienVars->get(0) - 1);
                    }

                    if (CollisionComponent::collisionTest(tmpPosition.first, tmpPosition.second) == false ) {
                        // not occupied by agent, so express the movement to the alien vars to be picked up by the engine
                        alienVars->set(0, tmpPosition.first);
                        alienVars->set(1, tmpPosition.second);
                    }

                    this->stack.pop();
                    break;
                }

                case INS::SAVEALIEN: {
                    // NOTE takes one parameter and returns nothing
                    int val = stack.top();
                    stack.pop();
                    incrProgramCounter();
                    int addr = getCurrentInstruction();
                    alienVars->set(addr, (val%32)+1);
//                    alien[addr] = (val%32)+1;
                    break;
                }

                case INS::LOADALIEN: {
                    incrProgramCounter();
                    int addr = getCurrentInstruction();
                    int val;
//                    val = alien[addr];
                    val = alienVars->get(addr);
                    stack.push(val);
                    break;
                }

                case INS::SAVE: {
                    int val = stack.top();
                    stack.pop();
                    incrProgramCounter();
                    int addr = getCurrentInstruction();
                    data[addr] = val;
                    break;
                }

                case INS::LOAD: {
                    incrProgramCounter();
                    int addr = getCurrentInstruction();
                    int val = data[addr];
                    stack.push(val);
                    break;
                }

                case INS::JMP: {
                    incrProgramCounter();
                    int addr = getCurrentInstruction();
                    programCounter = addr-1;
                    break;
                }

                case INS::BRT: {
                    int a = stack.top();
                    this->stack.pop();

                    incrProgramCounter();
                    int addr = getCurrentInstruction();

//                    std::cout << "INS::BRT " << a << " " << addr << std::endl;

                    if (a!=0) {
                        programCounter = addr-1;
                    }
                    break;
                }

                case INS::BRF: {
                    int a = stack.top();
                    this->stack.pop();

                    incrProgramCounter();
                    int addr = getCurrentInstruction();

                    if (a==0) {
                        programCounter = addr-1;
                    }
                    //                std::cout << " " << programCounter << std::endl;
                    break;
                }

                case INS::END_OF_FILE: {
                    running = 0;
                    done = true;
                    break;
                }

                default: {
                    std::cerr << "Unknown Instruction pc = " << programCounter << " instr = " << getCurrentInstruction() << std::endl;
                    running = 0;
                    done = true;
                }

            }

            incrProgramCounter();
        }

    }

}
