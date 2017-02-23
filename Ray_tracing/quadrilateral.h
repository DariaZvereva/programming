#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H
#include "point.h"
#include "figure.h"
#include "plane.h"

class Quadrilateral : public Figure
{
    Point v1, v2, v3, v4;
public:
    Quadrilateral();
    Quadrilateral(Point a, Point b, Point c, Point d, png::rgb_pixel col);
    Point intersection(const Ray &ray) const;
    Point getNormal(const Point point) const;
    png::rgb_pixel getColor(Point point) const override;
    Plane getContainingPlane() const;
};

#endif // QUADRILATERAL_H
