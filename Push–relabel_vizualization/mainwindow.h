#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "push_relable.h"
#include "network.h"
#include "netvision.h"
#include "ui_mainwindow.h"
#include "push_relable.h"

#include <QKeyEvent>
#include <QMainWindow>
#include <QPen>
#include <QTimer>
#include <QPainter>
#include <QDesktopWidget>
#include <vector>
#include <cstdlib>
#include <string>
#include <memory>

#include <iostream>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Network &net, QWidget *parent = 0);
    ~MainWindow();

protected:
   void paintEvent(QPaintEvent *event);
   void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;

    NetVision netVision;
    Network* netInitial;
    MaxFlow algorythm;
    std::shared_ptr<QTimer> timer;
    bool planarisation;
    void planarizeNet();
    void placeVertexesRandomly();
    void drawVertex(QPainter &painter, VertexVision vertex);
    void drawEdge(QPainter &painter, NetworkEdge edge);
    void drawAlgoVertex(QPainter &painter);
    void showVertexes(QPainter &painter);
    void showAlgoVertexes(QPainter &painter);
    void showEdges(QPainter &painter);
    void showBorder(QPainter &painter);
    void showLines(QPainter &painter);
    void showAlgoAction(QPainter  &painter);
private slots:
    void visualisation();
    void planarizationStep();
signals:
    void planarized();
};

#endif // MAINWINDOW_H
