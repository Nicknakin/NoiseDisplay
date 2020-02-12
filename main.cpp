#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>

#include <SFML/Graphics.hpp>

#include "perlin.h"

struct setting{
    const int width;
    const int height;
    const int sideLength;
    const int speed;
};

int main(int argc, char** argv){
    setting config = {(argc >= 2)? std::stoi(argv[1]): 1920/2, (argc >= 3)? std::stoi(argv[2]): 1080/2, (argc >= 4)? std::stoi(argv[3]): 10, (argc >= 5)? std::stoi(argv[4]): 60};
    
    const int gridWidth = config.width/config.sideLength, gridHeight = config.height/config.sideLength;

    Perlin ng{std::vector<int>{255,255,255}};

    std::vector<sf::RectangleShape*> blocks{};
    blocks.resize(gridWidth*gridHeight);
    
    //Create window with SFML and set framerate
    sf::RenderWindow window(sf::VideoMode(config.width,config.height), "SFML Project");
    window.setFramerateLimit(config.speed);
    for(int i = 0; i < blocks.size(); i++){
        blocks[i] = new sf::RectangleShape{sf::Vector2<float>{(float) config.sideLength, (float) config.sideLength}};
        int x = i%(gridWidth)*config.sideLength, y = floor(i/gridWidth)*config.sideLength;
        blocks[i]->setPosition(x, y);
        blocks[i]->setSize(sf::Vector2f{(float) config.sideLength, (float) config.sideLength});
        float xNoise = x/((float) config.width)*25, yNoise = y/((float) config.height)*25;
        float noise = ng(std::vector<float>{xNoise, yNoise, 0.5f});
        unsigned int col = noise*255;
        blocks[i]->setFillColor(sf::Color{col, col, col});
    }

    while (window.isOpen()) {
    window.clear();
        for(auto block : blocks){
            window.draw(*block);
        }
        window.display();
    }
    std::cout << std::endl;
    return 0;
}
