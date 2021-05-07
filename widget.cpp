#include "widget.h"
#include "ui_widget.h"

double lerp(double v0, double v1, double t){
    return v0 + t * (v1 - v0);
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    label = new QLabel(this);
    label->setMinimumSize(1, 1);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pix = new QPixmap(this->width(), this->height());
    pix->fill(QColor("red"));
    buff = new QPixmap(this->width(), this->height());
    buff->fill(QColor("red"));
    label->setPixmap(*pix);
    qDebug() << opers.size();
    box = new QGridLayout;
    box->setGeometry(this->rect());
    box->addWidget(label);
    box->setContentsMargins(0, 0, 0 ,0);
    box->setSpacing(0);
    this->setLayout(box);
    QColor BUFF;
    palette.resize(ITERATIONS + 1);
    for(int it = 0; it < ITERATIONS + 1; ++it){
        BUFF.setHsv(360 * (static_cast<double>(it)/ITERATIONS), 180, 240);
        palette[it] = BUFF;
    }

    re_draw();
    delete opers[0];
    qDebug() << cur_it << "   " << opers.size() << "inti" ;
    coord temp;
    temp.tgr = tgr;
    temp.bgr = bgr;
    temp.rgr = rgr;
    temp.lgr = lgr;
    coords.push_back(temp);
    opers.pop_back();
    coords.pop_back();
    cur_it--;
    qDebug() << cur_it << "   " << opers.size() << "aainti" ;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::keyReleaseEvent(QKeyEvent *event){
        if (event->key() == Qt::Key_Z && event->modifiers() & Qt::ControlModifier){
                if(opers.size() == 0 || cur_it == 0) return;
                if(opers.size() - 1 == cur_it && !is_pushed){
                    coord temp;
                    temp.tgr = tgr;
                    temp.bgr = bgr;
                    temp.rgr = rgr;
                    temp.lgr = lgr;
                    opers.push_back(pix);
                    coords.push_back(temp);
                    cur_it++;
                    is_pushed = true;
                }
                delete buff;
                cur_it--;
                pix = opers[cur_it];
                buff = new QPixmap(*pix);
                is_swaped = false;
                lgr = coords[cur_it].lgr;
                rgr = coords[cur_it].rgr;
                tgr = coords[cur_it].tgr;
                bgr = coords[cur_it].bgr;
                label->setPixmap(pix->scaled(this->width(),this->height()));
                qDebug() << lgr << rgr << bgr << tgr;
                Widget::update();
        } else if (event->key() == Qt::Key_Y && event->modifiers() & Qt::ControlModifier){
            if(opers.size() == cur_it+1 || opers.size() == 0) return;
            delete buff;
            cur_it++;
            pix = opers[cur_it];
            buff = new QPixmap(*pix);
            is_swaped = false;
            lgr = coords[cur_it].lgr;
            rgr = coords[cur_it].rgr;
            tgr = coords[cur_it].tgr;
            bgr = coords[cur_it].bgr;
            label->setPixmap(pix->scaled(this->width(),this->height()));
            qDebug() << lgr << rgr << bgr << tgr;
            Widget::update();
    }
    qDebug() << cur_it << "   " << opers.size() <<  " iter";
}

void Widget::mouseMoveEvent(QMouseEvent *event) {
    if (cur == event->pos()) return;

    cur = event->pos();
//    qDebug() << cur;
    QWidget::update();
}


void Widget::mousePressEvent(QMouseEvent *event){
        this->setMouseTracking(true);
        begin = event->pos();
        cur = begin;
}

void Widget::mouseReleaseEvent(QMouseEvent *event){
    auto t_begin = begin;
    auto t_cur = cur;
    if(begin.rx() > t_cur.rx()){
        qSwap(t_begin, t_cur);
        if(t_begin.ry() > t_cur.ry()){
            qSwap(t_begin.ry(),t_cur.ry());
        }
    }
//    qDebug() << t_begin << "  ---  " << t_cur;
    double t_lgr = qAbs(rgr - lgr) * ((t_begin.rx()*1.0) / this->width()) + lgr;
    double t_rgr = qAbs(rgr - lgr) * ((t_cur.rx() * 1.0) / this->width()) + lgr;
    double t_tgr = qAbs(tgr - bgr) * (((this->height() - t_begin.ry()) * 1.0) / this->height()) + bgr;
    double t_bgr = qAbs(tgr - bgr) * (((this->height() - t_cur.ry()) * 1.0) / this->height()) + bgr;
    if(qAbs(t_lgr - t_rgr) >= 1e-16 && qAbs(t_bgr - t_tgr) >= 1e-16){
        coord temp;
        temp.tgr = tgr;
        temp.bgr = bgr;
        temp.rgr = rgr;
        temp.lgr = lgr;
        tgr = t_tgr;
        rgr = t_rgr;
        bgr = t_bgr;
        lgr = t_lgr;
        coords.push_back(temp);
    }


//    bgr = qAbs(tgr-bgr) * ((t_begin.rx()*1.0) / this->width())
//    qDebug() << lgr << "  ---  " << t_cur;
    begin = {-1, -1};
    cur = begin;
    this->setMouseTracking(false);
    re_draw();
    Widget::update();
}

QColor Widget::smooth_color(double n, Complex num) {
    double lzn = std::log(num.len_sqr()) / 2;
    double nu = std::log(lzn / std::log(2)) / std::log(2);
    n = n + 1 - nu;
    QColor fc, sc, res;
    fc = palette[int(n)];
    sc = palette[int(n) + 1];
    res.setRgb(lerp(fc.red(), sc.red(), n - (int)n),
            lerp(fc.green(), sc.green(), n - (int)n),
            lerp(fc.blue(), sc.blue(), n - (int)n));
    return res;
}



void Widget::re_draw(){
    double delta_x = qAbs((rgr - lgr))/(1.0 * (double)(render_size.width()));
    double delta_y = qAbs((tgr - bgr))/(1.0 * (double)(render_size.height()));
    qDebug() << lgr << rgr << bgr << tgr;
    QImage* img = new QImage(render_size, QImage::Format_ARGB32_Premultiplied);
    QPainter* painter = new QPainter(img);
    bool greater  = false;
    QPen pen;
    int great = 0;
    for(int x = 0; x < render_size.width(); ++x){
        for(int y = 0; y < render_size.height(); ++y){

            Complex c(lgr + delta_x * (x * 1.0), tgr - delta_y * (y * 1.0));
            Complex pt(0, 0);
            greater = false;
            for(int it = 0; it < ITERATIONS; ++it){
                if(pt.len_sqr() > 4){
                    pen.setColor(smooth_color(it, pt));
                    painter->setPen(pen);
                    painter->drawPoint(x, y);
                    greater = true;
                    break;
                }
                pt = pt * pt + c;
            }
            if(!greater) {

                pen.setColor(QColor(0, 0, 0));
                painter->setPen(pen);
                painter->drawPoint(x, y);
            } else {
                great++;
            }
        }
    }

    qDebug() << cur_it << "  " << opers.size() << " num bef ";
    coord temp;
    temp.tgr = tgr;
    temp.bgr = bgr;
    temp.rgr = rgr;
    temp.lgr = lgr;

    if(cur_it == -1){

    }

    if(cur_it != opers.size()){
        int sz = opers.size();
        qDebug() << sz - cur_it << " delta";
        for(int iter = cur_it; iter < (sz - 1); ++iter){
            qDebug() << opers.size() << "  sizes";
            delete opers[opers.size() - 1];
            opers.pop_back();
            coords.pop_back();
        }

        if(is_swaped){
            qSwap(buff, pix);
        }
        delete buff;
        if(!is_pushed)  {opers.push_back(pix);         cur_it++;}

        is_pushed = false;


    } else {

        if(!is_swaped){
            opers.push_back(pix);
            delete buff;
        } else {
            opers.push_back(buff);
            delete pix;
        }
            cur_it++;
            is_pushed = false;
    }

    qDebug() << cur_it << "  " << opers.size() << " num af ";
    pix = new QPixmap(QPixmap::fromImage(*img));
    delete img;
//    painter->end();
    buff = new QPixmap(*pix);
    is_swaped = false;
    label->setPixmap(pix->scaled(this->width(),this->height()));

}

void Widget::moveEvent(QMoveEvent *event){}
void Widget::resizeEvent(QResizeEvent *event){
//    qDebug() << event->size();
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label->setPixmap(pix->scaled(this->width(),this->height()));
}

void Widget::paintEvent(QPaintEvent *event) {
    QPainter* painter = new QPainter;
    if(begin.rx() != -1 && begin.ry() != -1 && begin == cur){
        if(is_swaped){
            QPixmap* tmp = pix;
            pix = buff;
            buff = tmp;
            delete buff;
        } else {
            delete buff;
        }
        auto t_begin = begin;
        auto t_cur = cur;
        if(begin.rx() > t_cur.rx()){
            qSwap(t_begin, t_cur);
            if(t_begin.ry() > t_cur.ry()){
                qSwap(t_begin.ry(),t_cur.ry());
            }
        }


        if(t_begin.rx() < 0) t_begin.rx() = 0;
        if(t_begin.ry() > label->width()) t_begin.ry() = label->width();
        if(t_cur.rx() < 0) t_cur.rx() = 0;
        if(t_cur.ry() > label->height()) t_cur.ry() = label->height();
        qDebug() << " bef" << t_begin << "   " << t_cur;
        t_begin.rx() = static_cast<double>(render_size.width()) / static_cast<double>(this->width()) * t_begin.rx();
        t_begin.ry() = static_cast<double>(render_size.height()) / static_cast<double>(this->height()) * t_begin.ry();
        t_cur.rx() = static_cast<double>(render_size.width()) / static_cast<double>(this->width()) * t_begin.rx();
        t_cur.ry() = static_cast<double>(render_size.height()) / static_cast<double>(this->height()) * t_cur.ry();
        qDebug() << " af" << t_begin << "   " << t_cur;
        delete painter;
        QImage* img = new QImage(opers[cur_it]->size(), QImage::Format_ARGB32_Premultiplied);
        painter = new QPainter(img);
        painter->drawPixmap(0, 0, *opers[cur_it]);
        painter->setOpacity(0.3);
        painter->fillRect(t_begin.rx(), t_begin.ry(), t_cur.rx()-t_begin.rx(), t_cur.ry()-t_begin.ry(), Qt::blue);
        delete painter;
        buff = new QPixmap(QPixmap::fromImage(*img));
        delete  img;
        QPixmap* tmp = pix;
        pix = buff;
        buff = tmp;
        is_swaped = true;
        label->setPixmap(pix->scaled(this->width(),this->height()));

    } else if(is_swaped){
        QPixmap* tmp = pix;
        pix = buff;
        buff = tmp;
        is_swaped = false;

        label->setPixmap(pix->scaled(this->width(),this->height()));
    }



}

