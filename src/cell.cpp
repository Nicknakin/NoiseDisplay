#include "cell.h"

Cell::Cell(double x, double y, double sideLength): sf::RectangleShape(sf::Vector2<float>((float) sideLength, (float) sideLength)), x{x}, y{y}, sideLength{sideLength} {
    RectangleShape::setPosition(x, y);
}

double Cell::getX() {
    return x;
}
double Cell::getY() {
    return y;
}
double Cell::getNoiseX() {
    return noiseX;
}
double Cell::getNoiseY() {
    return noiseY;
}
double Cell::getSideLength() {
    return sideLength;
}

void Cell::setX(double x) {
    this->x = x;
}
void Cell::setY(double y) {
    this->y = y;
}
void Cell::setNoiseX(double x) {
    this->noiseX = x;
}
void Cell::setNoiseY(double y) {
    this->noiseY = y;
}
void Cell::setSideLength(double sideLength) {
    this->sideLength = sideLength;
}
