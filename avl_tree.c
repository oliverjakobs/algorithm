#include "avl_tree.h"

#include <stdlib.h>

/*
 * Utility function to get height of the tree
 */
static int avl_tree_height(avl_tree_node* n)
{
    return n ? n->height : 0;
}

/*
 * Utility function to update the height of a node
 */
static void avl_tree_update_height(avl_tree_node* n)
{
    if (n)
    {
        int l = avl_tree_height(n->left);
        int r = avl_tree_height(n->right);
        n->height = ((l > r) ? l : r) + 1;
    }
}

/*
 * Get balance of a node
 */
static int avl_tree_get_balance(avl_tree_node* n)
{
    return n ? avl_tree_height(n->left) - avl_tree_height(n->right) : 0;
}

/*
 * A utility function to right rotate subtree rooted with node
 */
static avl_tree_node* avl_tree_right_rotate(avl_tree_node* node)
{
    avl_tree_node* left = node->left;

    /* Perform rotation */
    node->left = left->right;
    left->right = node;

    /* Update parents */
    if (node->left) node->left->parent = node;
    left->parent = node->parent;
    node->parent = left;

    /* Update heights */
    avl_tree_update_height(node);
    avl_tree_update_height(left);

    return left;
}

/*
 * A utility function to left rotate subtree rooted with node
 */
static avl_tree_node* avl_tree_left_rotate(avl_tree_node* node)
{
    avl_tree_node* right = node->right;

    /* Perform rotation */
    node->right = right->left;
    right->left = node;

    /* Update parents */
    if (node->right) node->right->parent = node;
    right->parent = node->parent;
    node->parent = right;

    /* Update heights */
    avl_tree_update_height(node);
    avl_tree_update_height(right);

    return right;
}

static avl_tree_node* avl_tree_node_alloc(void* value, avl_tree_node* parent)
{
    avl_tree_node* node = malloc(sizeof(avl_tree_node));
    if (node)
    {
        node->value = value;
        node->parent = parent;
        node->left = NULL;
        node->right = NULL;
        node->height = 1; /* new node is initially added at leaf */
    }
    return node;
}

static avl_tree_node* avl_tree_rebalance_node(avl_tree_node* node)
{
    int balance = avl_tree_get_balance(node);

    if (balance > 1)
    {
        if (avl_tree_get_balance(node->left) < 0)
            node->left = avl_tree_left_rotate(node->left);

        if (node->parent)
        {
            if (node == node->parent->left)
                node->parent->left = avl_tree_right_rotate(node);
            else
                node->parent->right = avl_tree_right_rotate(node);
        }
        else
        {
            node = avl_tree_right_rotate(node);
        }
    }
    else if (balance < -1)
    {
        if (avl_tree_get_balance(node->right) > 0)
            node->right = avl_tree_right_rotate(node->right);

        if (node->parent)
        {
            if (node == node->parent->left)
                node->parent->left = avl_tree_left_rotate(node);
            else
                node->parent->right = avl_tree_left_rotate(node);
        }
        else
        {
            node = avl_tree_left_rotate(node);
        }
    }

    return node;
}

avl_tree_node* avl_tree_insert(avl_tree_node* root, void* value, int(*cmp_func)(const void*,const void*))
{
    avl_tree_node* node = NULL;
    avl_tree_node* temp = root;

    /* find position to insert */
    while (temp)
    {
        if (cmp_func(value, temp->value) == 0) return NULL; /* key already in tree */

        node = temp;
        if (cmp_func(value, temp->value) < 0)
            temp = temp->left;
        else 
            temp = temp->right;
    }

    /* insert node */
    if (!node)
        return avl_tree_node_alloc(value, NULL);  /* allocate root */
    else if (cmp_func(value, node->value) < 0) 
        node->left = avl_tree_node_alloc(value, node);
    else
        node->right = avl_tree_node_alloc(value, node);

    /* rebalance tree */
    while (node)
    {
        avl_tree_update_height(node);

        temp = avl_tree_rebalance_node(node);
        node = temp->parent;
    }

    root = temp;
    return root;
}

avl_tree_node* avl_tree_remove(avl_tree_node* root, void* value, int(*cmp_func)(const void*,const void*))
{
    avl_tree_node* node = root;

    while (node && cmp_func(node->value, value) != 0)
    {
        if (cmp_func(value, node->value) < 0)
            node = node->left;
        else
            node = node->right;
    }
  
    if (!node) return NULL; /* key not found */

    /* node with only one child or no child */
    if ((node->left == NULL) || (node->right == NULL))
    {
        avl_tree_node* child = node->left ? node->left : node->right;

        /* update parents link to child */
        if (node->parent)
        {
            if (node == node->parent->left)
                node->parent->left = child;
            else
                node->parent->right = child;
        }
        else
        {
            /* node to be removed is root, so update root pointer */
            root = child;
        }

        /* update parent pointer if node has one child */
        if (child) child->parent = node->parent;

        free(node);
        node = child;
    }
    else
    {
        /* node with two children: Get the inorder successor */
        avl_tree_node* temp = avl_tree_min(node->right);
  
        /* Copy the inorder successor's data to this node */
        node->value = temp->value;

        /* 
         * update successor's parents link
         * successor can only have a right child or no child
         */
        if (temp->parent)
        {
            if (temp == temp->parent->left)
                temp->parent->left = temp->right;
            else
                temp->parent->right = temp->right;
        }
        
        free(temp);
    }
    
    /* rebalance tree */
    avl_tree_node* temp = NULL;
    while (node)
    {
        avl_tree_update_height(node);

        temp = avl_tree_rebalance_node(node);
        node = temp->parent;
    }
    
    root = temp;
    return root;
}

const avl_tree_node* avl_tree_find(const avl_tree_node* root, const void* value, int(*cmp_func)(const void*,const void*))
{
    const avl_tree_node* node = root;
    while (node && cmp_func(value, node->value) != 0)
    {
        if (cmp_func(value, node->value) < 0)
            node = node->left;
        else 
            node = node->right;
    }
    return node;
}

avl_tree_node* avl_tree_min(avl_tree_node* root)
{
    if (!root) return NULL;

    avl_tree_node* current = root;

    /* loop down to find the leftmost leaf */
    while (current->left) current = current->left;

    return current;
}

avl_tree_node* avl_tree_next_inorder(avl_tree_node* node)
{
    if (node)
    {
        if (node->right) return avl_tree_min(node->right);

        avl_tree_node* next = node->parent;
        while (next && node == next->right)
        {
            node = next;
            next = next->parent;
        }
        return next;
    }
    return NULL;
}