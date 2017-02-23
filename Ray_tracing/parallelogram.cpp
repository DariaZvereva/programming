#include "parallelogram.h"
#include <cmath>

Parallelogram::Parallelogram()
        : base(UNDEF_POINT),
          first_vector(UNDEF_POINT),
          second_vector(UNDEF_POINT)
{}

Parallelogram::Parallelogram(const Point &vertex, const Point &v1, const Point &v2, png::rgb_pixel col)
        : base(vertex),
          first_vector(v1),
          second_vector(v2)
{
    color = col;
}

Plane Parallelogram::getContainingPlane() const {
    return Plane(base, first_vector, second_vector);
}

Point Parallelogram::intersection(const Ray &ray) const {
    Plane containing_plane = this->getContainingPlane();
    Point result = containing_plane.intersection(ray);

    Point check = result - base;
    long double scal = first_vector * second_vector;
    long double first_proj = check * first_vector;
    long double second_proj = check * second_vector;
    long double first_coordinate = (first_proj * (second_vector * second_vector) - second_proj * scal)
                        / ((first_vector * first_vector) * (second_vector * second_vector) - scal * scal);
    long double second_coordinate = (second_proj * (first_vector * first_vector) - first_proj * scal)
                       / ((first_vector * first_vector) * (second_vector * second_vector) - scal * scal);
    if (0 <= first_coordinate && first_coordinate <= 1 && second_coordinate <= 1 && 0 <= second_coordinate) {
        return result;
    }
    return UNDEF_POINT;
}

Point Parallelogram::getNormal(const Point point) const {
    return this->getContainingPlane().getNormal();
}


png::rgb_pixel Parallelogram::getColor(Point point) const {
    if (texture) {
        Point e_1 = first_vector.normalize();
        Point e_2 = e_1.vectorMultiply(this->getContainingPlane().getNormal());
        Point on_figure = point - base;
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
