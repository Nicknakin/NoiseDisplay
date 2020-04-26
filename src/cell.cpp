#include "cell.h"

Cell::Cell(float x, float y, float sideLength): x{x}, y{y}, sideLength{sideLength}, sf::RectangleShape(sf::Vector2<float>(sideLength, sideLength)) {
    RectangleShape::setPosition(x, y);
}

float Cell::getX() {
    return x;
}
float Cell::getY() {
    return y;
}
float Cell::getNoiseX() {
    return noiseX;
}
float Cell::getNoiseY() {
    return noiseY;
}
float Cell::getSideLength() {
    return sideLength;
}

void Cell::setX(float x) {
    this->x = x;
}
void Cell::setY(float y) {
    this->y = y;
}
void Cell::setNoiseX(float x) {
    this->noiseX = x;
}
void Cell::setNoiseY(float y) {
    this->noiseY = y;
}
void Cell::setSideLength(float sideLength) {
    this->sideLength = sideLength;
}
