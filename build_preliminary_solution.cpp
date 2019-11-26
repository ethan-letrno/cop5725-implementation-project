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

//Taken from naive, and divide_subsets
bool sortbysecdesc(const std::pair<std::vector<int>,double> &a, const std::pair<std::vector<int>,double> &b)
{
    return (a.second > b.second);
}

bool sortbygroup(const std::pair<std::vector<int>,double> &a, const std::pair<std::vector<int>,double> &b)
{
    return (a.first.size() > b.first.size());
    
}


Node * find_min_sort_cost(Node *, Node *, std::string);
void fix_scan(Node *);
int contains_attribute(Node * newNode, Node * currentNode);


Node * build_preliminary_solution(std::vector< std::vector< std::string > > terminal_nodes, std::string file) {
    
    std::vector< std::vector<int> > terminals;
    
    //Get column number for each attribute, store in terminals
    for(int i = 0; i < terminal_nodes.size(); i++) {
        std::vector< int > temp;
        for (int k = 0; k < terminal_nodes[i].size(); k++) 
            temp.push_back(getColumnForLineitemElement(terminal_nodes[i][k]));
        terminals.push_back(temp);
    }
    
    //Create vector of pairs for cardinalities
    std::vector<std::pair<std::vector<int>,double>> cardinalities;
    
	//Get cardinalities of terminals
	for(int x = 0; x < terminals.size(); x++)
		cardinalities.push_back(make_pair(terminals[x], estimateCardinality(terminals[x], file)));
    
    //Sort cardinalities in descending order
    sort(cardinalities.begin(), cardinalities.end(), sortbysecdesc);
    
    //Now sort by group
    sort(cardinalities.begin(), cardinalities.end(), sortbygroup);
    
    //Create root node
    std::vector< int > root_info;

    for(int i = 0; i <= 15; i++)
        root_info.push_back(i);
    Node * root = newNode(root_info);
    
    //for each terminal node
    for(int v = 0; v < cardinalities.size(); v++) {
        std::pair<std::vector<int>, double> umin;
        //u min = argminu csort(u,v) | u in G'       find the parent with the smallest sort cost
        
        //Create new node for the terminal
        Node * nNode = newNode(cardinalities[v].first);
            
        //IF we are the root node T
        if (root->children.size() == 0)
            root->children.push_back(nNode);
        else {
            //find node in G' such that it and the node v has the smallest sort cost
            //u_min is a node already in the tree
           
            Node * u_min = find_min_sort_cost(root, nNode, file);
            
            //make nNode a child of u_min
            u_min->children.push_back(nNode);
            
        }
        
    }
    
    fix_scan(root);
    std::cout << "The tree is currently as follows: " << std::endl;
    preord(root);

    return root;
}

void fix_scan(Node * root) {
    //for each children node of root
    for (int i = 0; i < root->children.size(); i++) {
        
        int match = 0;
        for(int j = 0; j < root->children[i]->group.size(); j++) {
            //go up to the children's name. if we run out and still find a match, we are good
            if (root->children[i]->group[j] == root->group[j]) { //assumes the parents group by is larger
                match = 1;
            }
            else
                match = 0;
            
        }
        
        if (match) {
            //move to the front of the list
            root->children.insert(root->children.begin(), root->children[i]);
            root->children.erase(root->children.begin()+i+1);
        }
        
        fix_scan(root->children[i]);

    }
}

Node * find_min_sort_cost(Node * currentNode, Node * newNode, std::string file) {
    
    //float current_min = sortCost(root->first, newNode->first);
    Node * current_min_node = currentNode;
    
    if (current_min_node == NULL)
        return current_min_node;
    
    double sort_cost = sortCost(currentNode->group, file);
    
    for (int i = 0; i < currentNode->children.size(); i++) {
        int attr = contains_attribute(newNode, currentNode->children[i]);
        if (attr == newNode->group.size()) {
            Node * min = find_min_sort_cost(currentNode->children[i], newNode, file);
        
            if (sortCost(min->group, file) < sort_cost)
                current_min_node = min;
        } 
    }
    
    return current_min_node;
    
}

int contains_attribute(Node * newNode, Node * currentNode) {
    int attribute = 0;
    for(int j = 0; j < newNode->group.size(); j++) {
        for(int k = 0; k < currentNode->group.size(); k++) {
            if (newNode->group[j] == currentNode->group[k])
                attribute++;  
        }
    }
    
    return attribute;
    
}


