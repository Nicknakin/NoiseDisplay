#ifndef cellinc
#define cellinc

#include <SFML/Graphics.hpp>

class Cell: public sf::RectangleShape {
public:
    Cell(float x, float y, float sideLength);

    float getX();
    float getY();
    float getNoiseX();
    float getNoiseY();
    float getSideLength();

    void setX(float x);
    void setY(float y);
    void setNoiseX(float x);
    void setNoiseY(float y);
    void setSideLength(float sideLength);

private:
    float x, y, noiseX, noiseY, sideLength;

};

#endif
