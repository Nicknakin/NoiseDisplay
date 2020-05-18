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
#include "cell.h"

struct setting {
    const int width;
    const int height;
    const int sideLength;
    const int speed;
};

void setBlocks(std::vector<Cell*> blocks, int start, int end, struct setting config, Perlin* ngp){
    Perlin& ng = *ngp;
    const int gridWidth = config.width/config.sideLength, gridHeight = config.height/config.sideLength;
    const float aspectRatio = (float) gridWidth/ (float) gridHeight;
    for(int i = start; i <end; i++){
        int x = i%(gridWidth)*config.sideLength, y = floor(i/gridWidth)*config.sideLength;
        float xNoise = x/((float) config.width)*25, yNoise = y/((float) config.height)*25/aspectRatio;
        float noise = ng(std::vector<float>{xNoise, yNoise, 0.f});
        sf::Uint8 col{(unsigned char) (noise*255)};
        blocks[i]->setNoiseX(xNoise);
        blocks[i]->setNoiseY(yNoise);
        blocks[i]->setFillColor(sf::Color{col, col, col});
    }
}

void updateBlocks(std::vector<Cell*> blocks, int start, int end, Perlin *ngp, float z){
    auto ng = *ngp;
    for(auto [block, i] = std::tuple{blocks[start], start}; i < end; block = blocks[++i]){
            sf::Uint8 col = 255*ng(std::vector<float>{block->getNoiseX(), block->getNoiseY(), z});
            block->setFillColor(sf::Color{col, 0, 0}); 
        }
}

int main(int argc, char** argv) {
    setting config = {
        /*width*/       (argc >= 2)? std::stoi(argv[1]): 800,
        /*height*/      (argc >= 3)? std::stoi(argv[2]): 800,
        /*sideLength*/  (argc >= 4)? std::stoi(argv[3]): 5,
        /*speed*/       (argc >= 5)? std::stoi(argv[4]): 60
    };

    const int gridWidth = config.width/config.sideLength, gridHeight = config.height/config.sideLength;

    Perlin ng{25, 25, 25};

    std::vector<Cell*> blocks{};
    blocks.resize(gridWidth*gridHeight);

    //Create window with SFML and set framerate
    sf::RenderWindow window(sf::VideoMode(config.width,config.height), "SFML Project");
    window.setFramerateLimit(config.speed);

    for(int i = 0; i < (int) blocks.size(); i++) {
        blocks[i] = new Cell{(float) (i%gridWidth*config.sideLength), (float) (floor(i/gridWidth)*config.sideLength), float (config.sideLength)};
    }

    std::vector<std::thread> activeThreads{};
    unsigned threads = std::thread::hardware_concurrency();
    if(threads == 0)
        setBlocks(blocks, 0, blocks.size(), config, &ng);
    else {
        for(int i = 0; i < (int) threads; i++) {
            activeThreads.push_back(std::thread(setBlocks, blocks, blocks.size()/threads*i, blocks.size()/threads*(i+1), config, &ng));
        }
    }
    for(int i = 0; i < (int) activeThreads.size(); i++) {
        activeThreads[i].join();
    }
    activeThreads.resize(0);

    float z = 0.0f;

    while (window.isOpen()) {
        window.clear();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if(threads == 0)
            updateBlocks(blocks, 0, blocks.size(), &ng, z);
        else {
            for(int i = 0; i < (int) threads; i++) {
                activeThreads.push_back(std::thread(updateBlocks, blocks, blocks.size()/threads*i, blocks.size()/threads*(i+1), &ng, z));
            }
        }
        for(int i = 0; i < (int) activeThreads.size(); i++) {
            activeThreads[i].join();
        }
        activeThreads.resize(0);
        z += 0.01f;

        for(auto block : blocks) {
            window.draw(*block);
        }
        window.display();
    }
    std::cout << std::endl;
    return 0;
}
