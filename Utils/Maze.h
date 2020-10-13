//
// Created by sean on 25/07/2020.
//

#ifndef GLYNTH_MAZE_H
#define GLYNTH_MAZE_H

#include <string>
#include <vector>

namespace util {

    class Maze {
    public:
        Maze(unsigned int W=32, unsigned int H=32 )
        : width(W)
        , height(H)
        , size(width*height)
        {
            for (unsigned int n=0; n< size; n++)
            {
                cells.push_back(true);
            }

            Generate(4, 4);
        }

        void Generate(unsigned int startX = 2, unsigned int startY = 2) {
            generate2(startX, startY);
        }

        void generate2(unsigned int currentCellX, unsigned int currentCellY) {
            std::string randomDirections = GenerateRandomDirections();

            for (char c : randomDirections) {

                switch(c) {
                    case 'N':
                        if (currentCellY-2>0)
                        {
                            if (cells[currentCellX + ((currentCellY - 2)*width)] != 0 )
                            {
                                cells[currentCellX + ((currentCellY - 2)*width)] = 0;   // make cell empty
                                cells[currentCellX + ((currentCellY - 1)*width)] = 0;   // make wall between emtpy (also a cell)
                                generate2(currentCellX, currentCellY - 2);
                            }
                        }
                        break;

                    case 'S':
                        if ( currentCellY+2<height)
                        {
                            if (cells[currentCellX + ((currentCellY + 2)*width)] != 0 )
                            {
                                cells[currentCellX + ((currentCellY + 2)*width)] = 0;   // make cell empty
                                cells[currentCellX + ((currentCellY + 1)*width)] = 0;   // make wall between emtpy (also a cell)
                                generate2(currentCellX, currentCellY + 2);
                            }
                        }
                        break;

                    case 'W':
                        if ( currentCellX-2>0)
                        {
                            if (cells[currentCellX-2 + (currentCellY * width)] != 0 )
                            {
                                cells[currentCellX-2 + (currentCellY * width)] = 0;   // make cell empty
                                cells[currentCellX-1 + (currentCellY * width)] = 0;   // make wall between emtpy (also a cell)
                                generate2(currentCellX - 2, currentCellY);
                            }
                        }
                        break;

                    case 'E':
                        if ( currentCellX+2<width)
                        {
                            if (cells[currentCellX+2 + (currentCellY*width)] != 0 )
                            {
                                cells[currentCellX+2 + (currentCellY * width)] = 0;   // make cell empty
                                cells[currentCellX+1 + (currentCellY * width)] = 0;   // make wall between emtpy (also a cell)
                                generate2(currentCellX + 2, currentCellY);
                            }
                        }
                        break;
                }
            }
        }

        std::string GenerateRandomDirections() {
            std::string directions = "NESW";

            for (unsigned int times=0; times<8; times++) {
                for (unsigned int from = 0; from < 4; from++) {
                    unsigned int to = rand() % 4;
                    char tmp = directions[to];
                    directions[to] = directions[from];
                    directions[from] = tmp;
                }
            }

            return directions;
        }

        bool occupied(unsigned int row, unsigned int col) {
            return cells[row*width+col];
        }

        bool wall(unsigned int row, unsigned int col) {
            return occupied(row, col);
        }

        bool passage(unsigned int row, unsigned int col) {
            return !occupied(row, col);
        }


    private:
        unsigned int width, height, size;
        std::vector<bool> cells;

    };


};



#endif //GLYNTH_MAZE_H
