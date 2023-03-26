#ifndef LSPECANVAS_H
#define LSPECANVAS_H

#include "stable.h"

namespace Ui {
class LspeCanvas;
} // namespace Ui

class LspeCanvas : public QWidget {
    Q_OBJECT

public:
    explicit LspeCanvas(QWidget *parent = 0);
    ~LspeCanvas();

    void setInterval(int interval);
    void start();

    //! customizable function
    void paintEvent(QPaintEvent *event);

    void updateShouldDrawBBox(int status);

    void drawObject(lspe::RigidBody *obj);

public slots:
    void render();

private:
    Ui::LspeCanvas *ui;

    Solver solver;

    QPainter *painter;
    QTimer   *tmRender;

    bool shouldDrawBBox;
    bool initialized;

    //! customizable function
    static bool visit(const lspe::abt::node *node, void *extra);
    static bool _query(const lspe::abt::node *node, void *extra);
    void        setup();
};

#endif // LSPECANVAS_H
