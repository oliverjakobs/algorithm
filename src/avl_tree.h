#ifndef AVL_TREE_H
#define AVL_TREE_H

/*
 * AVL tree node
 */
typedef struct avl_tree_node
{
	void* value;
	struct avl_tree_node* parent;
	struct avl_tree_node* left;
	struct avl_tree_node* right;
	int height;
} avl_tree_node;

/*
 * Iterative function to insert key and value in subtree with given root
 * Returns new root of subtree or NULL if insert failed.
 */
avl_tree_node* avl_tree_insert(avl_tree_node* root, void* value, int(*cmp_func)(const void*,const void*));

/*
 * Iterative function to remove a node with given key from subtree with
 * given root.
 * Returns root of the modified subtree or NULL if remove failed.
 */
avl_tree_node* avl_tree_remove(avl_tree_node* root, void* value, int(*cmp_func)(const void*,const void*));

/*
 * Iterative function to find a node with given key from tree with
 * given root.
 * Returns found node or NULL if no node was found.
 */
const avl_tree_node* avl_tree_find(const avl_tree_node* root, const void* value, int(*cmp_func)(const void*,const void*));

/*
 * Returns the node with minimum key value found in a tree with given root.
 */
avl_tree_node* avl_tree_min(avl_tree_node* root);

/*
 * Returns the inorder successor of the node or NULL if there is none.
 */
avl_tree_node* avl_tree_next_inorder(avl_tree_node* node);

#endif /* !AVL_TREE_H */