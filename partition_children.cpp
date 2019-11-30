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
void get_copy(Node *, Node *);
void add_subsets(Node *, std::vector<Node*>, std::string, Node *);

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
        
        
    for (int k_prime = 1; k_prime <= k; k_prime++) {
        
        A = DivideSubsets(u, k_prime, filename);

        //compute new cost of C prime
        //This needs changed. We don't want the cost of a node u, as this might not be the root.
        //Need to ADD the subsets into the tree and THEN check the cost.
        //and we need to do this without actually changing the original tree
        //new_g_prime = get_potential_tree(u, A, g_prime);
        //c_prime = get_cost(new_g_prime, filename);
        
        Node * new_g_prime = newNode(g_prime->group);
        get_copy(g_prime, new_g_prime);
        
        //now add subsets to new_g_prime
        //std::cout << std::endl << std::endl << std::endl;
        add_subsets(new_g_prime, A, filename, u);
        
        //std::cout << "NEW TREE-------------------" << std::endl;
        //preord(new_g_prime);
        
        //and compute the  new cost
        double C_prime = get_cost(new_g_prime, filename);
        
        //std::cout << C_prime << std::endl; 
        
        
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
        add_subsets(g_prime, SS, filename, u);
        return 1;
    }
    else
        return 0;
    
}


void get_copy(Node * g_prime, Node * newRoot) {
    //create copy of g_prime root
    //if(g_prime->group.size() == 15) 
    //    Node * newTree = newNode(g_prime->group);
    
    for(int i = 0; i < g_prime->children.size(); i++) {
        Node * newChild = newNode(g_prime->children[i]->group);
        newRoot->children.push_back(newChild);
        
        get_copy(g_prime->children[i], newRoot->children[i]);
        
    }
    
    //return newRoot;?
}

void add_subsets(Node * root, std::vector<Node*> A, std::string filename, Node * u) {
    //sort the subset
    
    for(int i = 0; i < root->children.size(); i++) {
        if (u->group == root->group) {

            std::vector<std::pair<std::vector<int>,double>> cardinalities;
        
            //Getting the cardinalities of each of u's child nodes
            //std::cout << A.size() << std::endl;
            for(int x = 0; x < A.size(); x++){
                cardinalities.push_back(std::make_pair(A[x]->group, estimateCardinality(A[x]->group, filename)));
                //std::cout << estimateCardinality(A[x]->group, filename) << std::endl;
            }
        
            std::sort(cardinalities.begin(), cardinalities.end(), sortbysecdesc);
            
            std::vector<int> unioned_subset;
            for(int x = 0; x < cardinalities.size(); x++){
                for (int j = 0; j < cardinalities[x].first.size(); j++)
                    unioned_subset.push_back(cardinalities[x].first[j]);
            }
            
            Node * union_node = newNode(unioned_subset);
            
            //add the node to the tree
            /*std::cout << "Root children: " << std::endl;
            for (int i = 0; i < root->children.size(); i++) {
                for(int j = 0; j < root->children[i]->group.size(); j++) {
                    std::cout << root->children[i]->group[j] << std::endl;
                }
            }*/
            
            union_node->children = root->children;
            
            /*std::cout << "Union children: " << std::endl;
            for (int i = 0; i < union_node->children.size(); i++) {
                for(int j = 0; j < union_node->children[i]->group.size(); j++) {
                    std::cout << union_node->children[i]->group[j] << std::endl;
                }
            } */
            
            
            root->children.clear();
            root->children.push_back(union_node);
        }
        else
            add_subsets(root->children[i], A, filename, u);
            
    }
    
}





double get_cost(Node * root, std::string file) {
    //I was previously adding up the cost of the children
    //But that doesn't make sense, right?
    //If we compute T->AB, the main cost would be going through the table T
    //So if T has three kids, then we would go through the table 3 times
    
    //So if you have T->AB, and AB->A,B, then we would only have to go through table T once
    // and the rest of the cost would be on AB for computing A, B
    // so I'm getting the cost of the PARENTS, not the children
    
    
    //Gets the cost of each edge
    double total_cost = 0;
    
    //For each child of root
    for (int i = 0; i < root->children.size(); i++) {
        //If it's the first child, it is the one scan cost for this node
        if (i == 0)
            total_cost += scanCost(root->group, file);
        else //else you must sort the node
            total_cost += sortCost(root->group, file);
            
        //std::cout << "Current Total cost of the tree is: " << total_cost << std::endl;
            
        //Get the cost of this child's children
        total_cost += get_cost(root->children[i], file);
    }
    
    
    return total_cost;
}
