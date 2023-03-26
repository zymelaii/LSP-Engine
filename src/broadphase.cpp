#include <string.h>
#include <lspe/broadphase.h>

namespace lspe {

using namespace broadphase;

BroadPhase::BroadPhase()
    : moveCapacity(16)
    , moveCount(0)
    , pairCapacity(16)
    , pairCount(0)
    , queryId(abt::null) {
    moveBuffer = (int *)malloc(moveCapacity * sizeof(int));
    LSPE_ASSERT(moveBuffer != nullptr);
    memset(moveBuffer, 0, moveCapacity * sizeof(int));

    pairBuffer = (IntPair *)malloc(pairCapacity * sizeof(IntPair));
    LSPE_ASSERT(pairBuffer != nullptr);
    memset(pairBuffer, 0, pairCapacity * sizeof(IntPair));
}

BroadPhase::~BroadPhase() {
    free(moveBuffer);
    moveBuffer = nullptr;

    free(pairBuffer);
    pairBuffer = nullptr;
}

int BroadPhase::addObject(const bbox2 &box, void *userdata) {
    int index = tree.addObject(box, userdata);
    addMove(index);
    return index;
}

void BroadPhase::delObject(int id) {
    delMove(id);
    tree.delObject(id);
}

void BroadPhase::moveObject(
    int id, const bbox2 &box, const vec2 &displacement) {
    bool shouldBuffer = tree.moveObject(id, box, displacement);
    if (shouldBuffer) { addMove(id); }
}

void BroadPhase::addMove(int id) {
    if (moveCount == moveCapacity) {
        auto oldMoveBuffer = moveBuffer;

        moveCapacity *= 2;
        moveBuffer   = (int *)malloc(moveCapacity * sizeof(int));
        LSPE_ASSERT(moveBuffer != nullptr);
        memset(moveBuffer, 0, moveCapacity * sizeof(int));

        memcpy(moveBuffer, oldMoveBuffer, moveCount * sizeof(int));
        free(oldMoveBuffer);
    }

    moveBuffer[moveCount] = id;
    ++moveCount;
}

void BroadPhase::delMove(int id) {
    LSPE_ASSERT(id >= 0 && id < moveCount);

    for (int i = 0; i < moveCount; ++i) {
        if (moveBuffer[i] == id) { moveBuffer[i] = abt::null; }
    }
}

const IntPair *BroadPhase::getPairs(int *count) const {
    LSPE_ASSERT(count != nullptr);

    *count = pairCount;
    return pairBuffer;
}

void BroadPhase::updatePairs() {
    pairCount = 0;

    //! query all buffered objects and add new pairs
    for (int i = 0; i < moveCount; ++i) {
        queryId = moveBuffer[i];
        if (queryId == abt::null) continue;

        bbox2 box = tree.getFattenBBox(queryId);
        tree.query(_query, box, this);
    }

    //! all things done
    //! moveBuffer can be cleared
    for (int i = 0; i < moveCount; ++i) {
        queryId = moveBuffer[i];
        if (queryId == abt::null) continue;

        tree.setUnMoved(queryId);
    }

    moveCount = 0;
}

void *BroadPhase::getUserdata(int id) const {
    return tree.getUserdata(id);
}

void BroadPhase::query(abt::fnvisit processor, const bbox2 &box, void *extra) {
    tree.query(processor, box, extra);
}

void BroadPhase::traverse(abt::fnvisit visit, void *extra, int method) {
    abt::traverse(&tree, visit, extra, method);
}

bool BroadPhase::_query(const abt::node *node, void *extra) {
    auto bp = (BroadPhase *)extra;
    LSPE_ASSERT(bp != nullptr);

    auto &tree = bp->tree;

    //! skip self
    if (node->index == bp->queryId) { return true; }

    //! the pair exists or etc.
    if (tree.wasMoved(bp->queryId) && bp->queryId < node->index) {
        return true;
    }

    if (bp->pairCount == bp->pairCapacity) {
        auto oldPairBuffer = bp->pairBuffer;

        bp->pairCapacity *= 2;
        bp->pairBuffer = (IntPair *)malloc(bp->pairCapacity * sizeof(IntPair));
        LSPE_ASSERT(bp->pairBuffer != nullptr);
        memset(bp->pairBuffer, 0, bp->pairCapacity * sizeof(IntPair));

        memcpy(bp->pairBuffer, oldPairBuffer, bp->pairCount * sizeof(IntPair));
        free(oldPairBuffer);
    }

    bp->pairBuffer[bp->pairCount].first  = min(bp->queryId, node->index);
    bp->pairBuffer[bp->pairCount].second = max(bp->queryId, node->index);
    ++bp->pairCount;

    return true;
}

}; // namespace lspe
