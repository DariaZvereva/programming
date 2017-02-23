#include "quadrilateral.h"
#include "triangle.h"
#include <cmath>

Quadrilateral::Quadrilateral()
        : v1(UNDEF_POINT),
          v2(UNDEF_POINT),
          v3(UNDEF_POINT),
          v4(UNDEF_POINT)
{}

Quadrilateral::Quadrilateral(Point a, Point b, Point c, Point d, png::rgb_pixel col)
        : v1(a),
          v2(b),
          v3(c),
          v4(d)
{
    color = col;
    Plane containing_plane = getContainingPlane();
    Point plane_normal = containing_plane.getNormal();
    Ray ray_to_v4(v4, plane_normal);
    if (containing_plane.intersection(ray_to_v4) == UNDEF_POINT) {
        ray_to_v4 = ray_to_v4 * (-1);
    }
    v4 = containing_plane.intersection(ray_to_v4);
}

Plane Quadrilateral::getContainingPlane() const {
    return Plane(v1, Point(v2 - v1), Point(v4 - v1));
}


Point Quadrilateral::getNormal(const Point point) const{
    return Point(getContainingPlane().getNormal());
}

Point Quadrilateral::intersection(const Ray &ray) const {
    Triangle first(v1, v2, v3, color);
    Triangle second(v1, v2, v4, color);
    Triangle third(v1, v3, v4, color);
    Triangle fourth(v2, v3, v4, color);
    long double sqr_1 = first.getSquare();
    long double sqr_2 = second.getSquare();
    long double sqr_3 = third.getSquare();
    long double sqr_4 = fourth.getSquare();
    Point point;
    if (abs(sqr_1 + sqr_3 - abs(sqr_2 - sqr_4)) < eps) {
        point = first.intersection(ray);
        if (point == UNDEF_POINT) {
            point = third.intersection(ray);
        }
    } else {
        point = second.intersection(ray);
        if (point == UNDEF_POINT) {
            point = fourth.intersection(ray);
        }
    }
    return point;
}

png::rgb_pixel Quadrilateral::getColor(Point point) const {
    if (texture) {
        Point e_1 = (v2 - v1).normalize();
        Point e_2 = e_1.vectorMultiply(this->getContainingPlane().getNormal());
        Point on_figure = point - v1;
        double first_proj = on_figure * e_1 / scale;
        double second_proj = on_figure * e_2 / scale;
        second_proj *= ((long double)texture->get_height() / (long double)texture->get_width());
        double integer;
        double remainder_1 = std::modf(first_proj, &integer);
        if (remainder_1 < 0) {
            remainder_1 += 1;
        }
        double remainder_2 = std::modf(second_proj, &integer);
        if (remainder_2 < 0) {
            remainder_2 += 1;
        }
        return (*texture)[size_t(remainder_1 * (texture->get_height() - 1))][size_t(remainder_2 * (texture->get_width() - 1))];
    } else {
        return color;
    }
}
