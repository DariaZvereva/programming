#include "netvision.h"
#include "network.h"
#include <numeric>
#include <algorithm>
#include <math.h>
#include <iostream>

NetVision::NetVision(Network* Net, int width, int height){
    numberOfVertexes = Net->getNumberOfVertexes();
    maxScreenX = width;
    maxScreenY = height;
    minScreenX = 50;
    minScreenY = 50;
    maxDisplacement = std::numeric_limits<double>::max();
    maxVelocityDelta = std::numeric_limits<double>::max();
    for (size_t i = 0; i < numberOfVertexes; ++i){
        VertexVision v(i);
        if (i == Net->getSource()) {
            v.isSource = true;
        }
        if (i == Net->getSink()) {
            v.isSink = true;
        }
        // располагаем вершины рандомно
        v.xCenter = rand() % maxScreenX + 50;
        v.yCenter = rand() % maxScreenY + 50;
        v.xPhysical = v.xCenter;
        v.yPhysical = v.yCenter;
        vertexes.push_back(v);
    }
    edges.resize(numberOfVertexes);
    for (size_t i = 0; i < numberOfVertexes; ++i){
        for (size_t j = 0; j < numberOfVertexes; ++j){
            if (Net->getCapacity(i, j) > 0){
                edges[i].push_back(NetworkEdge(i, j, Net->getCapacity(i, j), Net->getFlow(i, j)));
            }
        }
    }
}


void NetVision::doStep(){
    maxVelocityDelta = 0;
    maxDisplacement = 0;
    std::vector<VertexVision> tempVertexes;
    for (size_t i = 0; i < numberOfVertexes; ++i){
        double newPhysicalX;
        double newXVelocity = vertexes[i].xVelocity * FRICTION_COEF;
        newPhysicalX = vertexes[i].xPhysical + newXVelocity * STEP_TIME;
        double newPhysicalY;
        double newYVelocity = vertexes[i].yVelocity * FRICTION_COEF;
        newPhysicalY = vertexes[i].yPhysical + newYVelocity * STEP_TIME;
        //пересчет действия силы Кулона
        for (size_t j = 0; j < numberOfVertexes; ++j) {
            if (i == j) continue;
            double distanceX = vertexes[i].xPhysical - vertexes[j].xPhysical;
            double distanceY = vertexes[i].yPhysical - vertexes[j].yPhysical;
            double distance = sqrt(distanceX * distanceX + distanceY * distanceY);
            newXVelocity += COULOMB_COEF * VERTEX_CHARGE * VERTEX_CHARGE / (distance * distance) * distanceX / distance * STEP_TIME;
            newYVelocity += COULOMB_COEF * VERTEX_CHARGE * VERTEX_CHARGE / (distance * distance) * distanceY / distance * STEP_TIME;
        }
        //пересчет действия силы жесткости пружины
        for (size_t j = 0; j < edges[i].size(); ++j) {
            double distanceX = vertexes[i].xPhysical - vertexes[edges[i][j].to].xPhysical;
            double distanceY = vertexes[i].yPhysical - vertexes[edges[i][j].to].yPhysical;
            double distance = sqrt(distanceX * distanceX + distanceY * distanceY);
            newXVelocity += SPRING * (LENGTH_OF_SPRING - distance) * distanceX / distance * STEP_TIME;
            newYVelocity += SPRING * (LENGTH_OF_SPRING - distance) * distanceY / distance * STEP_TIME;
        }
        //сохраняем максимальное изменение местоположения вершин и их векторов скорости
        maxDisplacement = std::max(maxDisplacement, std::abs(newPhysicalX - vertexes[i].xPhysical));
        maxDisplacement = std::max(maxDisplacement, std::abs(newPhysicalY - vertexes[i].yPhysical));
        maxVelocityDelta = std::max(maxVelocityDelta, std::abs(newXVelocity - vertexes[i].xVelocity));
        maxVelocityDelta = std::max(maxVelocityDelta, std::abs(newYVelocity - vertexes[i].yVelocity));
        VertexVision v(i);
        v.xPhysical = newPhysicalX;
        v.yPhysical = newPhysicalY;
        v.xVelocity = newXVelocity;
        v.yVelocity = newYVelocity;
        tempVertexes.push_back(v);
    }
    // обновляем координаты вершин
    for (size_t i = 0; i < numberOfVertexes; ++i){
        vertexes[i].xPhysical = tempVertexes[i].xPhysical;
        vertexes[i].yPhysical = tempVertexes[i].yPhysical;
        vertexes[i].xVelocity = tempVertexes[i].xVelocity;
        vertexes[i].yVelocity = tempVertexes[i].yVelocity;
    }
    //std::cout << maxDisplacement << " " << maxVelocityDelta << std::endl;
    updateCoordinates();
}

void NetVision::updateScreenCoordinates(){
    double realWidth = maxPhysicalX - minPhysicalX;
    double realHeight = maxPhysicalY - minPhysicalY;
    int screenWidth = maxScreenX - minScreenX;
    int screenHeight = maxScreenY - minScreenY;
    for (size_t i = 0; i < numberOfVertexes; ++i) {
        vertexes[i].xCenter = static_cast<int>(round((vertexes[i].xPhysical - minPhysicalX)/realWidth * screenWidth)) + 50;
        vertexes[i].yCenter = static_cast<int>(round((vertexes[i].yPhysical - minPhysicalY)/realHeight * screenHeight)) + 50;
    }
}

void NetVision::updateCoordinates(){
    double tempMinX = std::numeric_limits<double>::max();
    double tempMaxX = std::numeric_limits<double>::min();
    double tempMinY = std::numeric_limits<double>::max();
    double tempMaxY = std::numeric_limits<double>::min();
    for (size_t i = 0; i < numberOfVertexes; ++i){
        tempMaxX = std::max(tempMaxX, vertexes[i].xPhysical);
        tempMaxY = std::max(tempMaxY, vertexes[i].yPhysical);
        tempMinX = std::min(tempMinX, vertexes[i].xPhysical);
        tempMinY = std::min(tempMinY, vertexes[i].yPhysical);
    }
    minPhysicalX = tempMinX;
    maxPhysicalX = tempMaxX;
    minPhysicalY = tempMinY;
    maxPhysicalY = tempMaxY;
    updateScreenCoordinates();
}

bool NetVision::isPlanarized(){
    if (maxVelocityDelta < EPSILON && maxDisplacement < EPSILON){
        updateCoordinates();
        return true;
    }
    return false;
}


size_t NetVision::getNumberOfVertexes() const{
    return numberOfVertexes;
}

VertexVision NetVision::getVertex(size_t i) const{
    return vertexes[i];
}


NetworkEdge NetVision::getEdge(size_t i, size_t j) const{
    return edges[i][j];
}

size_t NetVision::getNumberOfEdges(size_t i) const {
    return edges[i].size();
}

VertexVision::VertexVision(size_t i){
    number = i;
    radius = VERTEX_RADIUS;
    xCenter = 0;
    yCenter = 0;
    xPhysical = 0;
    yPhysical = 0;
    xVelocity = 0;
    yVelocity = 0;
    isSink = false;
    isSource = false;
}
