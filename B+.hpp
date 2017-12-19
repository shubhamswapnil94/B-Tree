#ifndef BPLUS_HPP_
#define BPLUS_HPP_
#include <iostream>
#include <string>
#include "Node.hpp"

using namespace std;

class BPTree {
private:
    int order;
    BPNode *root;
    Leaf *tail;
    Leaf *head;
    

public:
    
    int find_order();
    void init(int order);  
    vector<string> search(float key);
    vector<pair<float, string>> search(float key_start, float key_end);
    void insert2(float key, string value);
};

#endif
