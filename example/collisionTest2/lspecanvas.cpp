#include "lspecanvas.h"
#include "ui_lspecanvas.h"

#include <array>

LspeCanvas::LspeCanvas(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LspeCanvas)
    , man(nullptr)
    , shouldDrawBBox(false)
    , initialized(false)
    , ondrag(false)
    , selection(nullptr)
    , shouldBack(false)
    , enableResponse(false) {
    ui->setupUi(this);

    man = setup();
    LSPE_ASSERT(man != nullptr);

    tmRender = new QTimer(this);
    LSPE_ASSERT(tmRender != nullptr);

    connect(tmRender, &QTimer::timeout, this, &LspeCanvas::render);
}

LspeCanvas::~LspeCanvas() {
    LSPE_ASSERT(tmRender != nullptr);
    LSPE_ASSERT(man != nullptr);

    tmRender->stop();
    delete tmRender;
    tmRender = nullptr;

    delete man;
    man = nullptr;

    delete ui;
}

void LspeCanvas::setInterval(int interval) {
    LSPE_ASSERT(tmRender != nullptr);
    tmRender->setInterval(interval);
}

void LspeCanvas::start() {
    LSPE_ASSERT(tmRender != nullptr);
    tmRender->start();
    initialized = true;
}

void LspeCanvas::render() {
    man->stepforward();
    // qDebug() << "Finish stepforward()";
    update();
}

void LspeCanvas::updateShouldDrawBBox(int status) {
    shouldDrawBBox = status;
}

void LspeCanvas::updateShouldRespondCollision(int status) {
    if (status != 0) {
        QMessageBox::information(
            this,
            "LSP-Engine Demo",
            "这狗屎东西多多少少有点毛病，为了您的身心健康，我建议您再把它关上"
            "！");
    }
    enableResponse = status;
}

void LspeCanvas::paintEvent(QPaintEvent *event) {
    if (initialized) {
        // qDebug() << "Begin rendering";

        QPainter tmp(this);
        painter = &tmp;

        painter->setPen(Qt::blue);
        painter->setBrush(Qt::NoBrush);

        if (shouldDrawBBox) {
            man->traverse(
                (lspe::abt::fnvisit)visit, this, lspe::abt::POSTORDER);
        }
        // qDebug() << "Finish traverse(fnvisit, void*, int)";

        painter->setRenderHint(QPainter::Antialiasing);

        painter->setBrush(QColor(255, 0, 0, 200));

        for (auto obj : man->getObjects()) {
            if (obj->index == -1) continue;
            drawObject(obj);
            query(obj);
        }

        painter->setPen(QPen(Qt::black, 8));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(0, 0, width(), height());
        painter = nullptr;

        // qDebug() << "Finish rendering";
    }

    QWidget::paintEvent(event);
}

void LspeCanvas::drawObject(Object *obj) {
    using namespace lspe::shape;
    switch (obj->type) {
        case LINE: {
            auto e = (Line *)obj->shape.data;
            painter->save();
            painter->setPen(QPen(Qt::red, 2));
            painter->drawLine(e->pa.x, e->pa.y, e->pb.x, e->pb.y);
            painter->setPen(Qt::black);
            auto center = lspe::centroidOf(*e);
            painter->drawText(center.x, center.y, QString::number(obj->index));
            painter->restore();
        } break;
        case CIRCLE: {
            auto e = (Circle *)obj->shape.data;

            // qDebug() << "draw Circle [" << obj->shape << "]: center=("
            // 	<< e->center.x << "," << e->center.y << ") r =" << e->r;

            painter->drawEllipse(
                e->center.x - e->r, e->center.y - e->r, e->r * 2, e->r * 2);
            painter->save();
            painter->setPen(Qt::black);
            painter->drawText(
                e->center.x, e->center.y, QString::number(obj->index));
            painter->restore();
        } break;
        case POLYGEN: {
            auto e = (Polygen *)obj->shape.data;
            painter->drawRect(
                e->vertices[0].x,
                e->vertices[0].y,
                (e->vertices[2] - e->vertices[0]).x,
                (e->vertices[2] - e->vertices[0]).y);
            // painter->drawPolygon(
            // 	(QPointF*)&e->vertices[0], e->vertices.size());
            painter->save();
            painter->setPen(Qt::black);
            painter->drawText(
                e->center.x, e->center.y, QString::number(obj->index));
            painter->restore();
        } break;
        case ELLIPSE: {
            auto e = (Ellipse *)obj->shape.data;

            // qDebug() << "draw Ellipse [" << obj->shape << "]: center=("
            // 	<< e->center.x << "," << e->center.y << ") r=("
            // 	<< e->rx << "," << e->ry << ") rotation ="
            // 	<< e->rotation / lspe::Pi * 180 << "degree";

            painter->translate(e->center.x, e->center.y);
            painter->rotate(e->rotation / lspe::Pi * 180);
            painter->translate(-e->center.x, -e->center.y);
            painter->drawEllipse(
                e->center.x - e->rx, e->center.y - e->ry, e->rx * 2, e->ry * 2);
            painter->resetTransform();
            painter->save();
            painter->setPen(Qt::black);
            painter->drawText(
                e->center.x, e->center.y, QString::number(obj->index));
            painter->restore();
        } break;
        default:
            LSPE_ASSERT(false);
    }
}

bool querySelection(const lspe::abt::node *node, void *extra) {
    using namespace lspe::shape;

    struct QueryExtra {
        Object    **_selection;
        bool       *_ondrag;
        lspe::vec2 *_point;
    };

    auto qe  = (QueryExtra *)extra;
    auto obj = (Object *)(node->userdata);

    LSPE_DEBUG("Execute querySelection; target index=%d;", obj->index);

    bool hit = false;
    switch (obj->type) {
        case LINE:
            hit = false;
            break;
        case CIRCLE:
            hit = lspe::contain(*(Circle *)(obj->shape.data), *qe->_point);
            break;
        case POLYGEN:
            hit = lspe::contain(*(Polygen *)(obj->shape.data), *qe->_point);
            break;
        case ELLIPSE:
            hit = lspe::contain(*(Ellipse *)(obj->shape.data), *qe->_point);
            break;
        default:
            LSPE_ASSERT(false);
    }

    static const char *ShapeName[] = {"Circle", "Polygen", "Ellipse"};

    if (hit) {
        LSPE_DEBUG(
            "Execute querySelection: %s[%d] has been selected",
            ShapeName[obj->type - CIRCLE],
            obj->index);

        *qe->_selection = obj;
        *qe->_ondrag    = true;
        return false;
    }

    LSPE_DEBUG("Execute querySelection: no selection");

    return true;
}

void LspeCanvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        LSPE_ASSERT(!ondrag);

        lspe::vec2 point(event->x(), event->y());
        LSPE_DEBUG("Entry mousePressEvent: Point(%f,%f)", point.x, point.y);

        struct QueryExtra {
            Object    **_selection;
            bool       *_ondrag;
            lspe::vec2 *_point;
        } extra;

        extra._selection = &selection;
        extra._ondrag    = &ondrag;
        extra._point     = &point;

        man->query(querySelection, point, &extra);

        if (ondrag) { precoord = point; }
    }
}

void LspeCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (ondrag) {
        LSPE_ASSERT(selection != nullptr);

        lspe::vec2 point(event->x(), event->y());

        auto displacement = point - precoord;
        man->moveObject(selection->index, selection->box, displacement);

        if (enableResponse) {
            query(selection);
            if (shouldBack) {
                man->moveObject(
                    selection->index, selection->box, -displacement);
                shouldBack = false;
                LSPE_DEBUG("Drag Object: rejected movement");
            } else {
                man->translate(selection->shape, selection->type, displacement);
                precoord = point;
            }
        } else {
            man->translate(selection->shape, selection->type, displacement);
            precoord = point;
        }

        update();
    }
}

void LspeCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (!ondrag) return;

        LSPE_ASSERT(selection != nullptr);

        lspe::vec2 point(event->x(), event->y());
        LSPE_DEBUG("Entry mousePressEvent: Point(%f,%f)", point.x, point.y);

        selection = nullptr;
        ondrag    = false;
    }
}

void LspeCanvas::query(Object *obj) {
    struct QueryExtra {
        Object *_object;
        void   *_extra;
        void   *_userdata;
        void   *_this;
    } extra;

    void *_extra[2] = {painter, &collider};

    extra._object   = obj;
    extra._extra    = _extra;
    extra._userdata = nullptr;
    extra._this     = this;

    man->query(LspeCanvas::_query, obj->box, &extra);
}

void configCollider(
    lspe::Collider &collider, const Object &a, const Object &b) {
    using namespace lspe::shape;

    collider.reset();

    static lspe::collision::fnsupport supports[4] = {
        lspe::collision::supportLine,
        lspe::collision::supportCircle,
        lspe::collision::supportPolygen,
        lspe::collision::supportEllipse,
    };

    lspe::vec2 c1, c2;

    switch (a.type) {
        case LINE:
            c1 = lspe::centroidOf(*(Line *)(a.shape.data));
            break;
        case CIRCLE:
            c1 = lspe::centroidOf(*(Circle *)(a.shape.data));
            break;
        case POLYGEN:
            c1 = lspe::centroidOf(*(Polygen *)(a.shape.data));
            break;
        case ELLIPSE:
            c1 = lspe::centroidOf(*(Ellipse *)(a.shape.data));
            break;
        default:
            LSPE_ASSERT(false);
    }

    switch (b.type) {
        case LINE:
            c2 = lspe::centroidOf(*(Line *)(&b.shape));
            break;
        case CIRCLE:
            c2 = lspe::centroidOf(*(Circle *)(&b.shape));
            break;
        case POLYGEN:
            c2 = lspe::centroidOf(*(Polygen *)(&b.shape));
            break;
        case ELLIPSE:
            c2 = lspe::centroidOf(*(Ellipse *)(&b.shape));
            break;
        default:
            LSPE_ASSERT(false);
    }

    lspe::vec2 *firstdirection = new lspe::vec2(c1 - c2);

    collider.setTestPair(a.shape, b.shape);
    collider.bindSupports(supports[a.type], supports[b.type]);
    collider.bindInitialGenerator(
        [](lspe::Shape, lspe::Shape, const lspe::vec2 &, void *extra)
            -> lspe::vec2 {
            return *(lspe::vec2 *)extra;
        });
    collider.bindExtraData(firstdirection);
}

bool LspeCanvas::hasCollisionResponse() const {
    return enableResponse;
}

void LspeCanvas::cancelPreStep() {
    shouldBack = true;
}

bool LspeCanvas::_query(const lspe::abt::node *node, void *extra) {
    struct QueryExtra {
        Object *_object;
        void   *_extra;
        void   *_userdata;
        void   *_this;
    };

    auto qe       = (QueryExtra *)extra;
    auto painter  = (QPainter *)((void **)(qe->_extra))[0];
    auto collider = (lspe::Collider *)((void **)(qe->_extra))[1];

    auto p = qe->_object;
    auto q = (Object *)(node->userdata);

    configCollider(*collider, *p, *q);

    std::array<const char *, 5> stype{"Line", "Circle", "Polygen", "Ellipse"};

    if (p->index != q->index && collider->collided()) {
        lspe::Arbiter arbiter(collider, 64);
        arbiter.setEps(0.1f);

        LSPE_ASSERT(arbiter.isActive());
        arbiter.perform();

        if (arbiter.isCollided()) {
            qDebug() << "Collision Test Results:" << stype[p->type] << "x"
                     << stype[q->type] << "[" << p->index << ":" << q->index
                     << "]";

            lspe::vec2 a, b;
            arbiter.getClosetPoint(&a, &b);

            painter->save();
            painter->setPen(QPen(Qt::cyan, 2));
            painter->drawLine(a.x, a.y, b.x, b.y);
            painter->restore();

            auto lc = (LspeCanvas *)(qe->_this);
            if (lc->hasCollisionResponse()) { lc->cancelPreStep(); }
        }
    }

    delete (lspe::vec2 *)collider->getExtraData();

    return true;
}

bool LspeCanvas::visit(lspe::abt::node *node, void *extra) {
    auto       p = (LspeCanvas *)extra;
    lspe::vec2 l = node->box.lower;
    lspe::vec2 s = node->box.upper - l;
    p->painter->drawRect(l.x, l.y, s.x, s.y);
    return true;
}

lspeman *LspeCanvas::setup() {
    auto man = new lspeman;

    man->setBBoxExtension(4.0f);

    // man->newLine();

    man->newPolygen();
    man->newPolygen();
    man->newPolygen();
    man->newPolygen();

    man->newCircle();
    man->newCircle();

    man->newEllipse();
    man->newEllipse();

    man->setStep(0.08f);

    return man;
}
