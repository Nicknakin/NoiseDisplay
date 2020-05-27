#ifndef cellinc
#define cellinc

#include <SFML/Graphics.hpp>

class Cell: public sf::RectangleShape {
public:
    Cell(double x, double y, double sideLength);

    double getX();
    double getY();
    double getNoiseX();
    double getNoiseY();
    double getSideLength();

    void setX(double x);
    void setY(double y);
    void setNoiseX(double x);
    void setNoiseY(double y);
    void setSideLength(double sideLength);

private:
    double x, y, noiseX, noiseY, sideLength;

};

#endif
