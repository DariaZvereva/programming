#include "plane.h"
#include <assert.h>

Plane::Plane() {
    base = UNDEF_POINT;
    first_vector = UNDEF_POINT;
    second_vector = UNDEF_POINT;
}

Plane::Plane(const Point &p, const Point &v1, const Point &v2){
    assert(! v1.isCollinear(v2));
    base = p;
    first_vector = v1;
    second_vector = v2;
    Point normal = this->getNormal();
    second_vector = second_vector.normalize();
    first_vector = normal.vectorMultiply(second_vector);
}

Point Plane::getNormal() const {
    return first_vector.vectorMultiply(second_vector).normalize();
}

Point Plane::intersection(const Ray& ray) const {
    Point n = this->getNormal();
    if (ray.direction.isOrtogonal(n)) {
        return UNDEF_POINT;
    }
    long double t = (base * n - ray.base * n) / (ray.direction * n);
    if (t < 0) {
        return UNDEF_POINT;
    }
    return Point(ray.base + ray.direction * t);
}

Point Plane::getFirstVec() const {
    return first_vector;
}

Point Plane::getSecondVec() const {
    return second_vector;
}

Point Plane::getBase() const {
    return base;
}

