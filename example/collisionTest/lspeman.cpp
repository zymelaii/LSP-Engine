#include "lspeman.h"

using namespace lspe;
using namespace lspe::abt;
using namespace lspe::shape;
using namespace lspe::collision;

lspeman::lspeman()
    : step(1.0f)
    , e(time(0L))
    , u1(0, 600)
    , u2(4, 64)
    , u3(0, 1) {
    tree = new abtree();
    LSPE_ASSERT(tree != nullptr);
}

lspeman::~lspeman() {
    LSPE_ASSERT(tree != nullptr);

    delete tree;
    tree = nullptr;

    for (auto e : objects) {
        switch (e->shape.type) {
            case ShapeType::eLine:
                delete (Line *)(e->shape.data);
                break;
            case ShapeType::eCircle:
                delete (Circle *)(e->shape.data);
                break;
            case ShapeType::ePolygen:
                delete (Polygen *)(e->shape.data);
                break;
            case ShapeType::eEllipse:
                delete (Ellipse *)(e->shape.data);
                break;
            default:
                LSPE_ASSERT(false);
        }
        delete e;
    }

    objects.clear();
}

bbox2 lspeman::bboxOf(Shape shape) {
    // qDebug() << "Perform bboxOf(Shape, int)";

    bbox2 box;

    switch (shape.type) {
        case ShapeType::eLine:
            box = lspe::bboxOf(*(Line *)(shape.data));
            break;
        case ShapeType::eCircle:
            box = lspe::bboxOf(*(Circle *)(shape.data));
            break;
        case ShapeType::ePolygen:
            box = lspe::bboxOf(*(Polygen *)(shape.data));
            break;
        case ShapeType::eEllipse:
            box = lspe::bboxOf(*(Ellipse *)(shape.data));
            break;
        default:
            LSPE_ASSERT(false);
    }

    return box;
}

ObjectList &lspeman::getObjects() {
    return objects;
}

void lspeman::newLine() {
    Shape e;
    e.data = new Line;
    e.type = ShapeType::eLine;
    quickGenerate(e);
    addObject(e);
}

void lspeman::newCircle() {
    Shape e;
    e.data = new Circle;
    e.type = ShapeType::eCircle;
    quickGenerate(e);
    addObject(e);
}

void lspeman::newPolygen() {
    Shape e;
    e.data = new Polygen;
    e.type = ShapeType::ePolygen;
    quickGenerate(e);
    addObject(e);
}

void lspeman::newEllipse() {
    Shape e;
    e.data = new Ellipse;
    e.type = ShapeType::eEllipse;
    quickGenerate(e);
    addObject(e);
}

void lspeman::quickGenerate(Shape shape) {
    // qDebug() << "Perform quickGenerate(Shape, int)";

    switch (shape.type) {
        case ShapeType::eLine: {
            auto e    = (Line *)(shape.data);
            e->pa     = {u1(this->e), u1(this->e)};
            e->pb     = {u1(this->e), u1(this->e)};
            e->anchor = u3(this->e);
            e->type   = 0; //! defaultly line segment
        } break;
        case ShapeType::eCircle: {
            auto e    = (Circle *)(shape.data);
            e->center = {u1(this->e), u1(this->e)};
            e->r      = u2(this->e);
        } break;
        case ShapeType::ePolygen: {
            auto e = (Polygen *)(shape.data);
            // const size_t n = (int)u2(this->e);
            vec2 pos(u1(this->e), u1(this->e));
            vec2 size(u2(this->e), u2(this->e));
            e->vertices.clear();
            e->vertices.push_back({pos.x, pos.y});
            e->vertices.push_back({pos.x + size.x, pos.y});
            e->vertices.push_back({pos.x + size.x, pos.y + size.y});
            e->vertices.push_back({pos.x, pos.y + size.y});
            e->center = {
                pos.x + size.x / 2,
                pos.y + size.y / 2,
            };
        } break;
        case ShapeType::eEllipse: {
            auto e      = (Ellipse *)(shape.data);
            e->center   = {u1(this->e), u1(this->e)};
            e->rx       = u2(this->e);
            e->ry       = u2(this->e);
            e->rotation = u3(this->e) * Pi;
        } break;
        default:
            LSPE_ASSERT(false);
    }
}

void lspeman::translate(Shape shape, const vec2 &displacement) {
    // qDebug() << "Perform translate(Shape, int, vec2)";

    switch (shape.type) {
        case ShapeType::eLine: {
            auto e = (Line *)(shape.data);
            e->pa  += displacement;
            e->pb  += displacement;
        } break;
        case ShapeType::eCircle: {
            auto e    = (Circle *)(shape.data);
            e->center += displacement;
        } break;
        case ShapeType::ePolygen: {
            auto e = (Polygen *)(shape.data);
            for (int i = 0; i < e->vertices.size(); ++i) {
                e->vertices[i] += displacement;
            }
            e->center += displacement;
        } break;
        case ShapeType::eEllipse: {
            auto e    = (Ellipse *)(shape.data);
            e->center += displacement;
        } break;
        default:
            LSPE_ASSERT(false);
    }
}

int lspeman::addObject(Shape shape, const bbox2 *pbb) {
    LSPE_ASSERT(tree != nullptr);

    // qDebug() << "Perform addObject(Shape, int, bbox2)";

    bbox2 box;
    if (pbb != nullptr) {
        box = *pbb;
    } else {
        box = bboxOf(shape);
    }

    // qDebug() << "bboxOf(shape, type) = { ("
    // 	<< box.lower.x << "," << box.lower.y << "), ("
    // 	<< box.upper.x << "," << box.upper.y << ") }";

    auto obj = new Object;
    int  oid = tree->addObject(box, (void *)obj);

    obj->shape = shape;
    obj->box   = box;
    obj->index = oid;
    objects.push_back(obj);

    return oid;
}

void lspeman::delObject(int oid) {
    LSPE_ASSERT(tree != nullptr);

    auto it =
        std::find_if(objects.begin(), objects.end(), [oid](const Object *obj) {
            return oid == obj->index;
        });
    if (it != objects.end()) {
        tree->delObject(oid);
        (*it)->index = -1;
    }
}

void lspeman::moveObject(int oid, const bbox2 &box, const vec2 &displacement) {
    LSPE_ASSERT(tree != nullptr);

    auto it =
        std::find_if(objects.begin(), objects.end(), [oid](const Object *obj) {
            return oid == obj->index;
        });

    if (it == objects.end()) return;

    tree->moveObject(oid, box, displacement);
    (*it)->box = {box.lower + displacement, box.upper + displacement};
}

void lspeman::setBBoxExtension(float r) {
    LSPE_ASSERT(r > FLT_EPSILON);
    tree->setExtension(r);
}

void lspeman::setStep(float s) {
    LSPE_ASSERT(s > FLT_EPSILON);
    step = s;
}

void lspeman::traverse(fnvisit visit, void *extra, int method) {
    abt::traverse(tree, visit, extra, method);
}

void lspeman::query(fnvisit processor, const bbox2 &box, void *extra) {
    tree->query(processor, box, extra);
}

void lspeman::stepforward() {
    std::uniform_real_distribution<float> us(-32, 32);

    for (size_t i = 0; i < objects.size(); ++i) {
        auto &box = objects[i]->box;

        if (box.lower.x > 600 || box.lower.y > 600 || box.upper.x < 0
            || box.upper.y < 0) {
            quickGenerate(objects[i]->shape);
            box = bboxOf(objects[i]->shape);
        }

        vec2 displacement(us(e), us(e));
        displacement *= step;
        tree->moveObject(objects[i]->index, box, displacement);

        box = {box.lower + displacement, box.upper + displacement};
        translate(objects[i]->shape, displacement);
    }
}
