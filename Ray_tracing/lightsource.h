#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include "point.h"

struct LightSource
{
    Point coordinate;
    long double intensity;
    LightSource();
    LightSource(Point c, long double in);
};

#endif // LIGHTSOURCE_H
