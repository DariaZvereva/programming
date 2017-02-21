#include "mainwindow.h"

static const double ARROW_LEN = 0.8;
static const double K_SPRING = 0.5;
static const int WIDTH = 1800;
static const int HEIGHT = 900;

MainWindow::MainWindow(Network &net, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    netInitial = &net;
    ui->setupUi(this);
    resize(WIDTH, HEIGHT);
    algorythm = MaxFlow(net, 0);
    netVision = NetVision(netInitial, 2 * this->width() / 3 - 100, this->height() - 50);
    connect(this, SIGNAL(planarized()), this, SLOT(visualisation()));
    update();
    planarisation = false;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    switch (event->key()){
        case Qt::Key_Space:
            if (!planarisation && !netVision.isPlanarized()){
                planarisation = true;
                planarizeNet();
                return;
            }
            planarisation = false;
            timer->stop();
            emit planarized();
        break;
        case Qt::Key_Left:
            if (algorythm.getStepCounter() > 0) {
                size_t counter = algorythm.getStepCounter();
                counter--;
                algorythm = MaxFlow(*netInitial, counter);
                update();
            }
        break;
        case Qt::Key_Right:
            size_t counter = algorythm.getStepCounter();
            counter++;
            algorythm = MaxFlow(*netInitial, counter);
            update();
        break;
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    showBorder(painter);
    if (!planarisation) {
        showAlgoAction(painter);
    }
    showEdges(painter);
    showVertexes(painter);
    showLines(painter);
}


void MainWindow::planarizeNet(){
    timer = std::shared_ptr<QTimer>(new QTimer(this));
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(planarizationStep()));
    timer->start(1);
}

void MainWindow::planarizationStep(){
    for (int i = 0; i < 10; ++i) netVision.doStep();
    update();
    if (netVision.isPlanarized()){
        timer->stop();
        planarisation = false;
        emit planarized();
    }
}

void MainWindow::showVertexes(QPainter &painter){
    for (size_t i = 0; i < netVision.getNumberOfVertexes(); ++i) {
        drawVertex(painter, netVision.getVertex(i));
    }
}

void MainWindow::showEdges(QPainter &painter){
    for (size_t i = 0; i < netVision.getNumberOfVertexes(); ++i){
        for (size_t j = 0; j < netVision.getNumberOfEdges(i); ++j){
            drawEdge(painter, netVision.getEdge(i, j));
        }
    }
}

void MainWindow::showAlgoAction(QPainter &painter) {
    long long flow = algorythm.getMaxFlowValue();
    showAlgoVertexes(painter);
}

//рисует процесс работы алгоритма
void MainWindow::showAlgoVertexes(QPainter &painter) {
    for (size_t i = 0; i < algorythm.getNumberOfVertexes(); ++i){
        painter.setPen(Qt::NoPen);
        if (i == algorythm.getNetwork()->getSource()) {
            painter.setBrush(QBrush(QColor(0x00, 0xFF, 0x00)));
        } else {
            if (i == algorythm.getNetwork()->getSink()){
                painter.setBrush(QBrush(QColor(0xFF, 0x00, 0x00)));
            } else {
                painter.setBrush(QBrush(Qt::magenta));
            }
        }
        int radius = (this->width() / 3 - 60) / (2 * algorythm.getNumberOfVertexes());
        int xCenter = 2 * this->width() / 3 + 50 + radius + 3 + i * 2 * radius;
        int yCenter = this->height() - (algorythm.getHeight(i) + 1) * (this->height() / (algorythm.getMaxHeight() + 2) );
        QPoint centerPoint(xCenter, yCenter);
        painter.drawEllipse(centerPoint, radius, radius);
        QPen pen;
        pen.setColor(Qt::black);
        pen.setWidth(1);
        painter.setPen(pen);
        //print text
        QString num;
        if (i != algorythm.getNetwork()->getSource()){
            num = QString(std::to_string(algorythm.getExcess(i)).c_str());
            centerPoint = QPoint(xCenter - 4, yCenter + 5);
        } else {
            num = QString("INF");
            centerPoint = QPoint(xCenter - 12, yCenter + 5);
        }
        painter.drawText(centerPoint, num);
    }
}

//рисует границы областей рабочего пространства
void MainWindow::showBorder(QPainter &painter){
    //рамочка
    QPen pen;
    if (!netVision.isPlanarized()){
        QString message("Для начала и остановки планаризации нажмите ПРОБЕЛ");
        QPoint point = QPoint(23, 35);
        painter.drawText(point, message);
    }
    pen.setWidth(1);
    pen.setColor(QColor(0xC1, 0xC1, 0xC1));
    pen.setStyle(Qt::DashDotDotLine);
    static const QPointF points[4] = {
        QPointF(20, 20),
        QPointF(2 * this->width() / 3 - 40, 20),
        QPointF(2 * this->width() / 3 - 40, this->height() - 20),
        QPointF(20, this->height() - 20)
    };
    QString message("Для перехода на следующий/предыдущий шаг алгоритма, используйте стрелки");
    QPoint point = QPoint(23, this->height() - 30);
    painter.drawText(point, message);
    painter.setPen(pen);
    painter.drawConvexPolygon(points, 4);

}

//линеечки
void MainWindow::showLines(QPainter &painter){
    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(0xC1, 0xC1, 0xC1));
    pen.setStyle(Qt::DashDotDotLine);
    for (size_t i = 0; i <= algorythm.getMaxHeight(); ++i) {
        pen.setColor(QColor(0xC1, 0xC1, 0xC1));
        painter.setPen(pen);
        QPoint from(2 * this->width() / 3 + 50, this->height() - (i + 1) * (this->height() / (algorythm.getMaxHeight() + 2) ));
        QPoint to(this->width() - 10, this->height() - (i + 1) * (this->height() / (algorythm.getMaxHeight() + 2)));
        painter.drawLine(from, to);
        QString num(std::to_string(i).c_str());
        from = QPoint(2 * this->width() / 3 + 43 - std::to_string(i).length() * 7, this->height() - (i + 1) * (this->height() / (algorythm.getMaxHeight() + 2) ));
        pen.setColor(QColor(Qt::black));
        painter.setPen(pen);
        painter.setFont(QFont("Arial", 10));
        painter.drawText(from, num);
    }
}

void MainWindow::visualisation(){
    update();
}

// рисует вершину в графе
void MainWindow::drawVertex(QPainter &painter, VertexVision vertex){
    painter.setPen(Qt::NoPen);
    if (vertex.isSource) {
        painter.setBrush(QBrush(QColor(0x00, 0xFF, 0x00)));
    } else {
        if (vertex.isSink){
            painter.setBrush(QBrush(QColor(0xFF, 0x00, 0x00)));
        } else {
            painter.setBrush(QBrush(Qt::magenta));
        }
    }
    QPoint centerPoint(vertex.xCenter, vertex.yCenter);
    painter.drawEllipse(centerPoint, vertex.radius, vertex.radius);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    //print text
    QString num(std::to_string(vertex.number).c_str());
    centerPoint = QPoint(vertex.xCenter - vertex.radius * 2 / 3 + 1, vertex.yCenter + vertex.radius * 2 / 3);
    painter.setFont(QFont("Arial", 8));
    painter.drawText(centerPoint, num);
}

// рисует ребро
void MainWindow::drawEdge(QPainter &painter, NetworkEdge edge){
    QPen pen;
    pen.setWidth(2);
    if (algorythm.getNetwork()->getFlow(edge.from, edge.to) > 0){
        pen.setColor(QColor(0x44, 0xF3, 0xFF));
    } else {
        pen.setColor(Qt::blue);
    }
    painter.setPen(pen);
    QPoint point_from(netVision.getVertex(edge.from).xCenter, netVision.getVertex(edge.from).yCenter);
    QPoint point_to(netVision.getVertex(edge.to).xCenter, netVision.getVertex(edge.to).yCenter);
    painter.drawLine(point_from, point_to);
    pen.setWidth(5);
    painter.setPen(pen);
    point_from = QPoint(netVision.getVertex(edge.from).xCenter + ARROW_LEN * (netVision.getVertex(edge.to).xCenter - netVision.getVertex(edge.from).xCenter),
                        netVision.getVertex(edge.from).yCenter + ARROW_LEN * (netVision.getVertex(edge.to).yCenter - netVision.getVertex(edge.from).yCenter));
    painter.drawLine(point_from, point_to);
    //print text
    QString text((std::to_string(algorythm.getNetwork()->getFlow(edge.from, edge.to)) + "/" + std::to_string(edge.capacity)).c_str());
    QPoint centerPoint = QPoint(netVision.getVertex(edge.from).xCenter + 0.5 * (netVision.getVertex(edge.to).xCenter - netVision.getVertex(edge.from).xCenter),
                                netVision.getVertex(edge.from).yCenter + 0.5 * (netVision.getVertex(edge.to).yCenter - netVision.getVertex(edge.from).yCenter));
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.setFont(QFont("Arial", 9));
    painter.drawText(centerPoint, text);

}

