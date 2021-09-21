#include "lspeman.h"

using namespace lspe;
using namespace lspe::abt;

lspeman::lspeman()
	: step(1.0f), e(time(0L)),
	u1(0, 600), u2(4, 64)
{
	tree = new abtree();
	LSPE_ASSERT(tree != nullptr);
}

lspeman::~lspeman()
{
	LSPE_ASSERT(tree != nullptr);
	delete tree;
	tree = nullptr;
}

const ObjectList& lspeman::getObjects() const
{
	return objects;
}

int lspeman::addObject(const bbox2 &box)
{
	LSPE_ASSERT(tree != nullptr);

	int oid = tree->addObject(box, nullptr);
	objects.push_back({ box, oid });

	return oid;
}

void lspeman::delObject(int oid)
{
	LSPE_ASSERT(tree != nullptr);

	auto it = std::find_if(objects.begin(), objects.end(),
		[oid](const Object &obj) { return oid == obj.index; });
	if (it != objects.end())
	{
		tree->delObject(oid);
		objects.erase(it);
	}
}

bool lspeman::moveObject(int oid, const bbox2 &box,
		const vec2 &displacement)
{
	LSPE_ASSERT(tree != nullptr);

	auto it = std::find_if(objects.begin(), objects.end(),
		[oid](const Object &obj) { return oid == obj.index; });
	if (it == objects.end()) return false;

	bool needMove = tree->moveObject(oid, box, displacement);
	if (needMove)
	{
		(*it).box = { box.lower + displacement, box.upper + displacement };
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

void lspeman::stepforward()
{
	std::uniform_real_distribution<float> us(-32, 32);

	for (int i = 0; i < objects.size(); ++i)
	{
		auto &box = objects[i].box;
		if (box.lower.x > 600 || box.lower.y > 600
			|| box.upper.x < 0 || box.upper.y < 0)
		{
			tree->delObject(objects[i].index);

			vec2 pos(u1(e), u1(e));
			vec2 size(u2(e), u2(e));

			box = { pos, pos + size };
			objects[i].index = tree->addObject(box, nullptr);
		}

		vec2 displacement(us(e), us(e));
		displacement *= step;
		tree->moveObject(objects[i].index, box, displacement);
		box = { box.lower + displacement, box.upper + displacement };
	}
}
