#ifndef POINT_H
#define POINT_H
#include <limits>

long double const eps = 10e-15;

struct Point
{
    long double x, y, z;
    Point();
    Point(long double a, long double b, long double c);

    long double length(const Point& other) const;
    long double length() const;
    Point operator +(const Point& other) const;
    Point operator -(const Point& other) const;
    long double operator *(const Point& other) const;
    bool operator ==(const Point& other) const;
    Point normalize() const;
    Point operator*(const long double &lambda) const;
    Point vectorMultiply(const Point &other) const;
    bool isCollinear(const Point &other) const;
    bool isOrtogonal(const Point &other) const;
    bool isUndefined() const;
};

const Point UNDEF_POINT = Point(std::numeric_limits<long double>::max(), std::numeric_limits<long double>::max(), std::numeric_limits<long double>::max());

#endif // POINT_H
