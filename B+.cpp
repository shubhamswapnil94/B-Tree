#include <string>
#include <queue>
#include <iostream>

#include "B+.hpp"

using namespace std;

void BPTree::init(int order) {
    
    this->order = order;

    root = new Leaf();
    tail = head = static_cast<Leaf *>(root);
}   

int BPTree::find_order() {
    return order;
}

void BPTree::insert2(float insert_key, string data_Val) {
    BPNode *curr_node = root;
   
    
    while (curr_node->find_type().compare("LEAFNODE") != 0) {
        
        Inter_Node *new_inter_node = static_cast<Inter_Node *>(curr_node);
        for (vector<float>::iterator i = new_inter_node->keys.begin();
             i < new_inter_node->keys.end(); ++i) {
            if (insert_key < *i) {
                curr_node = new_inter_node->child.at(i - new_inter_node->keys.begin());             
                break;
            }
        }
    }
   
    static_cast<Leaf *>(curr_node)->Insert(insert_key, data_Val);
   
    if (curr_node->num_keys() == order) {
       
        auto split_result = static_cast<Leaf *>(curr_node)->split(order);
        Inter_Node *curr_parent = static_cast<Inter_Node *>(curr_node->par);
        bool needs_new_root = true;
        while (nullptr != curr_parent) {
            curr_parent->merge(split_result);            
            if (curr_parent->num_keys() == order) {
                split_result = curr_parent->brk(order);
                
                curr_node = curr_parent;
                curr_parent = static_cast<Inter_Node *>(curr_node->find_par());
            } else {
                needs_new_root = false;
                break;
            }
        }

        if (needs_new_root) {
            Inter_Node *new_root = split_result.first;
            auto new_root_right_child = split_result.second;
            
            new_root->insert_child(0, curr_node); 
            new_root->insert_child(1, new_root_right_child);
            curr_node->set_parent(new_root);
            new_root_right_child->set_parent(new_root);
            root = new_root;
        }        
    }
}

vector<string> BPTree::search(float key) {
    BPNode *curr_node = root;
    vector<string> search_output_arr;
    
    while (curr_node->find_type().compare("LEAFNODE") != 0) {       
        
        Inter_Node *new_inter_node = static_cast<Inter_Node *>(curr_node);
        for (vector<float>::iterator i = new_inter_node->keys.begin();
             i < new_inter_node->keys.end(); ++i) {
            
            if (key < *i) {
                curr_node = new_inter_node->child.at(i - new_inter_node->keys.begin());
                break;
            }
        }
    }

    Leaf *curr_data_node = static_cast<Leaf *>(curr_node);
    Leaf *curr_data_node_left = curr_data_node->left;

    while (curr_data_node_left != nullptr) {
        for (vector<float>::const_iterator i = curr_data_node_left->keys.begin();
             i < curr_data_node_left->keys.end(); ++i) {
            if (key == *i) {
                search_output_arr.push_back(curr_data_node_left->values.at(i - curr_data_node_left->keys.begin()));
            }
        }
        curr_data_node_left = curr_data_node_left->left;
    }

    while (curr_data_node != nullptr) {
        for (vector<float>::const_iterator i = curr_data_node->keys.begin(); i < curr_data_node->keys.end(); ++i) {
            if (key == *i) {
                search_output_arr.push_back(curr_data_node->values.at(i - curr_data_node->keys.begin()));
            }
        }
        curr_data_node = curr_data_node->right;
    }

    return search_output_arr;
}

/****************
 * Range Search
 ****************/
vector<pair<float, string>> BPTree::search(float key1, float key2) {    
    BPNode *curr_node = root;
    pair<float, string> range_val;
    vector<pair<float, string>> out_Arr;
  
    while (curr_node->find_type().compare("LEAFNODE") != 0) {

        Inter_Node *new_inter_node = static_cast<Inter_Node *>(curr_node);
        for (vector<float>::iterator i = new_inter_node->keys.begin();
             i < new_inter_node->keys.end(); ++i) {
            
            if (key1 < *i) {
                curr_node = new_inter_node->child.at(i - new_inter_node->keys.begin());       
                break;
            }
        }
    }

    Leaf *curr_data_node = static_cast<Leaf *>(curr_node);

    while (curr_data_node != nullptr) {
        for (vector<float>::const_iterator i = curr_data_node->keys.begin(); i < curr_data_node->keys.end(); ++i) {
            if ((*i >= key1) && (*i <= key2)) {
                out_Arr.push_back(
                        make_pair(*i, curr_data_node->values.at(i - curr_data_node->keys.begin())));
            }
        }
        curr_data_node = curr_data_node->right;
    }
    return out_Arr;
}

