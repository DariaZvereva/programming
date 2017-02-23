#include "lightsource.h"

LightSource::LightSource()
        : coordinate(UNDEF_POINT),
          intensity(0)
{}

LightSource::LightSource(Point c, long double in)
    : coordinate(c),
      intensity(in)
{}
