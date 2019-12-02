#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <stdlib.h>
#include <set>
#include <ctime>
#include <bits/stdc++.h> 
#include <math.h>  
#include <utility>
#include "utils.h"
#include "partition_children.h"

/*
// Represents a node of a tree 
struct Node 
{ 
    std::vector<int> group; 
   	std::vector<Node *>children; 
}; 
   
 // Utility function to create a new tree node 
Node *newNode(std::vector<int> group) 
{ 
    Node *temp = new Node; 
    temp->group = group; 
    return temp; 
} */


void TopDownSplit(Node * u, int k, std::string filename, Node * g_prime){

	int b = 0;

	do{
		b = partition_children(u, k, filename, g_prime);
	}while(b != 0);
	if(!u->children.empty()){
		for( Node * v : u->children){
			TopDownSplit(v, k, filename, v);
		}
	}

}
