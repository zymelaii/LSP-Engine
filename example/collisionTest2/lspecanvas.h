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

    bool hasCollisionResponse() const;
    void cancelPreStep();

public slots:
    void render();
    void updateShouldDrawBBox(int status);
    void updateShouldRespondCollision(int status);

private:
    Ui::LspeCanvas *ui;

    QPainter *painter;
    QTimer   *tmRender;

    lspeman *man;
    bool     shouldDrawBBox;
    bool     initialized;

    lspe::Collider collider;

    //! drag part
    bool       ondrag;    //! mark whether has dragged a object
    Object    *selection; //! pointer to the selected object
    lspe::vec2 precoord;  //! object's previous coordination

    bool enableResponse;
    bool shouldBack;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    //! customizable function
    static bool visit(const lspe::abt::node *node, void *extra);
    void        drawObject(Object *obj);
    void        query(Object *obj);
    static bool _query(const lspe::abt::node *node, void *extra);
    lspeman    *setup();
};

#endif // LSPECANVAS_H
