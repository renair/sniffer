#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include "tree.h"
#include "addrconverter.h"

void dump_tree_recurse(FILE*, tree*);

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

void dump_tree(char* filename, tree* root)
{
	if(root != 0)
	{
		FILE* fp = fopen(filename,"w");
		dump_tree_recurse(fp, root);
		fclose(fp);
	}
}

void dump_tree_recurse(FILE* file, tree* node)
{
	if(node != 0)
	{
		fwrite(&(node->_value),sizeof(unsigned long), 1, file);
		fwrite(&(node->_count),sizeof(unsigned long), 1, file);
		dump_tree_recurse(file, node->_left);
		dump_tree_recurse(file, node->_right);
	}
}

tree* load_tree(char* filename)
{
	FILE* fp = fopen(filename, "r");
	if(fp == NULL)
	{
		return create_node(0x00008888);
	}
	unsigned long value = 0;
	unsigned long count = 0;
	unsigned int readed = 1;
	fread(&value, sizeof(unsigned long), 1, fp);
	fread(&count, sizeof(unsigned long), 1, fp);
	tree* root = create_node(value);
	root->_count = count;
	while(fread(&value, sizeof(unsigned long), 1, fp) & fread(&count, sizeof(unsigned long), 1, fp))
	{
		tree* node = find_node(root, value);
		node->_count = count;
	}
	fclose(fp);
	return root;
}
