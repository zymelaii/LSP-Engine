#pragma once

#include "../lspe/base/base.h"
#include "../lspe/base/vec.h"
#include "../lspe/abt.h"

namespace lspe
{

namespace broadphase
{

typedef void (*fnnewpair)(
	void *firstData, void *secondData, void *extra);

struct IntPair
{
	int first;
	int second;
};

};

/********************************
 *  @author: ZYmelaii
 *
 *  @BroadPhase: intro for BroadPhase
 *
 *  @brief: approximately an proxy of abtree
 *
 *  @NOTES: 
 *******************************/
class BroadPhase
{
// public: friend BroadPhase::_query(const abt::node *node, void *extra);
public:
	BroadPhase();
	~BroadPhase();

	int  addObject(const bbox2 &box, void *userdata);
	void delObject(int id);
	void moveObject(int id, const bbox2 &box, const vec2 &displacement);

	void addMove(int id);
	void delMove(int id);

	const broadphase::IntPair* getPairs(int *count) const;
	void updatePairs();

	void* getUserdata(int id) const;

	void query(abt::fnvisit processor, const bbox2 &box, void *extra);
		//! query function that calls abtree::query()

protected:

private:
	abtree tree;

	int queryId;

	//! buffered all potential collision-rich objects
	int *moveBuffer;
	int moveCapacity;
	int moveCount;

	//! buffered all test pairs
	//! a pair is construct with (firstIndex, secondIndex)
	//! and there always exists firstIndex > secondIndex
	//! and some optimization can be applied
	broadphase::IntPair *pairBuffer;
	int pairCapacity;
	int pairCount;

	static bool _query(const abt::node *node, void *extra);
		//! query callback for abtree query

};

};
