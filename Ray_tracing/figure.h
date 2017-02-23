#ifndef FIGURE_H
#define FIGURE_H

#include "point.h"
#include "ray.h"
#include <memory>
#include <png++/png.hpp>
#include <string>

class Figure
{
protected:
    png::rgb_pixel color;
    long double reflection = 0;
    long double refraction = 0;
    std::shared_ptr<png::image<png::rgb_pixel>> texture;
    long double scale;
public:
    Figure();
    virtual Point intersection(const Ray &ray) const = 0;
    virtual Point getNormal(const Point point) const = 0;
    void setReflection(long double r);
    virtual void setTexture(std::string s, long double c);
    long double getRefraction() const;
    virtual png::rgb_pixel getColor(Point point) const;
    Ray getReflectedRay(const Point point, const Point vector) const;
    long double getReflection() const;
    virtual void setRefraction(long double c);
    virtual bool isRefractor() const;
    virtual ~Figure();
};

#endif // FIGURE_H
