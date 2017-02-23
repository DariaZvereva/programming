#include "ray.h"
#include <cmath>

Ray::Ray()
        : base(UNDEF_POINT),
          direction(UNDEF_POINT)
{}

Ray::Ray(Point b, Point d) {
    base = b;
    direction = d.normalize();
}


Ray Ray::moveForward() const {
    return Ray(base + direction * 2 * eps, direction);
}

Ray Ray::operator *(long double lambda) const {
    return Ray(base, direction * lambda);
}

Ray Ray::refract(Point point, Point normal, long double c) const {
    long double cos_a_2 = (direction * normal) * (direction * normal) + eps;
    long double sin_a_2 = 1 - cos_a_2;
    long double sin_b_2 = sin_a_2 / (c*c);
    long double cos_b_2 = 1 - sin_b_2 + eps;
    long double tg_a = sqrt(sin_a_2/cos_a_2);
    long double tg_b = sqrt(sin_b_2/cos_b_2);
    return Ray(point, direction + normal * (tg_a / tg_b - 1)).moveForward();
}
