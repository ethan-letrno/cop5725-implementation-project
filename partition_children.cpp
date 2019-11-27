#include <fstream>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <stdlib.h>
#include <set>
#include <ctime>
#include <bits/stdc++.h> 
#include <math.h>
#include "utils.h"
#include "divide_subsets.h"



double get_cost(Node *, std::string);
//int partition_children(Node *, int, std::string);


int partition_children(Node * u, int k, std::string filename) {
    //Find the best strategy to partition children of node u into at most k subsets
    
    //returns true if it can find a way to optimize u (split children of node u into smaller subsets)
    
    //recursively apply the splitting procedure on each child node of u
    int q = u->children.size();
    if (q <= 1)
        return 0;
    
    double C_min = get_cost(u, filename);
    std::cout << "The current cost of the tree is: " << C_min << std::endl;
                 
    std::vector< Node * > SS;
    
    if (k > q)
        k = q;
        
    for (int k_prime = 0; k_prime < k; k_prime++) {
        DivideSubsets(u, k_prime, filename);
        
        //compute new cost of C prime
        double C_prime = get_cost(u, filename);
        
        if (C_prime < C_min) {
            C_min = C_prime;
            SS.push_back(u);
        }
    }
    
    if (!SS.empty()) {
        /*
        update G prime according to SS 
        remove edges from u to children
        add new nodes and edges from u to new nodes
        add new edges from new nodes to previous children of u
        */
        for (int i = 0; i < SS.size(); i++)
            std::cout << SS[i]->group[0] << std::endl;
        return 1;
    }
    else
        return 0;
    
}

double get_cost(Node * root, std::string file) {
    //Gets the cost of each edge
    double total_cost = 0;
    
    //For each child of root
    for (int i = 0; i < root->children.size(); i++) {
        //If it's the first child, it is the one scan cost for this node
        if (i == 0)
            total_cost += scanCost(root->children[i]->group, file);
        else //else you must sort the node
            total_cost += sortCost(root->children[i]->group, file);
            
        //std::cout << "Current Total cost of the tree is: " << total_cost << std::endl;
            
        //Get the cost of this child's children
        total_cost += get_cost(root->children[i], file);
    }
    
    
    return total_cost;
}
