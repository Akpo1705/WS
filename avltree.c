#include "module.h"

#define DIRECTION_ROOT 0
#define DIRECTION_LEFT -1
#define DIRECTION_RIGHT 1
#define HEIGHT(p) 	((p==NULL) ? -1 : (((avl_node_t *)(p))->height))
#define MAX(a,b)	((a)>(b)?(a):(b))

int avltree_node_number(avl_tree_t tree)
{
    int    num_l = 0;
    int    num_r = 0;
    
    if (tree == NULL)
    {
        return 0;
    }

    num_l = avltree_node_number(tree->left);
    num_r = avltree_node_number(tree->right);
    return (num_l + num_r + 1);
}

int avltree_leaf_number(avl_tree_t tree)
{
    int    num_l = 0;
    int    num_r = 0;
    
    if (tree == NULL)
    {
        return 0;
    }
    
    if (tree->left == NULL && tree->right == NULL)
    {
        return 1;
    }
    
    num_l = avltree_leaf_number(tree->left);   
    num_r = avltree_leaf_number(tree->right);  
    return (num_l + num_r);
}


int avltree_height(avl_tree_t tree)
{
    return HEIGHT(tree);
}

void avltree_traverse_preorder(avl_tree_t tree)
{
    if (tree != NULL)
    {
        printf("%d ", tree->key);
        avltree_traverse_preorder(tree->left);
        avltree_traverse_preorder(tree->right);
    }
}

void avltree_traverse_inorder(avl_tree_t tree)
{
    if (tree != NULL)
    {
        avltree_traverse_inorder(tree->left);
        printf("%d ", tree->key);
        avltree_traverse_inorder(tree->right);
    }
}

void avltree_traverse_postorder(avl_tree_t tree)
{
    if (tree != NULL)
    {
        avltree_traverse_postorder(tree->left);
        avltree_traverse_postorder(tree->right);
        printf("%d ", tree->key);
    }
}

#ifdef RECURSIVE_ALGO
avl_node_t* avltree_search(avl_tree_t tree, avl_type_t key)
{
    if (tree == NULL || tree->key == key)
    {
        return tree;
    }

    if (key < tree->key)
    {
        return avltree_search(tree->left, key);
    }
    else
    {
        return avltree_search(tree->right, key);
    }
}
#else
avl_node_t* avltree_search(avl_tree_t tree, avl_type_t key)
{
    while ((tree != NULL) && (tree->key != key))
    {
        if (key < tree->key)
        {
            tree = tree->left;
        }
        else
        {
            tree = tree->right;
        }
    }

    return tree;
}
#endif

avl_node_t* avltree_minimum(avl_tree_t tree)
{
    if (tree == NULL)
    {
        return NULL;
    }

    while(tree->left != NULL)
    {
        tree = tree->left;
    }
    
    return tree;
}
 
avl_node_t* avltree_maximum(avl_tree_t tree)
{
    if (tree == NULL)
    {
        return NULL;
    }

    while(tree->right != NULL)
    {
        tree = tree->right;
    }
    
    return tree;
}


static avl_node_t* avltree_rotate_right(avl_tree_t tree)
{
    avl_tree_t sub_left;

    sub_left = tree->left;
    tree->left = sub_left->right;
    sub_left->right = tree;

    tree->height = MAX(HEIGHT(tree->left), HEIGHT(tree->right)) + 1;
    sub_left->height = MAX(HEIGHT(sub_left->left), tree->height) + 1;

    return sub_left;
}

static avl_node_t* avltree_rotate_left(avl_tree_t tree)
{
    avl_tree_t sub_right;

    sub_right = tree->right;
    tree->right = sub_right->left;
    sub_right->left = tree;

    tree->height = MAX(HEIGHT(tree->left), HEIGHT(tree->right)) + 1;
    sub_right->height = MAX(HEIGHT(sub_right->right), tree->height) + 1;

    return sub_right;
}

static avl_node_t* avltree_rotate_left_right(avl_tree_t tree)
{
    tree->left = avltree_rotate_left(tree->left);

    return avltree_rotate_right(tree);
}

static avl_node_t* avltree_rotate_right_left(avl_tree_t tree)
{
    tree->right = avltree_rotate_right(tree->right);

    return avltree_rotate_left(tree);
}

static avl_node_t* avltree_create_node(avl_type_t key, avl_node_t *left, avl_node_t* right)
{
    avl_node_t *p;

    if ((p = (avl_node_t *)malloc(sizeof(avl_node_t))) == NULL)
    {
        return NULL;
    }
    
    p->count_sr = 0;
    p->len_sr = 0;
    p->sr = NULL;
    p->key = key;
    p->height = 0;
    p->left = left;
    p->right = right;
    return p;
}

avl_node_t* avltree_insert(avl_tree_t tree, avl_type_t key)
{
    if (tree == NULL)
    {
        tree = avltree_create_node(key, NULL, NULL); 
        if (tree == NULL)
        {
            printf("ERROR: avltree_create_node failed.\n");
            return NULL;
        }
    }
    else if (key < tree->key)
    {
        tree->left = avltree_insert(tree->left, key);

        if (HEIGHT(tree->left) - HEIGHT(tree->right) == 2)
        {
            if (key < tree->left->key)
            {
                tree = avltree_rotate_right(tree);
            }
            else
            {
                tree = avltree_rotate_left_right(tree);
            }
        }
    }
    else if (key > tree->key) 
    {
        tree->right = avltree_insert(tree->right, key);

        if (HEIGHT(tree->right) - HEIGHT(tree->left) == 2)
        {
            if (key > tree->right->key)
            {
                tree = avltree_rotate_left(tree);
            }
            else
            {
                tree = avltree_rotate_right_left(tree);
            }
        }
    }
    else //key == tree->key
    {
        printf("ERROR: key is already exist.\n");
    }

    tree->height = MAX(HEIGHT(tree->left), HEIGHT(tree->right)) + 1;
    return tree;
}

static avl_node_t* avltree_delete_node(avl_tree_t tree, avl_node_t *z)
{
    if (tree == NULL || z == NULL)
    {
        return NULL;
    }

    if (z->key < tree->key) 
    {
        tree->left = avltree_delete_node(tree->left, z);

        if (HEIGHT(tree->right) - HEIGHT(tree->left) == 2)
        {
            avl_node_t *r = tree->right;
            if (HEIGHT(r->left) > HEIGHT(r->right))
            {
                tree = avltree_rotate_right_left(tree);
            }
            else
            {
                tree = avltree_rotate_left(tree);
            }
        }
    }
    else if (z->key > tree->key)
    {
        tree->right = avltree_delete_node(tree->right, z);

        if (HEIGHT(tree->left) - HEIGHT(tree->right) == 2)
        {
            avl_node_t *l = tree->left;
            if (HEIGHT(l->right) > HEIGHT(l->left))
            {
                tree = avltree_rotate_left_right(tree);
            }
            else
            {
                tree = avltree_rotate_right(tree);
            }
        }
    }
    else
    {
        if (tree->left != NULL && tree->right != NULL)
        {
            if (HEIGHT(tree->left) > HEIGHT(tree->right))
            {
                avl_node_t *max = avltree_maximum(tree->left);
                tree->key = max->key;
                tree->left = avltree_delete_node(tree->left, max);
            }
            else
            {
                avl_node_t *min = avltree_minimum(tree->right);
                tree->key = min->key;
                tree->right = avltree_delete_node(tree->right, min);
            }
        }
        else
        {
            avl_node_t *tmp = tree;
            tree = tree->left ? tree->left : tree->right;
            free(tmp);
        }
    }

    return tree;
}

avl_node_t* avltree_delete(avl_tree_t tree, avl_type_t key)
{
    avl_node_t *z; 

    if ((z = avltree_search(tree, key)) != NULL)
    {
        tree = avltree_delete_node(tree, z);
    }
    
    return tree;
}

void avltree_destory(avl_tree_t tree)
{
	if (tree == NULL)
	{
		return;
	}
	
	if (tree->left != NULL)
	{
		avltree_destory(tree->left);
	}
	
	if (tree->right != NULL)
	{
		avltree_destory(tree->right);
	}
	
	int i;
	for(i=0;i<tree->count_sr;i++)
		free(tree->sr[i]);
	
	free(tree->sr);
	free(tree);
}

int sr_realloc(avl_tree_t avl_tree, int size)
{
	avl_tree->sr = (struct TABLE_SR**)realloc(avl_tree->sr,size * sizeof(struct TABLE_SR*));
	if(avl_tree)
		avl_tree->len_sr++;
	else
		return 0;
	
	return 1;
}

int avltree_add_data(avl_tree_t avl_tree, tbl_sr_t tbl_sr)
{
	if(sr_realloc(avl_tree, avl_tree->len_sr + 1))
	{
		avl_tree->sr[avl_tree->count_sr] = tbl_sr;
		avl_tree->count_sr++;
	
		return 1;
	}
	return 0;
}

void avltree_show(avl_tree_t tree, avl_type_t key, int direction)
{
    if (tree != NULL)
    {
        if (direction == DIRECTION_ROOT)
        {
            printf("----------------------------------\n");
            printf("tree height: %d\n", avltree_height(tree));
            printf("tree nodes : %d\n", avltree_node_number(tree));
            printf("leaf nodes : %d\n", avltree_leaf_number(tree));
            printf("----------------------------------\n");
            printf("(%2d) root\n", key);
        }
        else if (direction == DIRECTION_RIGHT)
        {
            printf("(%2d) ---> (%2d)\n", key, tree->key);
        }
        else
        {
            printf("(%2d) <--- (%2d)\n", tree->key, key);
        }

        avltree_show(tree->left, tree->key, DIRECTION_LEFT);
        avltree_show(tree->right, tree->key, DIRECTION_RIGHT);
    }
}
