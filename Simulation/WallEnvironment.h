//
// Created by sean on 27/07/2020.
//

#ifndef GLYNTH_WALLENVIRONMENT_H
#define GLYNTH_WALLENVIRONMENT_H

#include "../Utils/Walls.h"

class WallEnvironment : public Engine::Entity {

public:
    WallEnvironment(char D, unsigned int W, unsigned int H)
            :   Engine::Entity()
            ,   width(W)
            ,   height(H)
            ,   walls(D, W, H)
            ,   rectangle(sf::Vector2f (1, 1))
    {
        rectangle.setFillColor(sf::Color(0, 0, 0));
    }

    virtual ~WallEnvironment() {

    }

    virtual void Render(sf::RenderWindow *W) {
        for (unsigned int row = 0; row<height; row++) {
            for (unsigned int col = 0; col<width; col++) {
                if (walls.occupied(col, row)) {
                    rectangle.setPosition((col), (row));
                    W->draw(rectangle);
                }
            }
        }
    }

    virtual void Update(float deltaTime) {
//        std::cout << "Update MazeEnvironment" << std::endl;
    };

private:
    unsigned int width, height;
    util::Walls walls;
    sf::RectangleShape rectangle;
};

#endif //GLYNTH_WALLENVIRONMENT_H
