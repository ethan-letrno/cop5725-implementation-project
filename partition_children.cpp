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
//Node * get_potential_tree(Node * u, std::vector<Node*>, Node * g_prime) 

//int partition_children(Node *, int, std::string);


int partition_children(Node * u, int k, std::string filename, Node * g_prime) {
    //Find the best strategy to partition children of node u into at most k subsets
    
    //returns true if it can find a way to optimize u (split children of node u into smaller subsets)
    
    //recursively apply the splitting procedure on each child node of u
    int q = u->children.size();
    if (q <= 1)
        return 0;
    
    double C_min = get_cost(g_prime, filename);
                 
    std::vector< Node * > SS;
    std::vector<Node *> A;
    
    if (k > q)
        k = q;
        
        
    for (int k_prime = 0; k_prime < k; k_prime++) {
        
        A = DivideSubsets(u, k_prime, filename);

        //compute new cost of C prime
        //This needs changed. We don't want the cost of a node u, as this might not be the root.
        //Need to ADD the subsets into the tree and THEN check the cost.
        //and we need to do this without actually changing the original tree
        //new_g_prime = get_potential_tree(u, A, g_prime);
        //c_prime = get_cost(new_g_prime, filename);
        
        
        double C_prime = get_cost(u, filename);
        
        
        //Then, if the cost of the added nodes is better, we remember them. 
        if (C_prime < C_min) {
            C_min = C_prime;
            SS = A;
        }
    }
    
    if (!SS.empty()) {
        /*
        add new nodes and edges from u to new nodes
        add new edges from new nodes to previous children of u
        */
        return 1;
    }
    else
        return 0;
    
}

/*
Node * get_potential_tree(Node * u, std::vector<Node*> A, Node * g_prime) {
        
        
}
*/


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
