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
    setting config = {(argc >= 2)? std::stoi(argv[1]): 1920/2, (argc >= 3)? std::stoi(argv[2]): 1080/2-1, (argc >= 4)? std::stoi(argv[3]): 10, (argc >= 5)? std::stoi(argv[4]): 60};
  
    Perlin ng{std::vector<int>{255,255,255}};

    std::vector<sf::RectangleShape> blocks{};
    blocks.resize((config.width*config.height)/(config.sideLength*config.sideLength), sf::RectangleShape{sf::Vector2<float>{config.sideLength, config.sideLength}});
    
    //Create window with SFML and set framerate
    sf::RenderWindow window(sf::VideoMode(config.width,config.height), "SFML Project");
    window.setFramerateLimit(config.speed);
    for(int i = 0; i < blocks.size(); i++){
        auto &block = blocks[i];
        int x = i%(config.width/config.sideLength)*config.sideLength, y = floor(i/(config.width/config.sideLength))*config.sideLength;
        block.setPosition(x, y);
        float xNoise = x/((float) config.width)*255, yNoise = y/((float) config.height)*255;
        float noise = ng(std::vector<float>{xNoise, yNoise, 0.5f});
        int col = noise*255;
        block.setFillColor(sf::Color{col});
    }

    while (window.isOpen()) {
        
        std::cout << ',';
        for(auto &block : blocks){
            window.draw(block);
        }
        std::cout << '.';
        window.display();
    }
    std::cout << std::endl;
    return 0;
}
