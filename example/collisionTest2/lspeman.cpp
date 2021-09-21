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
			case LINE:    delete (Line   *)e->shape; break;
			case CIRCLE:  delete (Circle *)e->shape; break;
			case POLYGEN: delete (Polygen*)e->shape; break;
			case ELLIPSE: delete (Ellipse*)e->shape; break;
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
		case LINE:    box = lspe::bboxOf(*(Line   *)shape); break;
		case CIRCLE:  box = lspe::bboxOf(*(Circle *)shape); break;
		case POLYGEN: box = lspe::bboxOf(*(Polygen*)shape); break;
		case ELLIPSE: box = lspe::bboxOf(*(Ellipse*)shape); break;
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
	quickGenerate((Shape)e, LINE);
	addObject((Shape)e, LINE);
}

void lspeman::newCircle()
{
	auto e = new Circle;
	quickGenerate((Shape)e, CIRCLE);
	addObject((Shape)e, CIRCLE);
}

void lspeman::newPolygen()
{
	auto e = new Polygen;
	quickGenerate((Shape)e, POLYGEN);
	addObject((Shape)e, POLYGEN);
}

void lspeman::newEllipse()
{
	auto e = new Ellipse;
	quickGenerate((Shape)e, ELLIPSE);
	addObject((Shape)e, ELLIPSE);
}

void lspeman::quickGenerate(Shape shape, int type)
{
	LSPE_ASSERT(shape != nullptr);

	// qDebug() << "Perform quickGenerate(Shape, int)";

	switch (type)
	{
		case LINE:
		{
			auto e = (Line*)shape;
			e->pa = { u1(this->e), u1(this->e) };
			e->pb = { u1(this->e), u1(this->e) };
			e->anchor = u3(this->e);
			e->type = 0; //! defaultly line segment
		}
		break;
		case CIRCLE:
		{
			auto e = (Circle*)shape;
			e->center = { u1(this->e), u1(this->e) };
			e->r = u2(this->e);
		}
		break;
		case POLYGEN:
		{
			auto e = (Polygen*)shape;
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
			auto e = (Ellipse*)shape;
			e->center = { u1(this->e), u1(this->e) };
			e->rx = u2(this->e);
			e->ry = u2(this->e);
			e->rotation = u3(this->e) * Pi;
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
			auto e = (Line*)shape;
			e->pa += displacement;
			e->pb += displacement;
		}
		break;
		case CIRCLE:
		{
			auto e = (Circle*)shape;
			e->center += displacement;
		}
		break;
		case POLYGEN:
		{
			auto e = (Polygen*)shape;
			for (int i = 0; i < e->vertices.size(); ++i)
			{
				e->vertices[i] += displacement;
			}
			e->center += displacement;
		}
		break;
		case ELLIPSE:
		{
			auto e = (Ellipse*)shape;
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

bool lspeman::moveObject(int oid, const bbox2 &box,
		const vec2 &displacement)
{
	LSPE_ASSERT(tree != nullptr);

	auto it = std::find_if(objects.begin(), objects.end(),
		[oid](const Object *obj) { return oid == obj->index; });
	if (it == objects.end()) return false;

	bool needMove = tree->moveObject(oid, box, displacement);
	if (needMove)
	{
		(*it)->box = { box.lower + displacement, box.upper + displacement };
	}

	return needMove;
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

void lspeman::stepforward()
{
	
}
