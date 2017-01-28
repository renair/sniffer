#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include "tree.h"
#include "addrconverter.h"

tree* create_node(unsigned long value)
{
	tree* node = (tree*)malloc(sizeof(tree));
	node->_value = value;
	node->_left = 0;
	node->_right = 0;
	node->_count = 0;
	return node;
}

void clean_tree(tree* root)
{
	if(root != 0)
	{
		clean_tree(root->_left);
		clean_tree(root->_right);
		free(root);
	}
}

tree* find_node(tree* t, unsigned long addr)
{
	if(t->_value < addr)
	{
		return t->_left == 0 ? (t->_left = create_node(addr)) : find_node(t->_left, addr);
	}
	else if(t->_value > addr)
	{
		return t->_right == 0 ? (t->_right = create_node(addr)) : find_node(t->_right, addr);
	}
	else
	{
		return t;
	}
}

void print_tree(tree* node)
{
	if(node != 0)
	{
		char ip[15];
		printf("%s:%lu\n",long_to_ip(node->_value, ip, 15), node->_count);
		print_tree(node->_left);
		print_tree(node->_right);
	}
}

