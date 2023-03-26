#include "lspeman.h"

using namespace lspe;
using namespace lspe::abt;
using namespace lspe::shape;
using namespace lspe::collision;

lspeman::lspeman()
	: step(1.0f), e(time(0L)),
	u1(0, 600), u2(4, 64), u3(0, 1)
{
	tree = new abtree();
	LSPE_ASSERT(tree != nullptr);
}

lspeman::~lspeman()
{
	LSPE_ASSERT(tree != nullptr);

	delete tree;
	tree = nullptr;

	for (auto e : objects)
	{
		switch (e->type)
		{
			case LINE:    delete (Line   *)e->shape.data; break;
			case CIRCLE:  delete (Circle *)e->shape.data; break;
			case POLYGEN: delete (Polygen*)e->shape.data; break;
			case ELLIPSE: delete (Ellipse*)e->shape.data; break;
			default: LSPE_ASSERT(false);
		}
		delete e;
	}

	objects.clear();
}

bbox2 lspeman::bboxOf(Shape shape, int type)
{
	LSPE_ASSERT(shape != nullptr);

	// qDebug() << "Perform bboxOf(Shape, int)";

	bbox2 box;

	switch (type)
	{
		case LINE:    box = lspe::bboxOf(*(Line   *)shape.data); break;
		case CIRCLE:  box = lspe::bboxOf(*(Circle *)shape.data); break;
		case POLYGEN: box = lspe::bboxOf(*(Polygen*)shape.data); break;
		case ELLIPSE: box = lspe::bboxOf(*(Ellipse*)shape.data); break;
		default: LSPE_ASSERT(false);
	}

	return box;
}

ObjectList& lspeman::getObjects()
{
	return objects;
}

void lspeman::newLine()
{
	auto e = new Line;
	Shape shape;
	shape.data = e;
	shape.type = ShapeType::eLine;
	quickGenerate(shape, LINE);
	addObject(shape, LINE);
}

void lspeman::newCircle()
{
	auto e = new Circle;
	Shape shape;
	shape.data = e;
	shape.type = ShapeType::eCircle;
	quickGenerate(shape, CIRCLE);
	addObject(shape, CIRCLE);
}

void lspeman::newPolygen()
{
	auto e = new Polygen;
	Shape shape;
	shape.data = e;
	shape.type = ShapeType::ePolygen;
	quickGenerate(shape, POLYGEN);
	addObject(shape, POLYGEN);
}

void lspeman::newEllipse()
{
	auto e = new Ellipse;
	Shape shape;
	shape.data = e;
	shape.type = ShapeType::eEllipse;
	quickGenerate(shape, ELLIPSE);
	addObject(shape, ELLIPSE);
}

void lspeman::quickGenerate(Shape shape, int type)
{
	LSPE_ASSERT(shape != nullptr);

	// qDebug() << "Perform quickGenerate(Shape, int)";

	switch (type)
	{
		case LINE:
		{
			auto e = (Line*)shape.data;
			e->pa = { u1(this->e), u1(this->e) };
			e->pb = { u1(this->e), u1(this->e) };
			e->anchor = u3(this->e);
			e->type = 0; //! defaultly line segment
		}
		break;
		case CIRCLE:
		{
			auto e = (Circle*)shape.data;
			e->center = { u1(this->e), u1(this->e) };
			e->r = u2(this->e);
		}
		break;
		case POLYGEN:
		{
			auto e = (Polygen*)shape.data;
			// const size_t n = (int)u2(this->e);
			vec2 pos(u1(this->e), u1(this->e));
			vec2 size(u2(this->e), u2(this->e));
			e->vertices.clear();
			e->vertices.push_back({ pos.x, pos.y });
			e->vertices.push_back({ pos.x + size.x, pos.y });
			e->vertices.push_back({ pos.x + size.x, pos.y + size.y });
			e->vertices.push_back({ pos.x, pos.y + size.y });
			e->center =
			{
				pos.x + size.x / 2,
				pos.y + size.y / 2,
			};
		}
		break;
		case ELLIPSE:
		{
			auto e = (Ellipse*)shape.data;
			e->center = { u1(this->e), u1(this->e) };
			e->rx = u2(this->e);
			e->ry = u2(this->e);
			e->rotation = u3(this->e) * Pi;
			// e->rotation = 0;
		}
		break;
		default: LSPE_ASSERT(false);
	}
}

void lspeman::translate(Shape shape, int type, const vec2 &displacement)
{
	LSPE_ASSERT(shape != nullptr);

	// qDebug() << "Perform translate(Shape, int, vec2)";

	switch (type)
	{
		case LINE:
		{
			auto e = (Line*)shape.data;
			e->pa += displacement;
			e->pb += displacement;
		}
		break;
		case CIRCLE:
		{
			auto e = (Circle*)shape.data;
			e->center += displacement;
		}
		break;
		case POLYGEN:
		{
			auto e = (Polygen*)shape.data;
			for (int i = 0; i < e->vertices.size(); ++i)
			{
				e->vertices[i] += displacement;
			}
			e->center += displacement;
		}
		break;
		case ELLIPSE:
		{
			auto e = (Ellipse*)shape.data;
			e->center += displacement;
		}
		break;
		default: LSPE_ASSERT(false);
	}
}

int lspeman::addObject(Shape shape, int type, const bbox2 *pbb)
{
	LSPE_ASSERT(tree != nullptr);
	LSPE_ASSERT(shape != nullptr);

	// qDebug() << "Perform addObject(Shape, int, bbox2)";

	bbox2 box;
	if (pbb != nullptr)
	{
		box = *pbb;
	} else
	{
		box = bboxOf(shape, type);
	}

	// qDebug() << "bboxOf(shape, type) = { ("
	// 	<< box.lower.x << "," << box.lower.y << "), ("
	// 	<< box.upper.x << "," << box.upper.y << ") }";

	auto obj = new Object;
	int oid = tree->addObject(box, (void*)obj);

	obj->shape = shape;
	obj->type  = type;
	obj->box   = box;
	obj->index = oid;
	objects.push_back(obj);

	return oid;
}

void lspeman::delObject(int oid)
{
	LSPE_ASSERT(tree != nullptr);

	auto it = std::find_if(objects.begin(), objects.end(),
		[oid](const Object *obj) { return oid == obj->index; });
	if (it != objects.end())
	{
		tree->delObject(oid);
		(*it)->index = -1;
	}
}

void lspeman::moveObject(int oid, const bbox2 &box,
		const vec2 &displacement)
{
	LSPE_ASSERT(tree != nullptr);

	auto it = std::find_if(objects.begin(), objects.end(),
		[oid](const Object *obj) { return oid == obj->index; });
	if (it == objects.end()) return;

	bool needMove = tree->moveObject(oid, box, displacement);
	(*it)->box = { box.lower + displacement, box.upper + displacement };
}

void lspeman::setBBoxExtension(float r)
{
	LSPE_ASSERT(r > FLT_EPSILON);
	tree->setExtension(r);
}

void lspeman::setStep(float s)
{
	LSPE_ASSERT(s > FLT_EPSILON);
	step = s;
}

void lspeman::traverse(fnvisit visit, void *extra, int method)
{
	abt::traverse(tree, visit, extra, method);
}

void lspeman::query(fnvisit processor,
	const bbox2 &box, void *extra)
{
	tree->query(processor, box, extra);
}

void lspeman::query(fnvisit processor,
	const vec2 &point, void *extra)
{
	tree->query(processor, point, extra);
}

void lspeman::stepforward()
{
	
}
