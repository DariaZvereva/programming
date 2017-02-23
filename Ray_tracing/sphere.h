#ifndef SPHERE_H
#define SPHERE_H
#include <png++/color.hpp>
#include "figure.h"
class Sphere : public Figure
{
    Point center;
    long double radius;
public:
    Sphere();
    Sphere(const Point& c, const long double &r, png::rgb_pixel col);
    Point intersection(const Ray &ray) const override;
    Point getNormal(const Point point) const override;
    bool isRefractor() const override;
    void setRefraction(long double c) override;
};

#endif // SPHERE_H
