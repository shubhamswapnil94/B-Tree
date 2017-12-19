#include <iostream>
#include <fstream>
#include "B+.hpp"

using namespace std;

int main(int argc, char **argv) {
    string inp_file = "input.txt";
    if (argc > 1) {    
        inp_file = argv[1];
    }
    
    ifstream inp(inp_file);
//if input cannot be opened    
    if (!inp) {
        
        cerr << "ERROR! " << inp_file << " could not be opened for reading!" << endl;
        exit(1);
    }
    
    ofstream out_file("output_file.txt");
    
    if (!out_file) {
    
        cerr << "ERROR! output_file.txt could not be opened for writing!" << endl;
        exit(1);
    }

    string order;
    getline(inp, order);

    auto btree = new BPTree();
    btree->init(atoi(order.c_str()));

    int curr_line = 0;
    
    while (inp) {
        if (curr_line == 0) {
            curr_line++;
        } else {
            
            string str_inp;
            inp >> str_inp;
            string delim1 = "(";
            string delim2 = ",";
            string delim3 = ")";

            size_t position = str_inp.find(delim1);                   
            string instruct = str_inp.substr(0, position);              
            str_inp.erase(0, position + delim1.length());            

            string input1, input2; 
            if (instruct == "Insert") 
             {
                position = str_inp.find(delim2);                
                input1 = str_inp.substr(0, position);                
                str_inp.erase(0, position + delim2.length());               

                position = str_inp.find(delim3);
                input2 = str_inp.substr(0, position);
                str_inp.erase(0, position + delim3.length());
              
                float key = atof(input1.c_str());                
                btree->insert2(key, input2);
            }    
            else{   
                if (str_inp.find(delim2) > (size_t) 1000) {
                    position = str_inp.find(delim3);
                    input1 = str_inp.substr(0, position);                   
                    str_inp.erase(0, position + delim3.length());
                } else {
                                  
                    position = str_inp.find(delim2);                
                    input1 = str_inp.substr(0, position);                    
                    str_inp.erase(0, position + delim2.length());                    

                    position = str_inp.find(delim3);
                    input2 = str_inp.substr(0, position);                
                    str_inp.erase(0, position + delim3.length());
                }
                float k1 = atof(input1.c_str());
                if (input2 == "") {
                    vector<string> values = btree->search(k1);
                    if (values.empty()) {
                        out_file << "NULL" << endl;
                    } else {
                        for (vector<string>::const_iterator i = values.begin(); i < values.end(); ++i) {
                            out_file << *i;
                            if (i < values.end() - 1)
                                out_file << ",";
                            else
                                out_file << endl;                     
                        }
                    }
                } else {
                    float k2 = atof(input2.c_str());
                    vector<pair<float, string>> values = btree->search(k1, k2);
                    if (values.empty()) {
                        out_file << "NULL" << endl;
                    } else {
                        for (vector<pair<float, string>>::const_iterator i = values.begin();
                             i < values.end(); ++i) {
                            out_file << "(" << values[i - values.begin()].first << "," << values[i - values.begin()].second
                                 << ")";
                            if (i < values.end() - 1)
                                out_file << ",";
                            else
                                out_file << endl;
              
                        }
                    }
                }
            }             
        }
    }

   
  
    return 0;
}
