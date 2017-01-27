#ifndef TREE_H
#define TREE_H

#include<string.h>

struct tree
{
	struct tree* _left;
	struct tree* _rigth;
	unsigned long _ip;
	int _count;
};

typedef struct tree tree;

void tree_init(tree*);
tree* tree_node(tree*, unsigned long);

#endif
