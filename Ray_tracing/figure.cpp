#include "figure.h"

Figure::Figure()
        : color(0, 0, 0),
          reflection(0) {
}
Figure::~Figure() {}

png::rgb_pixel Figure::getColor(Point p) const {
    return color;
}



void Figure::setReflection(long double r) {
    reflection = r;
}

Ray Figure::getReflectedRay(const Point point, const Point vector) const {
    Point normal = this->getNormal(point);
    Ray reflected = Ray(point, vector - normal * (vector * normal) * 2);
    return reflected.moveForward();
}

long double Figure::getReflection() const {
    return reflection;
}

bool Figure::isRefractor() const {
    return false;
}

long double Figure::getRefraction() const {
    return refraction;
}

void Figure::setRefraction(long double c) {}

void Figure::setTexture(std::string s, long double c) {
    png::image<png::rgb_pixel>* image = new png::image<png::rgb_pixel>(s);
    texture = std::shared_ptr<png::image<png::rgb_pixel>>(image);
    scale = c;
    assert(scale > 0);
}
