#ifndef PLANE_H
#define PLANE_H
#include "point.h"
#include "ray.h"

class Plane
{
private:
    Point base;
    Point first_vector;
    Point second_vector;
public:
    Plane(const Point &base, const Point &v1, const Point &v2);
    Plane();
    Point getNormal() const;
    Point intersection(const Ray& ray) const;
    Point getFirstVec() const;
    Point getSecondVec() const;
    Point getBase() const;
};

#endif // PLANE_H
