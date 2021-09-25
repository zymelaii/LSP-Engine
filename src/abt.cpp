#include <float.h>
#include <malloc.h>
#include <string.h>

#include <lspe/abt.h>

namespace lspe
{

namespace abt
{

bool novisit(const node *node, void *extra)
{
	return true;
}

struct _querywalker
{
	fnvisit processor;
	void   *extra;
	union
	{
		bbox2 *testbox;
		vec2  *testpoint;
	};
	bool    finished;
};

bool queryWalkerWithTestBox(const node *node, void *extra)
{
	auto e = (_querywalker*)extra;

	if (e->finished) return false;

	if (overlap(node->box, *e->testbox))
	{
		if (node->isLeaf())
		{
			e->finished = !e->processor(node, e->extra);
		}
		return !e->finished;
	}

	return false;
}

bool queryWalkerWithTestPoint(const node *node, void *extra)
{
	auto e = (_querywalker*)extra;

	if (e->finished) return false;

	if (contain(node->box, *e->testpoint))
	{
		if (node->isLeaf())
		{
			e->finished = !e->processor(node, e->extra);
		}
		return !e->finished;
	}

	return false;
}

void traversePreorder(node *tree, int index, fnvisit visit, void *extra);
void traverseInorder(node *tree, int index, fnvisit visit, void *extra);
void traversePostorder(node *tree, int index, fnvisit visit, void *extra);

void traverse(abtree *tree, fnvisit visit, void *extra, int method)
{
	LSPE_ASSERT(tree != nullptr);
	LSPE_ASSERT(tree->m_nodes != nullptr);
	LSPE_ASSERT(method == PREORDER
		|| method == INORDER || method == POSTORDER);

	if (visit == nullptr)
	{
		visit = novisit;
	}

	switch (method)
	{
		case PREORDER:
			traversePreorder(tree->m_nodes, tree->m_root, visit, extra);
			break;
		case INORDER:
			traverseInorder(tree->m_nodes, tree->m_root, visit, extra);
			break;
		case POSTORDER:
			traversePostorder(tree->m_nodes, tree->m_root, visit, extra);
			break;
	}
}

void traversePreorder(node *tree, int index, fnvisit visit, void *extra)
{
	if (index == null) return;

	node tmp;
	memcpy(&tmp, tree + index, sizeof(node));
	tmp.index = index;

	bool shouldExit = !visit(&tmp, extra);
	if (shouldExit) return;

	traversePreorder(tree, tree[index].left, visit, extra);
	traversePreorder(tree, tree[index].right, visit, extra);
}

void traverseInorder(node *tree, int index, fnvisit visit, void *extra)
{
	if (index == null) return;

	traversePreorder(tree, tree[index].left, visit, extra);

	node tmp;
	memcpy(&tmp, tree + index, sizeof(node));
	tmp.index = index;

	bool shouldExit = !visit(&tmp, extra);
	if (shouldExit) return;

	traversePreorder(tree, tree[index].right, visit, extra);
}

void traversePostorder(node *tree, int index, fnvisit visit, void *extra)
{
	if (index == null) return;

	traversePreorder(tree, tree[index].left, visit, extra);
	traversePreorder(tree, tree[index].right, visit, extra);

	node tmp;
	memcpy(&tmp, tree + index, sizeof(node));
	tmp.index = index;

	bool shouldExit = !visit(&tmp, extra);
	if (shouldExit) return;
}

bool node::isLeaf() const
{
	return left == null; //! or right == null
}

};

abtree::abtree() : m_nodes(nullptr), m_extension(2.0f)
{
	m_root = abt::null;

	m_capacity = 16;
	m_nnode = 0;
	m_nodes = (abt::node*)malloc(m_capacity * sizeof(abt::node));

	LSPE_ASSERT(m_nodes != nullptr);
	memset(m_nodes, 0, m_capacity * sizeof(abt::node));

	//! build linked list of the free nodes
	for (int i = 0; i < m_capacity - 1; ++i)
	{
		m_nodes[i].next = i + 1;
		m_nodes[i].height = -1;
	}

	m_nodes[m_capacity - 1].next = abt::null;
	m_nodes[m_capacity - 1].height = -1;
	m_freenode = 0;
}

abtree::~abtree()
{
	::free(m_nodes); //! free the entire node pool
	m_nodes = nullptr;
}

void abtree::setExtension(float r)
{
	LSPE_ASSERT(r >= FLT_EPSILON); //! assume r >= 0
	m_extension = r;
}

int abtree::addObject(const bbox2 &box, void *userdata)
{
	int node = allocate();

	m_nodes[node].box.lower = box.lower - m_extension;
	m_nodes[node].box.upper = box.upper + m_extension;
	m_nodes[node].userdata = userdata;
	m_nodes[node].moved = true;

	insert(node);

	return node;
}

void abtree::delObject(int id)
{
	LSPE_ASSERT(id >= 0 && id < m_capacity);
	LSPE_ASSERT(m_nodes[id].isLeaf());

	remove(id);
	this->free(id);
}

bool abtree::moveObject(int id, const bbox2 &box, const vec2 &displacement)
{
	LSPE_ASSERT(id >= 0 && id < m_capacity);
	LSPE_ASSERT(m_nodes[id].isLeaf());

	bbox2 fattenbox;
	fattenbox.lower = box.lower - m_extension;
	fattenbox.upper = box.upper + m_extension;

	//! predict movement
	vec2 dp = displacement * 4.0f;
	if (dp.x < FLT_EPSILON)
	{
		fattenbox.lower.x += dp.x;
	} else
	{
		fattenbox.upper.x += dp.x;
	}
	if (dp.y < FLT_EPSILON)
	{
		fattenbox.lower.y += dp.y;
	} else
	{
		fattenbox.upper.y += dp.y;
	}

	const bbox2 &originbox = m_nodes[id].box;
	if (contain(originbox, box))
	{
		bbox2 bound;
		bound.lower = fattenbox.lower - m_extension * 4.0f;
		bound.upper = fattenbox.upper + m_extension * 4.0f;

		if (contain(bound, originbox))
		{
			//! origin bounding box still contains new bounding box
			//! and isn't too large
			return false;
		}

		//! origin bounding box is too large and needs to be shrunk
	}

	remove(id);
	m_nodes[id].box = fattenbox;
	insert(id);
	m_nodes[id].moved = true;

	return true;
}

bbox2 abtree::getFattenBBox(int id) const
{
	LSPE_ASSERT(id >= 0 && id < m_capacity);
	LSPE_ASSERT(m_nodes[id].isLeaf());

	bbox2 box = m_nodes[id].box;

	return { box.lower - m_extension, box.upper + m_extension };
}

void* abtree::getUserdata(int id) const
{
	LSPE_ASSERT(id >= 0 && id < m_capacity);
	LSPE_ASSERT(m_nodes[id].isLeaf());

	return m_nodes[id].userdata;
}

bool abtree::wasMoved(int id)
{
	LSPE_ASSERT(id >= 0 && id < m_capacity);
	LSPE_ASSERT(m_nodes[id].isLeaf());

	return m_nodes[id].moved;
}

void abtree::setUnMoved(int id)
{
	LSPE_ASSERT(id >= 0 && id < m_capacity);
	LSPE_ASSERT(m_nodes[id].isLeaf());

	m_nodes[id].moved = false;
}

void abtree::query(
	abt::fnvisit processor,
	const bbox2 &box, void *extra)
{
	LSPE_ASSERT(processor != nullptr);

	bbox2 testbox = box;

	abt::_querywalker qw;
	qw.processor = processor;
	qw.extra     = extra;
	qw.testbox   = &testbox;
	qw.finished  = false;

	abt::traversePreorder(
		m_nodes, m_root, abt::queryWalkerWithTestBox, &qw);
}

void abtree::query(
	abt::fnvisit processor,
	const vec2 &point, void *extra)
{
	LSPE_ASSERT(processor != nullptr);

	vec2 testpoint = point;

	abt::_querywalker qw;
	qw.processor = processor;
	qw.extra     = extra;
	qw.testpoint = &testpoint;
	qw.finished  = false;

	abt::traversePreorder(
		m_nodes, m_root, abt::queryWalkerWithTestPoint, &qw);
}

int abtree::allocate()
{
	if (m_freenode == abt::null)
	{	//! expand the node pool
		abt::node *old_nodes = m_nodes;

		m_capacity *= 2;
		m_nodes = (abt::node*)malloc(m_capacity * sizeof(abt::node));

		LSPE_ASSERT(m_nodes != nullptr);
		memcpy(m_nodes, old_nodes, m_nnode * sizeof(abt::node));
		::free(old_nodes);

		for (int i = m_nnode; i < m_capacity - 1; ++i)
		{
			m_nodes[i].next = i + 1;
			m_nodes[i].height = -1;
		}

		m_nodes[m_capacity - 1].next = abt::null;
		m_nodes[m_capacity - 1].height = -1;
		m_freenode = m_nnode;
	}

	int node = m_freenode; //! pick a node from linked list of free nodes
	m_freenode = m_nodes[m_freenode].next;

	m_nodes[node].parent = abt::null;
	m_nodes[node].left = abt::null;
	m_nodes[node].right = abt::null;
	m_nodes[node].height = 0;
	m_nodes[node].userdata = nullptr;
	m_nodes[node].moved = false;

	++m_nnode;

	return node;
}

void abtree::free(int node)
{
	LSPE_ASSERT(node >= 0 && node < m_capacity);
	LSPE_ASSERT(m_nnode > 0);

	m_nodes[node].next = m_freenode;
	m_nodes[node].height = -1;
	m_freenode = node;

	--m_nnode;
}

void abtree::insert(int node)
{
	if (m_root == abt::null)
	{
		m_root = node;
		m_nodes[m_root].parent = abt::null;
		return;
	}

	//! find the best sibling for this node
	//! according to the minimum compute cost
	const bbox2 &originbox = m_nodes[node].box;
	int cursor = m_root;
	while (!m_nodes[cursor].isLeaf())
	{
		int left = m_nodes[cursor].left;
		int right = m_nodes[cursor].right;

		bbox2 unionbox = unionOf(m_nodes[cursor].box, originbox);

		float area = areaOf(m_nodes[cursor].box);
		float unionArea = areaOf(unionbox);

		//! cost of creating a new parent for current node pair
		float cost = unionArea * 2.0f;

		//! minimum cost of pushing the node further down the tree
		float inherCost = (unionArea - area) * 2.0f;

		//! cost of descening into left child
		float leftCost = inherCost
			+ areaOf(unionOf(originbox, m_nodes[left].box));
		if (!m_nodes[left].isLeaf())
		{
			leftCost -= areaOf(m_nodes[left].box);
		}

		//! cost of descening into right child
		float rightCost = inherCost
			+ areaOf(unionOf(originbox, m_nodes[right].box));
		if (!m_nodes[right].isLeaf())
		{
			rightCost -= areaOf(m_nodes[right].box);
		}

		//! current node is the best sibling
		if (cost < leftCost && cost < rightCost) break;

		//! preform descending
		cursor = leftCost < rightCost ? left : right;
	}

	//! create a new parent
	int sibling = cursor;
	int old_parent = m_nodes[sibling].parent;
	int new_parent = allocate();
	m_nodes[new_parent].parent = old_parent;
	m_nodes[new_parent].userdata = nullptr;
	m_nodes[new_parent].box = unionOf(originbox, m_nodes[sibling].box);
	m_nodes[new_parent].height = m_nodes[sibling].height + 1;

	if (old_parent == abt::null)
	{	//! the sibling is the root
		m_root = new_parent;
	} else
	{
		if (m_nodes[old_parent].left == sibling)
		{
			m_nodes[old_parent].left = new_parent;
		} else
		{
			m_nodes[old_parent].right = new_parent;
		}
	}

	m_nodes[new_parent].left = sibling;
	m_nodes[new_parent].right = node;
	m_nodes[sibling].parent = new_parent;
	m_nodes[node].parent = new_parent;

	//! walk back up the tree and fixing height and bounding boxes
	cursor = m_nodes[node].parent;
	while (cursor != abt::null)
	{
		cursor = balance(cursor);

		int left = m_nodes[cursor].left;
		int right = m_nodes[cursor].right;

		LSPE_ASSERT(left != abt::null);
		LSPE_ASSERT(right != abt::null);

		m_nodes[cursor].height =
			max(m_nodes[left].height, m_nodes[right].height) + 1;
		m_nodes[cursor].box = unionOf(m_nodes[left].box, m_nodes[right].box);

		cursor = m_nodes[cursor].parent;
	}
}

void abtree::remove(int node)
{
	if (node == m_root)
	{
		m_root = abt::null;
		return;
	}

	int parent = m_nodes[node].parent;
	int grandparent = m_nodes[parent].parent;
	int sibling = m_nodes[parent].left == node ?
		m_nodes[parent].right : m_nodes[parent].left;

	if (grandparent != abt::null)
	{
		if (m_nodes[grandparent].left == parent)
		{
			m_nodes[grandparent].left = sibling;
		} else
		{
			m_nodes[grandparent].right = sibling;
		}
		m_nodes[sibling].parent = grandparent;

		this->free(parent);

		int cursor = grandparent;
		while (cursor != abt::null)
		{
			cursor = balance(cursor);

			int left = m_nodes[cursor].left;
			int right = m_nodes[cursor].right;

			LSPE_ASSERT(left != abt::null);
			LSPE_ASSERT(right != abt::null);

			m_nodes[cursor].height =
				max(m_nodes[left].height, m_nodes[right].height) + 1;
			m_nodes[cursor].box = unionOf(m_nodes[left].box, m_nodes[right].box);

			cursor = m_nodes[cursor].parent;
		}
	} else
	{	//! the parent is the root
		m_root = sibling;
		m_nodes[sibling].parent = abt::null;
		this->free(parent);
	}
}

int abtree::balance(int node)
{
/** input:
 *                 ...                     ...
 *                 /                        \
 *                A                         A
 *              /   \                     /   \
 *             B     C                   B     C
 *            / \   / \                 / \   / \
 *           D  E  F  G                D  E  F  G
 *  output[heightOf(B) > heightOf(C) && height(D) > height(E)]:
 *                 ...                     ...
 *                 /                        \
 *                B                         B
 *              /   \                     /   \
 *             A     D                   A     D
 *            / \                       / \
 *           E  C                      E  C
 *             / \                       / \
 *            F  G                      F  G
 *  output[heightOf(B) > heightOf(C) && height(D) <= height(E)]:
 *                 ...                     ...
 *                 /                        \
 *                B                         B
 *              /   \                     /   \
 *             A     E                   A     E
 *            / \                       / \
 *           D  C                      D  C
 *             / \                       / \
 *            F  G                      F  G
 *  output[heightOf(C) > heightOf(B) && height(F) > height(G)]:
 *                 ...                     ...
 *                 /                        \
 *                C                         C
 *              /   \                     /   \
 *             A     F                   A     F
 *            / \                       / \
 *           B  G                      B  G
 *          / \                       / \
 *         D  E                      D  E
 *  output[heightOf(C) > heightOf(B) && height(F) <= height(G)]:
 *                 ...                     ...
 *                 /                        \
 *                C                         C
 *              /   \                     /   \
 *             A     G                   A     G
 *            / \                       / \
 *           B  F                      B  F
 *          / \                       / \
 *         D  E                      D  E
 **/

	LSPE_ASSERT(node != abt::null);

	int nodeA = node;
	abt::node *A = m_nodes + nodeA;
	if (A->isLeaf() || A->height < 2)
	{
		return nodeA;
	}

	int nodeB = A->left;
	int nodeC = A->right;
	abt::node *B = m_nodes + nodeB;
	abt::node *C = m_nodes + nodeC;
	LSPE_ASSERT(nodeB >= 0 && nodeB < m_capacity);
	LSPE_ASSERT(nodeC >= 0 && nodeC < m_capacity);

	int balance = B->height - C->height;

	if (balance > 1)
	{	//! node B is too deep
		int nodeD = B->left;
		int nodeE = B->right;
		abt::node *D = m_nodes + nodeD;
		abt::node *E = m_nodes + nodeE;
		LSPE_ASSERT(nodeD >= 0 && nodeD < m_capacity);
		LSPE_ASSERT(nodeE >= 0 && nodeE < m_capacity);

		B->left = nodeA;
		B->parent = A->parent;
		A->parent = nodeB;

		if (B->parent != abt::null)
		{
			if (m_nodes[B->parent].left == nodeA)
			{
				m_nodes[B->parent].left = nodeB;
			}
			else
			{
				m_nodes[B->parent].right = nodeB;
			}
		} else
		{
			m_root = nodeB;
		}

		if (D->height > E->height)
		{
			B->right = nodeD;
			A->left = nodeE;
			E->parent = nodeA;

			A->box = unionOf(C->box, E->box);
			B->box = unionOf(A->box, D->box);

			A->height = max(C->height, E->height) + 1;
			B->height = max(A->height, D->height) + 1;
		} else
		{
			B->right = nodeE;
			A->left = nodeD;
			D->parent = nodeA;

			A->box = unionOf(C->box, D->box);
			B->box = unionOf(A->box, E->box);

			A->height = max(C->height, D->height) + 1;
			B->height = max(A->height, E->height) + 1;
		}

		return nodeB;
	}

	if (balance < -1)
	{	//! node C is too deep
		int nodeF = C->left;
		int nodeG = C->right;
		abt::node *F = m_nodes + nodeF;
		abt::node *G = m_nodes + nodeG;
		LSPE_ASSERT(nodeF >= 0 && nodeF < m_capacity);
		LSPE_ASSERT(nodeG >= 0 && nodeG < m_capacity);

		C->left = nodeA;
		C->parent = A->parent;
		A->parent = nodeC;

		if (C->parent != abt::null)
		{
			if (m_nodes[C->parent].left == nodeA)
			{
				m_nodes[C->parent].left = nodeC;
			}
			else
			{
				m_nodes[C->parent].right = nodeC;
			}
		} else
		{
			m_root = nodeC;
		}

		if (F->height > G->height)
		{
			C->right = nodeF;
			A->right = nodeG;
			G->parent = nodeA;

			A->box = unionOf(B->box, G->box);
			C->box = unionOf(A->box, F->box);

			A->height = max(B->height, G->height) + 1;
			C->height = max(A->height, F->height) + 1;
		} else
		{
			C->right = nodeG;
			A->right = nodeF;
			F->parent = nodeA;

			A->box = unionOf(B->box, F->box);
			C->box = unionOf(A->box, G->box);

			A->height = max(B->height, F->height) + 1;
			C->height = max(A->height, G->height) + 1;
		}

		return nodeC;
	}

	return nodeA;
}

int abtree::height() const
{
	return heightOf(m_root);
}

int abtree::heightOf(int node) const
{
	return m_nodes[node].height;
}

};
