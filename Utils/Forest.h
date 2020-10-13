//
// Created by sean on 27/07/2020.
//

#ifndef GLYNTH_FOREST_H
#define GLYNTH_FOREST_H

#include <string>
#include <vector>

namespace util {

    class Forest {

    public:
        Forest(unsigned int W=32, unsigned int H=32 )
            : width(W)
            , height(H)
            , size(width*height)
        {
            for (unsigned int n = 0; n < size; n++)
            {
                cells.push_back(false);
            }
            Generate(64);
        }

        void Generate(int size) {
            for (int n=0;n<size; n++) {
                int x = (rand()%width + rand()%width)/2;
                int y = (rand()%height + rand()%height)/2;
                Tree(x, y);
            }
        }

        void Tree(unsigned int startX, unsigned int startY) {
            cells[(startX) + (startY * width)] = true;
        }

        bool occupied(unsigned int row, unsigned int col) {
            return cells[row*width+col];
        }

    private:
        unsigned int width, height, size;
        std::vector<bool> cells;
    };

}


#endif //GLYNTH_FOREST_H
