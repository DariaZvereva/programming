#ifndef PARALLELOGRAMM_H
#define PARALLELOGRAMM_H
#include "figure.h"
#include "plane.h"
class Parallelogram : public Figure
{
    Point base;
    Point first_vector;
    Point second_vector;
public:
    Parallelogram();
    Parallelogram(const Point& vertex, const Point& v1, const Point& v2, png::rgb_pixel col);
    Point intersection(const Ray &ray) const override;
    Plane getContainingPlane() const;
    Point getNormal(const Point point) const override;
    png::rgb_pixel getColor(Point point) const override;
};

#endif // PARALLELOGRAMM_H
