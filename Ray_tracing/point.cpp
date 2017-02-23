#include "point.h"
#include <cmath>


Point::Point()
        : x(std::numeric_limits<long double>::max()),
          y(std::numeric_limits<long double>::max()),
          z(std::numeric_limits<long double>::max())
{};

Point::Point(long double a, long double b, long double c): x(a), y(b), z(c) {};

long double Point::length(const Point& other) const {
   return sqrt((x - other.x)*(x-other.x) + (y-other.y)*(y-other.y) + (z-other.z)*(z-other.z));
}

long double Point::length() const {
   return sqrt(x*x + y*y + z*z);
}

Point Point::operator +(const Point& other) const {
   return Point(x+other.x, y+other.y, z+other.z);
}

Point Point::operator -(const Point& other) const {
   return Point(x-other.x, y-other.y, z-other.z);
}

bool Point::operator ==(const Point& other) const {
    return (fabsl(x - other.x) < eps) && (fabsl(y - other.y) < eps) && (fabsl(z - other.z) < eps);
}

Point Point::normalize() const {
    return Point(x/this->length(), y/this->length(), z/this->length());
}

long double Point::operator * (const Point& other) const {
    return (x * other.x + y * other.y + z * other.z);
}


Point Point::operator*(const long double &lambda) const {
    return Point(lambda * x, lambda * y, lambda * z);
}

Point Point::vectorMultiply(const Point &other) const {
    return Point(y*other.z - z*other.y, other.x*z - other.z*x, x*other.y-y*other.x);
}

bool Point::isCollinear(const Point &other) const {
    return (vectorMultiply(other).length() < eps);
}

bool Point::isOrtogonal(const Point &other) const {
    return (fabsl((*this)*other) < eps);
}

bool Point::isUndefined() const {
    return (*this == UNDEF_POINT);
}
