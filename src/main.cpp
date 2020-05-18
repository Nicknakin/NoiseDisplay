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
