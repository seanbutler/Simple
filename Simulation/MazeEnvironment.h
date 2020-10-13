//
// Created by sean on 24/07/2020.
//

#ifndef GLYNTH_MAZEENVIRONMENT_H
#define GLYNTH_MAZEENVIRONMENT_H

#include "../Utils/Maze.h"

class MazeEnvironment : public Engine::Entity {
public:
    MazeEnvironment(unsigned int W, unsigned int H)
    :   Entity()
    ,   width(W)
    ,   height(H)
    ,   maze(W, H)
    ,   rectangle(sf::Vector2f (1, 1))
    {
        rectangle.setFillColor(sf::Color(0, 0, 0));
    }

    virtual ~MazeEnvironment() {}

    virtual void Render(sf::RenderWindow *W){
        for (unsigned int row = 0; row<height; row++) {
            for (unsigned int col = 0; col<width; col++) {
                if (maze.wall(row, col)) {

                    rectangle.setPosition((row), (col));
                    W->draw(rectangle);

//                    rectangle.setPosition((row*2), (col*2));
//                    W->draw(rectangle);
//
//                    rectangle.setPosition((row*2+1), (col*2));
//                    W->draw(rectangle);
//
//                    rectangle.setPosition((row*2), (col*2+1));
//                    W->draw(rectangle);
//
//                    rectangle.setPosition((row*2+1), (col*2+1));
//                    W->draw(rectangle);
                }
            }
        }
    }

    virtual void Update(float deltaTime) {
//        std::cout << "Update MazeEnvironment" << std::endl;
    };

private:
    unsigned int width, height;
    util::Maze maze;
    sf::RectangleShape rectangle;
};
#endif //GLYNTH_MAZEENVIRONMENT_H
