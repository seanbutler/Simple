//
// Created by sean on 25/07/2020.
//

#ifndef GLYNTH_WALLS_H
#define GLYNTH_WALLS_H

#include <string>
#include <vector>

namespace util {

    class Walls {
    public:
        Walls(char D = 'V', unsigned int W=32, unsigned int H=32 )
            : direction(D)
            , width(W)
            , height(H)
            , size(width*height)
        {
            for (unsigned int n=0; n< size; n++)
            {
                cells.push_back(false);
            }

            Generate();
        }

        void Generate() {

            if (direction == 'V') {
                int topMargin = 1 + (rand() % ((height/2) - 1));
                int leftMargin = 1 + (rand() % (width - 1));

                Wall('V', topMargin, leftMargin, height/2);

                leftMargin = 1 + (rand() % ((height/2) - 1));
                topMargin = 1 + (rand() % (width - 1));

                Wall('H', topMargin, leftMargin, width/2);
            }

        }

        void Wall(char dir, unsigned int startX, unsigned int startY, unsigned int length) {

            if ( dir == 'V')
                for (int n=0;n<length;n++) {
                    cells[(startX+n) + (startY * width)] = true;
                }
            else
                for (int n=0;n<length;n++) {
                    cells[startX + ((startY+n) * width)] = true;
                }
        }

        std::string ShuffleString(std::string cards) {

            for (unsigned int times=0; times<cards.size()*2; times++) {
                for (unsigned int from = 0; from < cards.size(); from++) {
                    unsigned int to = rand() % cards.size();
                    char tmp = cards[to];
                    cards[to] = cards[from];
                    cards[from] = tmp;
                }
            }

            return cards;
        }

        bool occupied(unsigned int row, unsigned int col) {
            return cells[row*width+col];
        }

    private:
        char direction;
        unsigned int width, height, size;
        std::vector<bool> cells;

    };


};



#endif //GLYNTH_WALLS_H
