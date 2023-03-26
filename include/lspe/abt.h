#pragma once

/********************************
 *  @author: ZYmelaii
 *
 *  @object: Dynamic AABB Tree
 *
 *  @brief: Dynamic Bounding Volume Hierarchy Based On AABB Tree
 *
 *  @NOTES: Reference "box2d"
 *******************************/

#include "../lspe/base/base.h"
#include "../lspe/base/vec.h"
#include "../lspe/bbox.h"

namespace lspe {

namespace abt {
struct node;
}; // namespace abt
class abtree;

namespace abt {

static const int null = -1;

enum {
    PREORDER,
    INORDER,
    POSTORDER
};

//! fnvisit allows a custom visit callback function
//! enable providing an extra pointer for more flexible operation
//! return false if you want to terminate the visit procedure
//! otherwise return true
//! tip: valid part of node includes { box, userdata, height, moved }
typedef bool (*fnvisit)(const node *, void *extra);

void traverse(
    abtree *tree, fnvisit visit, void *extra = nullptr, int method = PREORDER);

struct node {
    bbox2 box;      //! bounding box (expanded defaultly)
    void *userdata; //! available for user

    union {
        int parent; //! parent node (index)
                    //! act as a abtree node when current node is valid
        int next;   //! point to next available free node
                    //! act as a linked list node when current node is free
        int index;  //! index of this node
                    //! this is only available in query function
    };

    int left, right; //! children (index)

    int height; //! node height from the bottom of current sub-tree
                //! normal node >= 0, free node = -1

    bool moved; //! mark if this bounding box was moved
                //! it indicates that the object has finished movement or
                //! placement

    bool isLeaf() const; //! check whether this node is a leaf node
};

}; // namespace abt

class abtree {
public:
    friend void abt::traverse(abtree *, abt::fnvisit, void *, int);

public:
    abtree();
    ~abtree();

    void setExtension(float r);
    //! set extension of bounding box

    int addObject(const bbox2 &box, void *userdata);
    //! add a new object with a bounding box
    //! providing an optional userdata param (usually real object pointer)

    void delObject(int id);
    //! delete the object by id (given by addObject())

    bool moveObject(int id, const bbox2 &box, const vec2 &displacement);
    //! update bounding box of the object and apply the displacement
    //! it'll adjust the bounding box and reinsert the object if the
    //! result was out of the original fatten bounding box
    //! return true if object was reinserted
    //! otherwise return false

    bbox2 getFattenBBox(int id) const;

    void *getUserdata(int id) const;

    bool wasMoved(int id);
    void setUnMoved(int id);
    //! clear move flag of the node

    void query(abt::fnvisit processor, const bbox2 &box, void *extra = nullptr);
    void
        query(abt::fnvisit processor, const vec2 &point, void *extra = nullptr);

protected:

private:
    int allocate();      //! allocate a new node from local pool (return index)
                         //! try doublizing local pool when failed allocating
    void free(int node); //! free a node from local pool

    void insert(int node);  //! insert a leaf node into abtree
                            //! this will call allocate() firstly
    void remove(int node);  //! remove the leaf node
    int  balance(int node); //! perform tree balancing
                            //! return new root of the subtree

    int height() const;           //! get height of abtree
    int heightOf(int node) const; //! get height of specific node

    abt::node *m_nodes; //! node pool
    int        m_root;  //! index of root node
    int        m_nnode; //! number of nodes
                        //! m_nnode = N(internal node) + N(leaf node)
                        //! N(internal node) = N(leaf node) - 1
    int m_capacity;     //! node pool capacity
                        //! initially 16
    int m_freenode;     //! root free node of a linked list
                        //! this is highly composited with the abtree

    float m_extension; //! bounding box extension for leaf node insertion
                       //! defaultly 2 (meters)
};

}; // namespace lspe
