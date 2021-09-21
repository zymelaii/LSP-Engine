#pragma once

#include <vector>
#include <QDebug>
#include <ctime>
#include <random>
#include <float.h>

#include <lspe/base/base.h>
#include <lspe/base/vec.h>
#include <lspe/base/mat.h>
#include <lspe/abt.h>

struct Object { lspe::bbox2 box; int index; };
using ObjectList = std::vector<Object>;

class lspeman
{
public:
	lspeman();
	~lspeman();

	const ObjectList& getObjects() const;

	int addObject(const lspe::bbox2 &box);
	void delObject(int oid);
	bool moveObject (int oid, const lspe::bbox2 &box,
		const lspe::vec2 &displacement);

	void setBBoxExtension(float r);
	void setStep(float s);

	void traverse(lspe::abt::fnvisit visit,
		void *extra = nullptr, int method = lspe::abt::PREORDER);

	void stepforward(); //! customizable function

private:
	ObjectList objects;
	lspe::abtree *tree;

	float step;

	std::mt19937 e; //! random utils
	std::uniform_real_distribution<float> u1;
	std::uniform_real_distribution<float> u2;

private:
	lspeman(const lspeman &man) = delete;

};
