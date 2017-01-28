#ifndef TREE_H
#define TREE_H

#include<string.h>

struct tree
{
	struct tree* _left;
	struct tree* _right;
	unsigned long _value;
	unsigned long _count;
};

typedef struct tree tree;

tree* create_node(unsigned long);
void clean_tree(tree*);
tree* find_node(tree*, unsigned long);
void print_tree(tree*);

#endif
