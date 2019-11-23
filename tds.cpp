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
} 

void TopDownSplit(Node * u, int k, std::string filename);
int PartitionChildren(Node * u, int k, std::string filename);


int main(){

	return 0;
}

void TopDownSplit(Node * u, int k, std::string filename){

	int b = 0;

	do{
		b = PartitionChildren(u, k, filename);
	}while(b != 0);

	for( Node * v : u->children){
		TopDownSplit(v, k, filename);
	}

}

int PartitionChildren(Node * u, int k, std::string filename){
	return 0;
}