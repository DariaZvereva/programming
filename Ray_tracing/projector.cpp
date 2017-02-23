#include "projector.h"
#include <assert.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include "sphere.h"
#include "triangle.h"
#include "quadrilateral.h"
#include <functional>

size_t CONCURRENCY = 4;

Projector::Projector()
            : manager(CONCURRENCY),
              point_of_view(UNDEF_POINT),
              screen(),
              width(9),
              height(12),
              scale(100),
              figures(),
              lights(),
              image(height * scale, width * scale),
              materials()
{}

Projector::Projector(Point &observer, Plane &plane, size_t n, size_t m, size_t scl)
            : manager(CONCURRENCY),
              width(n),
              height(m),
              scale(scl),
              figures(),
              lights(),
              image(height * scale, width * scale) {
    point_of_view = observer;
    Point normal = plane.getNormal();
    Ray view = Ray(point_of_view, normal);
    Point screen_base = UNDEF_POINT;
    Point trying = plane.intersection(view);
    if(!trying.isUndefined()) {
        screen_base = plane.intersection(view);
    } else {
        normal = normal * (-1);
        view = Ray(point_of_view, normal);
        trying = plane.intersection(view);
        if (!trying.isUndefined()) {
            screen_base = plane.intersection(view);
        }
    }
    assert(!(screen_base == UNDEF_POINT));
    screen = Plane(screen_base, plane.getFirstVec(), plane.getSecondVec());
}

png::rgb_pixel blend(png::rgb_pixel first_col, png::rgb_pixel second_col, long double alpha) {
    return png::rgb_pixel((int)first_col.red * alpha + (int)second_col.red * (1 - alpha),
                          (int)first_col.green  * alpha + (int)second_col.green * (1 - alpha),
                          (int)first_col.blue * alpha + (int)second_col.blue * (1 - alpha));
}


png::rgb_pixel Projector::getColor(const Ray& ray, size_t rec) {
    png::rgb_pixel color = png::rgb_pixel(0, 0, 0);
    std::shared_ptr<Figure> figure;
    Point nearest = UNDEF_POINT;
    for (size_t i = 0; i < figures.size(); ++i) {
        if ((ray.base - figures[i]->intersection(ray)).length() < (ray.base - nearest).length()) {
            nearest = figures[i]->intersection(ray);
            color = figures[i]->getColor(nearest);
            figure = figures[i];
        }
    }
    if (figure) {
        long double alpha = figure->getReflection()/100;
        if (figure->isRefractor()) {
            Ray refractedRay;
            if (figure->getNormal(nearest) * ray.direction > 0) {
                refractedRay = ray.refract(nearest, figure->getNormal(nearest), figure->getRefraction());
            } else {
                refractedRay = ray.refract(nearest, figure->getNormal(nearest) * (-1), (long double) (1 / figure->getRefraction()));
            }
            color = getColor(refractedRay, rec+1);
            if (alpha > 0 && rec < 4) {
                Ray reflected_ray = figure->getReflectedRay(nearest, ray.direction);
                png::rgb_pixel reflected_col = getColor(reflected_ray, rec+1);
                color = blend(reflected_col, color, alpha);
            }
        } else {
            if (alpha > 0 && rec < 4) {
                Ray reflected_ray = figure->getReflectedRay(nearest, ray.direction);
                png::rgb_pixel col = getColor(reflected_ray, rec+1);
                color = blend(col, color, alpha);
            }
        }
        if (!figure->isRefractor()) {
            return getLightness(nearest, color, figure, ray);
        }
        return color;

    }
    return color;
}

void Projector::addFigure(std::shared_ptr<Figure> fig) {
    figures.push_back(fig);
}

void Projector::addLight(std::shared_ptr<LightSource> light) {
    lights.push_back(light);
}

png::rgb_pixel Projector::getLightness(Point point, png::rgb_pixel col, std::shared_ptr<Figure> figure, Ray ray) const {
    if (point == UNDEF_POINT) {
        return col;
    }
    long double intensity = default_intens;
    for (size_t i = 0; i < lights.size(); ++i) {
        Point light = lights[i]->coordinate;
        Point direct = point - light;
        long double delta_intens = 0;
        Ray ray_from_light(light, direct);
        Point nearest(UNDEF_POINT);
        for (size_t j = 0; j < figures.size(); ++j) {
            if ((light - figures[j]->intersection(ray_from_light)).length() < (light - nearest).length()) {
                nearest = figures[j]->intersection(ray_from_light);
            }
        }
        if (point == nearest) {
            Point normal = figure->getNormal(point);
            Point to_ray_start = ray.base - point;
            if (to_ray_start * normal < 0) {
                normal = normal * (-1);
            }
            Point toLight = light - point;
            long double cos = toLight.normalize() * normal;
            if (cos < 0) {
                cos = 0;
            }
            delta_intens = lights[i]->intensity * cos / ((toLight.length()) * (toLight.length()));
        }
        intensity += delta_intens;
    }
    return png::rgb_pixel(std::fminl(col.red * intensity, 255), std::fminl(col.green * intensity, 255), std::fminl(col.blue * intensity, 255));
}

bool Projector::definePixel(int i, int j) {
    Point direct = screen.getBase() + screen.getFirstVec() * ((long double)(i - (int)image.get_height() / 2) / scale)
                    + screen.getSecondVec() * ((long double)(j - (int)image.get_width() / 2) / scale);
    Ray tracer = Ray(point_of_view, Point(direct - point_of_view));
    image[i][j] = getColor(tracer, 0);
    return true;
}

void Projector::project() {
    assert(!(point_of_view == UNDEF_POINT));
    for (int i = 0; i < (int)image.get_height(); ++i) {
        for (int j = 0; j < (int)image.get_width(); ++j) {
            /*if (i == image.get_height() / 2 + 2 && j == image.get_width() / 2 + 2) {
                std::cout << "lol" << std::endl;
            }*/
            manager.submit(std::bind(&Projector::definePixel, this, i, j));
        }
    }
    manager.shutdown();
}

void Projector::setObserver(long double x, long double y, long double z) {
    point_of_view = Point(x, y, z);
}

Point readCoords(std::ifstream &in){
    long double x, y, z;
    in >> x >> y >> z;
    return Point(x, y, z);
}

void Projector::read(std::string s) {
    std::ifstream in;
    in.open(s);
    std::string command;
    in >> command;
    while (command != "END") {
        std::cout << command << std::endl;
        if (command[0] == '#') {
            continue;
        }
        if (command == "viewport") {
            in >> command;
            Point base, v1, v2;
            while (command != "endviewport") {
                if (command == "origin") {
                    point_of_view = readCoords(in);
                }
                if (command == "topleft") {
                    base = readCoords(in);
                }
                if (command == "bottomleft") {
                    v1 = readCoords(in);
                    v1 = v1 - base;
                }
                if (command == "topright") {
                    v2 = readCoords(in);
                    v2 = v2 - base;
                }
                in >> command;
            }
            screen = Plane(base, v1, v2);
        }
        if (command == "lights") {
            in >> command;
            while (command != "endlights") {
                if (command == "reference") {
                    long double p, d;
                    in >> command;
                    while (command != "endreference"){
                        if (command == "power") {
                            in >> p;
                        }
                        if (command == "distance") {
                            in >> d;
                        }
                        in >> command;
                    }
                    default_intens = p / d / d;
                }
                if (command == "point") {
                    long double p;
                    Point base;
                    in >> command;
                    while (command != "endpoint") {
                        if (command == "coords") {
                            base = readCoords(in);
                        }
                        if (command == "power") {
                            in >> p;
                        }
                        in >> command;
                    }
                    std::shared_ptr<LightSource> l(new LightSource(base, p));
                    this->addLight(l);
                }
                in >> command;
            }
        }
        if (command == "materials") {
            in >> command;
            while (command != "endmaterials") {
                if (command == "entry") {
                    std::string s;
                    png::rgb_pixel col;
                    long double a, refl, refr;
                    in >> command;
                    while (command != "endentry") {
                        if (command == "name") {
                            in >> s;
                        }
                        if (command == "color") {
                            int r, g, b;
                            in >> r >> g >> b;
                            col = png::rgb_pixel(r, g ,b);
                        }
                        if (command == "alpha") {
                            in >> a;
                        }
                        if (command == "reflect") {
                            in >> refl;
                        }
                        if (command == "refract") {
                            in >> refr;
                        }
                        in >> command;
                    }
                    material m(s, col, a, refl, refr);
                    materials[m.name] = m;
                }
                in >> command;
            }
        }
        if (command == "geometry") {
            in >> command;
            while (command != "endgeometry") {
                if (command == "sphere") {
                    long double r;
                    Point center;
                    png::rgb_pixel col;
                    long double a, refl, refr;
                    in >> command;
                    while (command != "endsphere") {
                        std::cout << command << std::endl;
                        if (command == "coords") {
                            center = readCoords(in);
                        }
                        if (command == "radius") {
                            in >> r;
                        }
                        if (command == "material") {
                            std::string s;
                            in >> s;
                            material m = materials[s];
                            col = m.color;
                            a = m.alpha;
                            refl = m.reflect;
                            refr = m.refract;
                        }
                        in >> command;
                    }
                    std::shared_ptr<Figure> sph(new Sphere(center, r, col));
                    if (refl > 0) {
                        sph->setReflection(refl);
                    }
                    if (refr > 0) {
                        sph->setRefraction(refr);
                    }
                    this->addFigure(sph);
                }
                if (command == "triangle") {
                    Point vert[3];
                    png::rgb_pixel col;
                    long double a, refl;
                    in >> command;
                    while (command != "endtriangle") {
                        for (size_t i = 0; i < 3; ++i){
                            if (command == "vertex") {
                                vert[i] = readCoords(in);
                            }
                            in >> command;
                        }
                        if (command == "material") {
                            std::string s;
                            in >> s;
                            material m = materials[s];
                            col = m.color;
                            a = m.alpha;
                            refl = m.reflect;
                        }
                        std::shared_ptr<Figure> triang(new Triangle(vert[0], vert[1], vert[2], col));
                        if (refl > 0) {
                            triang->setReflection(refl);
                        }
                        this->addFigure(triang);
                        in >> command;
                    }
                }
                if (command == "quadrangle") {
                    Point vert[4];
                    png::rgb_pixel col;
                    long double a, refl;
                    in >> command;
                    while (command != "endquadrangle") {
                        for (size_t i = 0; i < 3; ++i){
                            if (command == "vertex") {
                                vert[i] = readCoords(in);
                            }
                        }
                        if (command == "material") {
                            std::string s;
                            in >> s;
                            material m = materials[s];
                            col = m.color;
                            a = m.alpha;
                            refl = m.reflect;
                        }
                        std::shared_ptr<Figure> quad(new Quadrilateral(vert[0], vert[1], vert[2], vert[3], col));
                        if (refl > 0) {
                            quad->setReflection(refl);
                        }
                        this->addFigure(quad);
                        in >> command;
                    }
                }
                in >> command;
            }
        }
        in >> command;
    }
}




void Projector::antiAliasing() {
    for (size_t i = 1; i + 1 < image.get_height(); ++i){
        for (size_t j = 1; j + 1 < image.get_width(); ++j) {
            image[i][j] = blend(image[i][j], image[i - 1][j], 0.7);
            image[i][j] = blend(image[i][j], image[i][j - 1], 0.7);
            image[i][j] = blend(image[i][j], image[i + 1][j], 0.7);
            image[i][j] = blend(image[i][j], image[i][j + 1], 0.7);
            image[i][j] = blend(image[i][j], image[i - 1][j - 1], 0.8);
            image[i][j] = blend(image[i][j], image[i - 1][j + 1], 0.8);
            image[i][j] = blend(image[i][j], image[i + 1][j + 1], 0.8);
            image[i][j] = blend(image[i][j], image[i + 1][j + 1], 0.8);
        }
    }
}

void Projector::show() {
    std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::system_clock::now();
    this->project();
    //this->antiAliasing();
    image.write("/home/penguinka/ray/result.png");
    std::chrono::time_point<std::chrono::system_clock> end_time = std::chrono::system_clock::now();
    std::cout << "Processing time " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds" << std::endl;
}
