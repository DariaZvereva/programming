#ifndef NETVISION_H
#define NETVISION_H
#include <vector>
#include <sys/types.h>
#include "network.h"


const double FRICTION_COEF = 0.3;
const double STEP_TIME = 0.16;
const double COULOMB_COEF = 840;
const double VERTEX_CHARGE = 40;
const double SPRING = 0.15;
const double LENGTH_OF_SPRING = 700;
const double EPSILON = 0.07;
static const int VERTEX_RADIUS = 13;

struct VertexVision {
    //кординаты на экране
    int xCenter;
    int yCenter;
    //физические координаты
    double xPhysical;
    double yPhysical;
    int radius;
    size_t number;
    double xVelocity;
    double yVelocity;
    bool isSource;
    bool isSink;

    VertexVision(size_t i);
};

//класс визуального графа для алгоритма проталкивания предпотока
class NetVision
{
private:
    //количество вершин
    size_t numberOfVertexes;
    // исток
    size_t source;
    // сток
    size_t sink;
    //"физические" координаты вершин графа
    double minPhysicalX;
    double maxPhysicalX;
    double minPhysicalY;
    double maxPhysicalY;
    //координаты вершин графа на экране
    int minScreenX;
    int maxScreenX;
    int minScreenY;
    int maxScreenY;
    // максимальное значение модуля вектора скорости
    double maxVelocityDelta;
    // максимальное значение модуля перемещения
    double maxDisplacement;
    //информация о вершинах
    std::vector <VertexVision> vertexes;
    // информация о ребрах
    std::vector <std::vector<NetworkEdge> > edges;

public:


    // конструктор из сети, принимает размеры экрана в экранных координатах
    NetVision(Network* Net, int width, int height);

    NetVision(): numberOfVertexes(0), source(0), sink(0){}


    // функция, пересчитывающая граничные физические координаты
    void updateCoordinates();
    // функция, превращающая новые "физические" координаты в экранные
    void updateScreenCoordinates();
    // функция, моделирующая действие сил за промежуток времени
    void doStep();
    // функция, определяющая планаризован граф или нет
    bool isPlanarized();

    void setMaxHeight(size_t h);

    size_t getMaxHeight() const;
    size_t getNumberOfVertexes() const;
    size_t getNumberOfEdges(size_t i) const;

    NetworkEdge getEdge(size_t i, size_t j) const;
    VertexVision getVertex(size_t i) const;

};

#endif // NETVISION_H
