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
#include <lspe/shape.h>
#include <lspe/collision.h>

enum { LINE, CIRCLE, POLYGEN, ELLIPSE };

struct Object
{
	lspe::Shape shape;
	int type;
	lspe::bbox2 box;
	int index;
};

using ObjectList = std::vector<Object*>;

class lspeman
{
public:
	lspeman();
	~lspeman();

	static lspe::bbox2 bboxOf(lspe::Shape shape, int type);

	ObjectList& getObjects();

	void newLine();
	void newCircle();
	void newPolygen();
	void newEllipse();

	void quickGenerate(lspe::Shape shape, int type);
	void translate(lspe::Shape shape, int type, const lspe::vec2 &displacement);

	int addObject(lspe::Shape shape, int type,
		const lspe::bbox2 *pbb = nullptr);
	void delObject(int oid);
	void moveObject(int oid, const lspe::bbox2 &box,
		const lspe::vec2 &displacement);

	void setBBoxExtension(float r);
	void setStep(float s);

	void traverse(lspe::abt::fnvisit visit,
		void *extra = nullptr, int method = lspe::abt::PREORDER);

	void query(lspe::abt::fnvisit processor,
		const lspe::bbox2 &box, void *extra = nullptr);
	void query(lspe::abt::fnvisit processor,
		const lspe::vec2 &point, void *extra = nullptr);

	void stepforward(); //! customizable function

private:
	ObjectList objects;
	lspe::abtree *tree;

	float step;

	std::mt19937 e; //! random utils
	std::uniform_real_distribution<float> u1;
	std::uniform_real_distribution<float> u2;
	std::uniform_real_distribution<float> u3;

private:
	lspeman(const lspeman &man) = delete;

};
