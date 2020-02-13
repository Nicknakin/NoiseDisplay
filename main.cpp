#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>

#include "perlin.h"

struct setting{
    const int width;
    const int height;
    const int sideLength;
    const int speed;
};

void setBlocks(std::vector<sf::RectangleShape*> blocks, int start, int end, struct setting config, Perlin* ngp){
    Perlin& ng = *ngp;
    const int gridWidth = config.width/config.sideLength, gridHeight = config.height/config.sideLength;
    for(int i = start; i <end; i++){
        int x = i%(gridWidth)*config.sideLength, y = floor(i/gridWidth)*config.sideLength;
        blocks[i]->setPosition(x, y);
        blocks[i]->setSize(sf::Vector2f{(float) config.sideLength, (float) config.sideLength});
        float xNoise = x/((float) config.width)*25, yNoise = y/((float) config.height)*25;
        float noise = ng(std::vector<float>{xNoise, yNoise, 0.f});
        sf::Uint8 col{(unsigned char) (noise*255)};
        blocks[i]->setFillColor(sf::Color{col, col, col});
    }
}

int main(int argc, char** argv){
    setting config = {
    /*width*/       (argc >= 2)? std::stoi(argv[1]): 800,
    /*height*/      (argc >= 3)? std::stoi(argv[2]): 800,
    /*sideLength*/  (argc >= 4)? std::stoi(argv[3]): 1,
    /*speed*/       (argc >= 5)? std::stoi(argv[4]): 60
    };
    
    const int gridWidth = config.width/config.sideLength, gridHeight = config.height/config.sideLength;
    
    Perlin ng{std::vector<int>{255,255,5}};

    std::vector<sf::RectangleShape*> blocks{};
    blocks.resize(gridWidth*gridHeight);
    
    //Create window with SFML and set framerate
    sf::RenderWindow window(sf::VideoMode(config.width,config.height), "SFML Project");
    window.setFramerateLimit(config.speed);
   
    for(int i = 0; i < blocks.size(); i++){
        blocks[i] = new sf::RectangleShape{sf::Vector2<float>{(float) config.sideLength, (float) config.sideLength}};
    }

    std::vector<std::thread> activeThreads{};
    unsigned threads = std::thread::hardware_concurrency();
    if(threads == 0 || true)
        setBlocks(blocks, 0, blocks.size(), config, &ng);
    else{
        for(int i = 0; i < threads; i++){
            activeThreads.push_back(std::thread(setBlocks, blocks, blocks.size()/threads*i, blocks.size()/threads*(i+1), config, &ng));
        }
    }   
    for(int i = 0; i < activeThreads.size(); i++){
        activeThreads[i].join();
    }   
    activeThreads.resize(0);
   
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
