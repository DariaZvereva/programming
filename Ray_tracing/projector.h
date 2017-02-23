#ifndef PROJECTOR_H
#define PROJECTOR_H
#include "point.h"
#include "plane.h"
#include "ray.h"
#include "figure.h"
#include "lightsource.h"
#include <sys/types.h>
#include <vector>
#include <memory>
#include <png++/png.hpp>
#include <string>
#include "material.h"
#include <map>
#include "thread_pool.h"


class Projector
{
private:
    thread_pool<bool> manager;
    Point point_of_view;
    Plane screen;
    size_t width;
    size_t height;
    size_t scale;
    std::vector<std::shared_ptr<Figure> > figures;
    std::vector<std::shared_ptr<LightSource> > lights;
    png::image<png::rgb_pixel> image;
    std::map<std::string, material> materials;
    long double default_intens = 0.5;
public:
    Projector();
    Projector(Point &observer, Plane &scr, size_t n, size_t m, size_t scl);
    void addFigure(std::shared_ptr<Figure> fig);
    void addLight(std::shared_ptr<LightSource> light);
    void setObserver(long double x, long double y, long double z);
    void setBottomLeft(long double x, long double y, long double z);
    void setTopLeft(long double x, long double y, long double z);
    void setTopRight(long double x, long double y, long double z);
    bool definePixel(int i, int j);
    void project();
    png::rgb_pixel getColor(const Ray& ray, size_t rec);
    png::rgb_pixel getLightness(Point point, png::rgb_pixel col, std::shared_ptr<Figure> figure, Ray ray) const;
    png::rgb_pixel getReflection(const Point point, const Point vec) const;
    void antiAliasing();
    void show();
    void read(std::string s);
};

#endif // PROJECTOR_H
