#ifndef TREE_H
#define TREE_H

#include "variation.h"

#include <vector>

class Node
{
public:
    Node();

    float minimax( bool maximizing );

    Variation variation_;

    std::vector<Node> children_;
};

class Tree
{
public:
    Node root;
};

#endif