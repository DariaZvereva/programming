#ifndef MATERIAL_H
#define MATERIAL_H
#include <string>
#include <map>
#include <png++/png.hpp>
#include <memory>

struct material
{
    std::string name;
    png::rgb_pixel color;
    long double alpha;
    long double reflect;
    long double refract;
    material();
    material(std::string s, png::rgb_pixel col, long double a, long double refl, long double refr);
};

#endif // MATERIAL_H
