#ifndef NODE_HPP_
#define NODE_HPP_

#include <string>
#include <vector>
#include <cmath>
#include <utility>
#include <cfloat>
#include <iostream>

using namespace std;

class BPNode {
protected:
    BPNode *par = nullptr;
    vector<float> keys = {FLT_MAX};    

public:
    
    virtual BPNode *find_par() { return par; }
    virtual void set_parent(BPNode *node) { par = node; }
    virtual string find_type() = 0;    
    int num_keys() 
    { return keys.size() - 1; }

    friend class BPTree;
};

class Inter_Node : public BPNode {
protected:
    vector<BPNode *> child;
    Inter_Node *par = nullptr;

public:
    virtual string find_type() 
    { return "INTERNAL"; }

    int insert_K(float key) {
        for (vector<float>::iterator i = keys.begin(); i < keys.end(); ++i) {
            if (key < *i) {                
                i = keys.insert(i, key);
                return i - keys.begin();
            }
        }
    }

    void merge(pair<Inter_Node *, BPNode *> split_res) {
        auto merge_with = split_res.first;
        auto its_ch = split_res.second;

        int k_pos = insert_K(*merge_with->keys.begin());
        insert_child(k_pos + 1, its_ch);
        its_ch->set_parent(this);
    }

    pair<Inter_Node *, BPNode *> brk(int order) {
        
        int n_left = (keys.begin() + (ceil(float(order) / 2) - 1)) - keys.begin();

        Inter_Node *n_right = new Inter_Node();
        n_right->keys = vector<float>(keys.begin() + n_left + 1, keys.end());
        n_right->child = vector<BPNode *>(child.begin() + n_left + 1, child.end());
        
        for (auto child: n_right->child) {
            child->set_parent(n_right);
        }
       
        Inter_Node *new_mid = new Inter_Node();
        new_mid->insert_K(*(keys.begin() + n_left));

        keys.resize(n_left);
        keys.push_back(FLT_MAX);
        child.resize(n_left + 1);      
        n_right->par = new_mid;

        return pair<Inter_Node *, Inter_Node *>(new_mid, n_right);
    }
    void insert_child(int pos, BPNode *child_ptr) {
        child.insert(child.begin() + pos, child_ptr);
    }
    friend class BPTree;
};

class Leaf : public BPNode {
private:
    vector<string> values = {"END_MARKER"};
    Leaf *left = nullptr;
    Leaf *right = nullptr;

public:
    virtual string find_type() 
    { return "LEAFNODE"; }

    void Insert(float key, string value) {
        for (vector<float>::iterator i = keys.begin(); i < keys.end(); ++i) {
            if (key < *i) {               
                i = keys.insert(i, key);               
                values.insert(values.begin() + (i - keys.begin()), value);                
                break;
            }
        }
    }
    
    pair<Inter_Node *, BPNode *> split(int order) {
               
        vector<float>::iterator olddata_key_end = keys.begin() + ceil(float(order) / 2) - 2;
        vector<string>::iterator olddata_values_end = values.begin() + ceil(float(order) / 2) - 2;
        
        Leaf *new_datanode = new Leaf();
        new_datanode->keys = vector<float>(olddata_key_end + 1, keys.end());      
        new_datanode->values = vector<string>(olddata_values_end + 1, values.end());

        keys.resize(olddata_key_end - keys.begin() + 1);
        keys.push_back(FLT_MAX);
        values.resize(olddata_key_end - keys.begin() + 1);
        values.push_back("END_MARKER");
       
        new_datanode->right = right;
        new_datanode->left = this;
        right = new_datanode;

        Inter_Node *new_in = new Inter_Node();
        new_in->insert_K(*new_datanode->keys.begin());

        return pair<Inter_Node *, Leaf *>(new_in, new_datanode);
    }

    friend class BPTree;
    friend class Inter_Node;
};

#endif
