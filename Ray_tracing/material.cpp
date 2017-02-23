#include "material.h"

material::material()
        : name(""),
          color(),
          alpha(0),
          reflect(0),
          refract(0) {}


material::material(std::string s, png::rgb_pixel col, long double a,
         long double refl, long double refr)
        : name(s),
          color(col),
          alpha(a),
          reflect(refl),
          refract(refr) {}
