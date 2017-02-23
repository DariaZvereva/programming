#include "sphere.h"
#include <math.h>
#include <assert.h>


Sphere::Sphere()
        : radius(0),
          center(UNDEF_POINT)
{}

Sphere::Sphere (const Point &c, const long double &r, png::rgb_pixel col)
        : radius(r),
          center(c)
{
    assert((r > 0) && !(center == UNDEF_POINT));
    color = col;
}


Point Sphere::intersection(const Ray &ray) const {
    long double a = ray.direction * ray.direction;
    long double b = ray.direction * (ray.base - center);
    long double c = (ray.base - center) * (ray.base - center) - radius * radius;
    long double D = b * b - a * c;
    if (D < 0) return UNDEF_POINT;
    long double t_1 = (-b + sqrt(D)) / a;
    long double t_2 = (-b - sqrt(D)) / a;
    if (t_1 < 0) {
        if (t_2 < 0) {
            return UNDEF_POINT;
        }
        t_1 = t_2;
    } else {
        if (t_2 >= 0) {
            if (t_2 < t_1) {
                t_1 = t_2;
            }
        }
    }
    return Point(ray.base + ray.direction * t_1);
}


Point Sphere::getNormal(const Point point) const {
    return (center - point).normalize();
}

bool Sphere::isRefractor() const {
    return refraction > 1;
}

void Sphere::setRefraction(long double c) {
    refraction = c;
}
