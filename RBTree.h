#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct node {
    node* left;
    node* right;
    node* parent;
    bool color;
    int key;
    node(bool c, int k)
    : color(c)
    , key(k)
    , left(nullptr)
    , right(nullptr)
    , parent(nullptr) {
    }
};

class RB_Tree {
 private:
    node* root;
    int num;

    void rotate_left(node* pt) {
        node* nd = pt->right;
        pt->right = nd->left;
        if (pt->right != nullptr) {
            pt->right->parent = pt;
        }
        nd->parent = pt->parent;
        if (pt->parent == nullptr) {
            root = nd;
        } else if (pt == pt->parent->left) {
            pt->parent->left = nd;
        } else {
            pt->parent->right = nd;
        }
        nd->left = pt;
        pt->parent = nd;
        return;
    }

    void rotate_right(node* pt) {
        node* nd = pt->left;
        pt->left = nd->right;
        if (pt->left != nullptr) {
            pt->left->parent = pt;
        }
        nd->parent = pt->parent;
        if (pt->parent == nullptr) {
            root = nd;
        } else if (pt == pt->parent->left) {
            pt->parent->left = nd;
        } else {
            pt->parent->right = nd;
        }
        nd->right = pt;
        pt->parent = nd;
        return;
    }

    node* search(int key) {
        node* pt = root;
        while (pt != nullptr && key != pt->key) {
            if (key < pt->key) {
                pt = pt->left;
            } else {
                pt = pt->right;
            }
        }
        return pt;
    }

    node* insert_f(node* root, node* pt) {
        if (root == nullptr) {
            return pt;
        }
        if (pt->key < root->key) {
            root->left = insert_f(root->left, pt);
            root->left->parent = root;
        } else if (pt->key > root->key) {
            root->right = insert_f(root->right, pt);
            root->right->parent = root;
        }
        return root;
    }

    void fix_insert(node *pt) {
        node* par = nullptr;
        node* gpar = nullptr;
        while (pt != root && get_color(pt) == 1 && get_color(pt->parent) == 1) {
            par = pt->parent;
            gpar = par->parent;
            if (par == gpar->left) {
                node* uncle = gpar->right;
                if (get_color(uncle) == 1) {
                    set_color(uncle, 0);
                    set_color(gpar, 1);
                    set_color(par, 0);
                    pt = gpar;
                } else {
                    if (pt == par->right) {
                        rotate_left(par);
                        pt = par;
                        par = pt->parent;
                    }
                    rotate_right(gpar);
                    swap(par->color, gpar->color);
                    pt = par;
                }
            } else {
                node* uncle = gpar->left;
                if (get_color(uncle) == 1) {
                    set_color(uncle, 0);
                    set_color(gpar, 1);
                    set_color(par, 0);
                    pt = gpar;
                } else {
                    if (pt == par->left) {
                        rotate_right(par);
                        pt = par;
                        par = pt->parent;
                    }
                    rotate_left(gpar);
                    swap(par->color, gpar->color);
                    pt = par;
                }
            }
        }
        set_color(root, 0);
    }

    void print_f(node* pt) {
        if (pt == nullptr) {
            return;
        }
        print_f(pt->left);
        cout << pt->key << " ";
        if (pt->left == nullptr) {
            cout << "null ";
        } else {
            cout << pt->left->key << " ";
        }
        if (pt->right == nullptr) {
            cout << "null ";
        } else {
            cout << pt->right->key << " ";
        }
        if (get_color(pt)) {
            cout << "R\n";
        } else {
            cout << "B\n";
        }
        print_f(pt->right);
    }

    int get_color(node* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->color;
    }

    void set_color(node* node, bool color) {
        if (node == nullptr) {
            return;
        }
        node->color = color;
    }

 public:
    RB_Tree() {
        root = nullptr;
        num = 0;
    }

    void insert(int key) {
        if (search(key) != nullptr) {
            return;
        }
        node* nd = new node(1, key);
        root = insert_f(root, nd);
        fix_insert(nd);
        num++;
        return;
    }

    void print() {
        print_f(root);
        return;
    }

    node* find(int k) {
        return search(k);
    }

    void elem_n() {
        cout << num << "\n";
    }
};