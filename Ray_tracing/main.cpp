#include <QCoreApplication>
#include <iostream>
#include <png++/png.hpp>
#include "projector.h"
#include "sphere.h"
#include "triangle.h"
#include "parallelogram.h"
#include "quadrilateral.h"
#include <memory>

//без света только сфера
void test0 (Projector& me) {
    Point sph_center(3, 0, 5);
    long double rad = 5;
    std::shared_ptr<Figure> sh_p(new Sphere(sph_center, rad, png::rgb_pixel(255, 0, 0)));
    me.addFigure(sh_p);
}
//Зеркало и невидимая сфера
void test1 (Projector& me) {
    Point sph_center(5, 7, 0);
    long double rad = 1;
    std::shared_ptr<Figure> sh_p(new Sphere(sph_center, rad, png::rgb_pixel(255, 0, 0)));
    me.addFigure(sh_p);
    sph_center = Point(5, 7, -5);
    rad = 2;
    std::shared_ptr<Figure> sh_p1(new Sphere(sph_center, rad, png::rgb_pixel(0, 255, 0)));
    me.addFigure(sh_p1);
    /*Point x1(4, -2, 2);
    Point x2(4, 2, 0.2);
    Point x3(2, 2, 1);
    std::shared_ptr<Figure> triang_p1(new Triangle(x1, x2, x3, png::rgb_pixel(0, 0, 255)));
    triang_p1->setReflection(70);
    me.addFigure(triang_p1);*/
    Point x0(-30, -30, 10);
    Point vec1(300, 0, 0);
    Point vec2(0, 300, 0);
    std::shared_ptr<Figure> par_p1(new Parallelogram(x0, vec1, vec2, png::rgb_pixel(255, 0, 100)));
    par_p1->setReflection(70);
    me.addFigure(par_p1);
    Point l(5, 10, -10);
    std::shared_ptr<LightSource> l1_p(new LightSource(l, 100));
    me.addLight(l1_p);
    l = Point(-5, 0, -10);
    std::shared_ptr<LightSource> l2_p(new LightSource(l, 70));
    me.addLight(l2_p);
}
//добавим свет к сфере
void test2 (Projector& me) {
    Point sph_center(3, 0, 5);
    long double rad = 1;
    std::shared_ptr<Figure> sh_p(new Sphere(sph_center, rad, png::rgb_pixel(255, 0, 0)));
    me.addFigure(sh_p);
    Point l = Point(0, -9, 6);
    std::shared_ptr<LightSource> l2_p(new LightSource(l, 70));
    me.addLight(l2_p);
}
//добавим треугольник без зеркала
void test3 (Projector& me) {
    Point sph_center(3, 0, 2);
    long double rad = 1;
    std::shared_ptr<Figure> sh_p(new Sphere(sph_center, rad, png::rgb_pixel(255, 0, 0)));
    me.addFigure(sh_p);
    sph_center = Point(0, 0, 10);
    rad = 2;
    std::shared_ptr<Figure> sh_p1(new Sphere(sph_center, rad, png::rgb_pixel(0, 255, 0)));
    me.addFigure(sh_p1);
    sph_center = Point(0, 0, -10);
    rad = 9;
    std::shared_ptr<Figure> sh_p2(new Sphere(sph_center, rad, png::rgb_pixel(255, 0, 0)));
    sh_p2->setRefraction(1.3);
    me.addFigure(sh_p2);
    Point x1(4, -2, 2);
    Point x2(4, 2, 0.2);
    Point x3(2, 2, 1);
    std::shared_ptr<Figure> triang_p1(new Triangle(x1, x2, x3, png::rgb_pixel(0, 0, 255)));
    me.addFigure(triang_p1);
    Point l(5, 10, -10);
    std::shared_ptr<LightSource> l1_p(new LightSource(l, 100));
    me.addLight(l1_p);
    l = Point(0, -9, 3);
    std::shared_ptr<LightSource> l2_p(new LightSource(l, 70));
    me.addLight(l2_p);
}
//отражающий треугольник и 2 сферы
void test4 (Projector& me) {
    Point sph_center(3, 0, 2);
    long double rad = 1;
    std::shared_ptr<Figure> sh_p(new Sphere(sph_center, rad, png::rgb_pixel(255, 0, 0)));
    me.addFigure(sh_p);
    sph_center = Point(0, 0, 10);
    rad = 2;
    std::shared_ptr<Figure> sh_p1(new Sphere(sph_center, rad, png::rgb_pixel(0, 255, 0)));
    me.addFigure(sh_p1);
    Point x1(4, -2, 2);
    Point x2(4, 2, 0.2);
    Point x3(2, 2, 12);
    std::shared_ptr<Figure> triang_p1(new Triangle(x1, x2, x3, png::rgb_pixel(0, 0, 255)));
    triang_p1->setReflection(70);
    me.addFigure(triang_p1);
    Point l(-5, 10, -10);
    std::shared_ptr<LightSource> l1_p(new LightSource(l, 100));
    me.addLight(l1_p);
    l = Point(0, 9, 3);
    std::shared_ptr<LightSource> l2_p(new LightSource(l, 70));
    me.addLight(l2_p);
}
//просто 2 сферы
void test5(Projector& me) {
    Point sph_center(5, 7, 0);
    long double rad = 2;
    std::shared_ptr<Figure> sh_p(new Sphere(sph_center, rad, png::rgb_pixel(255, 0, 0)));
    me.addFigure(sh_p);
    sph_center = Point(5, 7, -5);
    rad = 1;
    std::shared_ptr<Figure> sh_p1(new Sphere(sph_center, rad, png::rgb_pixel(0, 255, 0)));
    me.addFigure(sh_p1);
    Point l(5, 10, -10);
    std::shared_ptr<LightSource> l1_p(new LightSource(l, 100));
    me.addLight(l1_p);
    l = Point(0, -9, 3);
    std::shared_ptr<LightSource> l2_p(new LightSource(l, 70));
    me.addLight(l2_p);
}
//невыпуклый четырехугольник, отражающий в себе сферу
void test6 (Projector &me) {
    Point v1(0, -20, 0);
    Point v2(20, -20, 0);
    Point v3(5, -20, 5);
    Point v4(0, -20, 20);
    Point sph_center(4, -18, 10);
    long double rad = 2;
    std::shared_ptr<Figure> sh_p(new Sphere(sph_center, rad, png::rgb_pixel(255, 0, 0)));
    me.addFigure(sh_p);
    std::shared_ptr<Figure> p1(new Quadrilateral(v1, v2, v3, v4, png::rgb_pixel(0, 255, 0)));
    p1->setReflection(50);
    me.addFigure(p1);

    Point l(5, 10, -10);
    std::shared_ptr<LightSource> l1_p(new LightSource(l, 100));
    me.addLight(l1_p);
    l = Point(0, -9, 3);
    std::shared_ptr<LightSource> l2_p(new LightSource(l, 70));
    me.addLight(l2_p);
}
//преломяющая сфера
void test7 (Projector& me) {
    Point sph_center(0, 0, 5);
    long double rad = 2;
    std::shared_ptr<Figure> sh_p(new Sphere(sph_center, rad, png::rgb_pixel(255, 0, 0)));
    me.addFigure(sh_p);
    sph_center = Point(0, 0, -8);
    rad = 4;
    std::shared_ptr<Figure> sh_p2(new Sphere(sph_center, rad, png::rgb_pixel(255, 0, 0)));
    sh_p2->setRefraction(1.2);
    me.addFigure(sh_p2);
    Point x0(-30, -30, 10);
    Point vec1(300, 0, 0);
    Point vec2(0, 300, 0);
    std::shared_ptr<Figure> par_p1(new Parallelogram(x0, vec1, vec2, png::rgb_pixel(255, 255, 0)));
    me.addFigure(par_p1);
    Point l = Point(0, -9, -10);
    std::shared_ptr<LightSource> l2_p(new LightSource(l, 70));
    me.addLight(l2_p);
    l = Point(5, 10, -10);
    std::shared_ptr<LightSource> l1_p(new LightSource(l, 100));
    me.addLight(l1_p);
}
//текстура с преломлением
void test8 (Projector& me) {
    Point sph_center(0, 0, 15);
    long double rad = 2;
    std::shared_ptr<Figure> sh_p(new Sphere(sph_center, rad, png::rgb_pixel(255, 0, 100)));
    me.addFigure(sh_p);
    sph_center = Point(5, -10, 55);
    rad = 10;
    std::shared_ptr<Figure> sh_p1(new Sphere(sph_center, rad, png::rgb_pixel(100, 255, 0)));
    me.addFigure(sh_p1);
    sph_center = Point(10, 17, 45);
    rad = 15;
    std::shared_ptr<Figure> sh_p3(new Sphere(sph_center, rad, png::rgb_pixel(0, 255, 0)));
    me.addFigure(sh_p3);
    sph_center = Point(30, 0, 50);
    rad = 30;
    std::shared_ptr<Figure> sh_p2(new Sphere(sph_center, rad, png::rgb_pixel(255, 0, 0)));
    sh_p2->setRefraction(1.2);
    me.addFigure(sh_p2);
    sph_center = Point(30, 0, 50);
    rad = 15;
    std::shared_ptr<Figure> sh_p4(new Sphere(sph_center, rad, png::rgb_pixel(255, 0, 0)));
   // sh_p4->setRefraction(1.3);
    me.addFigure(sh_p4);
    Point x0(30, -200, -10);
    Point vec1(0, 0, 400);
    Point vec2(0, 400, 0);
    std::shared_ptr<Figure> par_p1(new Parallelogram(x0, vec1, vec2, png::rgb_pixel(255, 255, 0)));
    par_p1->setTexture("/home/penguinka/ray/Chess.png", 50);
    me.addFigure(par_p1);
    Point l = Point(0, -9, -10);
    std::shared_ptr<LightSource> l2_p(new LightSource(l, 400));
    me.addLight(l2_p);
}
//текстура на четырёхугольнике и треугольнике
void test9 (Projector& me) {
    Point v1(10, -1, 0);
    Point v2(-1, -1, 0);
    Point v3(-1, 10, 0);
    Point v4(6, 6, 0);
    std::shared_ptr<Figure> p1(new Quadrilateral(v1, v2, v3, v4, png::rgb_pixel(0, 255, 0)));
    p1->setTexture("/home/penguinka/ray/txtr3.png", 8);
    me.addFigure(p1);
    Point x1(7, 8, 2);
    Point x2(7, 12, 0.2);
    Point x3(5, 12, 1);
    std::shared_ptr<Figure> triang_p1(new Triangle(x1, x2, x3, png::rgb_pixel(0, 0, 255)));
    triang_p1->setTexture("/home/penguinka/ray/Chess.png", 5);
    me.addFigure(triang_p1);
    Point l = Point(0, -9, -10);
    std::shared_ptr<LightSource> l2_p(new LightSource(l, 150));
    me.addLight(l2_p);
}

int main()
{
    std::cout << "HI!" << std::endl;
    Point I(0, 0, -50);
    Point monitor_base(0, 0, -40);
    Point v1(3, 0, 0);
    Point v2(0, 3, 0);
    Plane monitor(monitor_base, v1, v2);
    Projector me(I, monitor, 9, 12, 100);
    test9(me);
//    Projector me;
//    me.read("/home/penguinka/ray/test4.txt");
    me.show();
    return 0;
}
