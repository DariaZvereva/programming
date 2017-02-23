#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "figure.h"
#include "plane.h"

class Triangle : public Figure
{
    Point base;
    Point first_vector;
    Point second_vector;
public:
    Triangle();
    Triangle(const Point& vertex1, const Point& vertex2, const Point& vertex3, png::rgb_pixel col);
    Plane getContainingPlane() const;
    Point intersection(const Ray &ray) const override;
    Point getNormal(const Point point) const override;
    png::rgb_pixel getColor(Point point) const override;
    long double getSquare() const;
};

#endif // TRIANGLE_H
