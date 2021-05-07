#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtDebug>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPoint>
#include <QLine>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>
#include <algorithm>
#include <QGridLayout>
#include <QLabel>
#include <complex.h>
#include <cmath>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

struct coord{
    double lgr, rgr, bgr, tgr;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void re_draw();
    void keyReleaseEvent(QKeyEvent *event);
    QColor smooth_color(double n, Complex num);
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    bool is_pushed = false;
    int ITERATIONS = 255;
    int cur_it = -1;
    double lgr = -2;
    double rgr = 1;
    double tgr = 1;
    double bgr = -1;
    const QSize render_size = {1920, 1080};
    QPixmap* buff = nullptr;
    QPixmap* pix = nullptr;
    bool is_swaped = false;
    bool selected = true;
    QPoint begin = {-1, -1};
    QPoint cur;
    double left = 0, right = 100, top = 0, bottom = 100;
    Ui::Widget *ui;
    QLabel* label;
    QGridLayout* box;
    QVector<QColor> palette;
    QVector<QPixmap*> opers;
    QVector<coord> coords;

};
#endif // WIDGET_H
