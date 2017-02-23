#ifndef RAY_H
#define RAY_H

#include "point.h"

struct Ray
{
    Point base;
    Point direction;
    Ray();
    Ray(Point b, Point d);
    Ray moveForward() const;
    Ray operator *(long double lambda) const;
    Ray refract(Point point, Point normal, long double c) const;
};

#endif // RAY_H
