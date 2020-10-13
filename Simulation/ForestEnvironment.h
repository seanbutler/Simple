//
// Created by sean on 27/07/2020.
//

#ifndef GLYNTH_FORESTENVIRONMENT_H
#define GLYNTH_FORESTENVIRONMENT_H

#include "../Utils/Forest.h"

class ForestEnvironment : public Engine::Entity {

public:
    ForestEnvironment(unsigned int W, unsigned int H)
            :   Entity()
            ,   width(W)
            ,   height(H)
            ,   trees(W, H)
            ,   rectangle(sf::Vector2f (1, 1))
    {
        rectangle.setFillColor(sf::Color(0, 0, 0));
    }

    virtual ~ForestEnvironment() {}

    virtual void Render(sf::RenderWindow *W){
        for (unsigned int row = 0; row<height; row++) {
            for (unsigned int col = 0; col<width; col++) {
                if (trees.occupied(col, row)) {
                    rectangle.setPosition((col), (row));
                    W->draw(rectangle);
                }
            }
        }
    }

    virtual void Update(float deltaTime) {
        std::cout << "Update ForestEnvironment" << std::endl;
    };

private:
    unsigned int width, height;
    util::Forest trees;
    sf::RectangleShape rectangle;
};

#endif //GLYNTH_FORESTENVIRONMENT_H
