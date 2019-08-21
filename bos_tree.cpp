#include<bits/stdc++.h>
using namespace std;

struct node {
  int key;
  int size;
  int height;
  node *left;
  node *right;
  node *parent;
};

node* new_node(int key) {
  node * leaf = new node;
  leaf -> key = key;
  leaf -> size = 1;
  leaf -> height = 1;
  leaf -> left = NULL;
  leaf -> right = NULL;
  return leaf;
}

class bos_tree {
  public:
    bos_tree();
    ~bos_tree();

    // inserts value to the tree
    void insert(int value);
    
    // removes node. returns true if node was present else false
    bool remove(int value);
    
    // return node if found else NULL
    node* search(int value);
    
    // return last node smaller than or equal to value
    node* lower_bound(int value);
    
    // get rank of value. If value is not in the  tree, return 
    // rank of lower bound
    int rank(int value);

    // remove tree to free memory;
    void remove_tree(node * leaf);


    node* succesor(node * leaf);

    node* minima();

    int size();

    // print tree
    void print();


  private:
    node * root;
    void fix_tree(node * leaf);
    void fix_tree_node(node * leaf);
    void shift_right(node * leaf);
    node * search(int value, node * current_lb, node * leaf);
    void fix_height_insert(node * grandparent, node * parent, node * child);
    void fix_height_delete(node * grandparent);
    void rotate_left(node * leaf);
    void rotate_right(node * leaf);
    bool is_left_child(node *leaf);
    bool is_right_child(node *leaf);
    void print(node * leaf, int ht);
};

bos_tree::bos_tree() {
  root = NULL;
}


void bos_tree::remove_tree(node * leaf) {
  if (leaf != NULL){
    remove_tree(leaf->left);
    remove_tree(leaf->right);
    delete leaf;
  }
}

bos_tree::~bos_tree(){
  remove_tree(root);
}

int bos_tree::size() {
  if(root == NULL) {
    return 0;
  }
  return root-> size;
}

node* bos_tree::minima() {
  node* res = root;
  if(res==NULL){
    return res;
  }
  while(res -> left != NULL) {
    res = res -> left;
  }
  return res;
}

bool bos_tree::is_left_child(node *leaf) {
  if (leaf != root && leaf != NULL && 
  leaf -> parent -> left != NULL && leaf -> parent -> left == leaf) {
    return true;
  }
  return false;
}

bool bos_tree::is_right_child(node *leaf) {
  if (leaf != root && leaf != NULL && 
  leaf -> parent -> right != NULL && leaf -> parent -> right == leaf) {
    return true;
  }
  return false;
}

node* bos_tree::search(int value, node * current_lb, node * leaf) {
  if (leaf==NULL){
    return current_lb;
  }
  if (leaf -> key <= value) {
    return search(value, leaf, leaf -> right);
  } else {
    return search(value, current_lb, leaf -> left);
  }
}

node * bos_tree::lower_bound(int value) {
  if (root==NULL){
    return root;
  }
  if (root-> key <= value) {
    return search(value, root, root -> right);
  } else {
    return search(value, NULL, root->left);
  }
}

node * bos_tree::search(int value) {
  node * res = lower_bound(value);
  if (res -> key == value) {
    return res;
  }
  return NULL;
}

int bos_tree::rank(int value) {
  node* res = lower_bound(value);
  if (res == NULL) {
    return 0;
  }

  int count = 1;

  if(res -> left != NULL) {
    count += res -> left -> size;
  }

  while (res != NULL){
    if (is_right_child(res)) {
      count += (res -> parent -> left == NULL)? 0: res -> parent -> left -> size;
      count ++;
    }
    res = res -> parent;
  } 
  return count;
}

void bos_tree::fix_tree_node(node * leaf) {
  int height = 0;
  int size =  1;

  if (leaf -> left != NULL) {
    height = (leaf -> left -> height > height)? leaf -> left -> height: height;
    size += leaf -> left -> size;
  }
  if (leaf -> right != NULL) {
    height = (leaf -> right -> height > height)? leaf -> right -> height: height;
    size += leaf -> right -> size;
  }

  leaf -> height = height + 1;
  leaf -> size   = size;

  return;
}

void bos_tree::fix_tree(node * leaf) {
  fix_tree_node(leaf);
  if (leaf -> parent != NULL) 
    fix_tree(leaf->parent);
}

void bos_tree::shift_right(node * leaf){
  node *res = leaf;
  if(res->left!=NULL && res->right!=NULL) {
    node *curr, *parent;
    parent = res;
    curr = res -> right;
    while (curr != NULL) {
      parent = curr;
      curr = curr -> left;
    }
    parent -> left = res -> left;
    res -> left -> parent = parent;
    while(parent != res) {
      parent -> size += res -> left -> size; 
      parent = parent -> parent;
    }
  } else if (res -> left != NULL) {
    res -> right = res -> left;
  }
}

node* bos_tree::succesor(node * leaf) {
  if(leaf -> right != NULL) {
    leaf = leaf -> right;
    while(leaf -> left != NULL) {
      leaf = leaf -> left;
    }
    return leaf;
  }

  while(leaf != NULL && is_right_child(leaf)){
    leaf = leaf -> parent;
  }

  return leaf->parent;
}

void bos_tree::rotate_right(node * leaf) {
  node *left, *parent;
  left =  leaf -> left;
  parent = leaf -> parent;

  if (leaf == root) {
    root = left;
  } else if (is_left_child(leaf)){
    parent -> left = left;
  } else {
    parent -> right = left;
  }

  leaf -> parent = left;
  leaf -> left   = left -> right;
  if (left -> right != NULL) {
    left -> right -> parent = leaf;
  }
  left -> right  = leaf;

  left -> parent = parent;

  fix_tree_node(leaf);
  fix_tree_node(left);
}

void bos_tree::rotate_left(node * leaf) {
  node *right, *parent;
  right =  leaf -> right;
  parent = leaf -> parent;

  if (leaf == root) {
    root = right;
  } else if (is_left_child(leaf)){
    parent -> left = right;
  } else {
    parent -> right = right;
  }

  leaf -> parent = right;
  leaf -> right   = right -> left;
  if (right -> left != NULL){
    right -> left -> parent = leaf;
  }
  right -> left  = leaf;

  right -> parent = parent;

  fix_tree_node(leaf);
  fix_tree_node(right);
}

void bos_tree::fix_height_insert(node * grandparent, node * parent, node * child) {
  if (grandparent == NULL) {
    return;
  }

  int hl, hr;
  hl = 0;
  hr = 0;
  if(grandparent -> left != NULL) {
    hl = grandparent -> left -> height;
  }
  if(grandparent -> right != NULL) {
    hr =  grandparent -> right -> height;
  }

  if ((hl-hr)*(hl-hr) <= 1) {
    fix_tree_node(grandparent);
    fix_height_insert(grandparent->parent, grandparent, parent);
    return;
  }

  if(is_left_child(child) && is_left_child(parent)) {
    rotate_right(grandparent);
    fix_height_insert(parent ->parent, parent, child);
  } else if(is_left_child(child) && is_right_child(parent)) {
    rotate_right(parent);
    rotate_left(grandparent);
    fix_height_insert(child -> parent, child, parent);
  } else if (is_right_child(child) && is_left_child(parent)) {
    rotate_left(parent);
    rotate_right(grandparent);
    fix_height_insert(child -> parent, child, parent);
  } else {
    rotate_left(grandparent);
    fix_height_insert(parent ->parent, parent, child);
  }
}

void bos_tree::insert(int value) {
  if (root == NULL) {
    root = new_node(value);
    root -> parent = NULL; 
    return;
  }
  node *curr, *parent, *grandparent;
  curr = root;
  parent = NULL;
  
  while(1) {
    if (curr == NULL) {
      curr = new_node(value);
      curr -> parent = parent;
      if(parent -> key <= value) {
        parent -> right = curr;
      } else {
        parent -> left = curr;
      }
      fix_tree_node(parent);
      grandparent = parent -> parent; 
      fix_height_insert(grandparent, parent, curr);
      return;
    }
    if (curr -> key <= value){
      parent = curr;
      curr = curr -> right;
    } else {
      parent = curr;
      curr = curr -> left;
    }
  }
}

void bos_tree::fix_height_delete(node *grandparent) { 
  if (grandparent == NULL) {
    return;
  }
  node *parent, *child;
  int hl, hr;
  hl = 0;
  hr = 0;
  if(grandparent -> left != NULL) {
    hl = grandparent -> left -> height;
  }
  if(grandparent -> right != NULL) {
    hr =  grandparent -> right -> height;
  }

  if ((hl-hr)*(hl-hr) <= 1) {
    fix_tree_node(grandparent);
    fix_height_delete(grandparent->parent);
    return;
  }
  parent = (hl>hr)?grandparent->left:grandparent->right;
  hl = 0;
  hr = 0;
  if(parent -> left != NULL) {
    hl = parent -> left -> height;
  }
  if(parent -> right != NULL) {
    hr =  parent -> right -> height;
  }
  child = (hl>hr)?parent->left:parent->right;
  if(is_left_child(child) && is_left_child(parent)) {
    rotate_right(grandparent);
    fix_height_delete(parent ->parent);
  } else if(is_left_child(child) && is_right_child(parent)) {
    rotate_right(parent);
    rotate_left(grandparent);
    fix_height_delete(child -> parent);
  } else if (is_right_child(child) && is_left_child(parent)) {
    rotate_left(parent);
    rotate_right(grandparent);
    fix_height_delete(child -> parent);
  } else {
    rotate_left(grandparent);
    fix_height_delete(parent ->parent);
  }
}

bool bos_tree::remove(int value) {
  node *res = lower_bound(value);
  if (res == NULL || res -> key != value) {
    return (1==0);
  }

  if(res -> parent!= NULL)
  if (res -> left != NULL && res -> right != NULL) {
    node *succ = succesor(res);
    if (succ != NULL){
      res -> key = succ -> key;
      if (is_left_child(succ)){
        succ -> parent -> left = succ -> right;
      } else {
        succ -> parent -> right = succ -> right;
      }
      if (succ -> right != NULL){
        succ -> right -> parent = succ -> parent;
      }
      fix_tree(succ->parent);
      fix_height_delete(succ->parent);
      delete succ;
      return (1==1);
    }
  }
  
  shift_right(res);
  if (res==root) {
    root = res -> right;
    if(root != NULL){
      root -> parent = NULL;
    }
    delete res;
    return (1==1);
  }

  node *parent = res -> parent;
  if(parent->left==res) {
    parent -> left = res -> right;
  } else {
    parent -> right = res -> right;
  }
  if (res -> right != NULL){
    res -> right -> parent = parent;
  }
  fix_tree(parent);
  fix_height_delete(parent);
  delete res;

  return (1==1);
}

void bos_tree::print() {
  print(root, 0);
}

void bos_tree::print(node * leaf, int ht) {
  if(leaf==NULL){
    return;
  }
  print(leaf->right,ht+1);
  for(int i=0;i<ht;i++){
    cout<<'\t';
  }
  if(leaf != root) {
    if(is_left_child(leaf)) {
      cout << '\\';
    } else {
      cout<<'/';
    }
  }
  cout<<"("<<leaf->key<<","<<leaf->height<<","<<rank(leaf->key)<<")"<<endl;
  print(leaf->left, ht+1);
}

