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

//Taken from naive, and divide_subsets
bool sortbysecdesc(const std::pair<std::vector<int>,double> &a, const std::pair<std::vector<int>,double> &b)
{
    return (a.second > b.second);
}

bool sortbygroup(const std::pair<std::vector<int>,double> &a, const std::pair<std::vector<int>,double> &b)
{
    return (a.first.size() > b.first.size());
    
}

// Represents a node of a tree 
struct Node 
{ 
    std::vector< int > group; 
   	std::vector<Node *>children; 
};


// Utility function to create a new tree node 
Node *newNode(std::vector< int > group) 
{ 
    Node *temp = new Node; 
    temp->group = group; 
    return temp; 
}

double estimateCardinality(std::vector<int> indices, std::string filename);
double sortCost(std::vector<int> indices, std::string filename);
double scanCost(std::vector<int> indices, std::string filename);
void tokenize(std::string const &str, std::vector<std::string> &out);
int getColumnForLineitemElement(std::string element);
Node * build_preliminary_solution(std::vector< std::vector< std::string > >, std::string);
void preord(Node* root);
Node * find_min_sort_cost(Node *, Node *, std::string);
void fix_scan(Node *);

int main() {
    
    std::vector<std::vector<std::string>> elements = {
		{"SHIPINSTRUCT"},
        {"ORDERKEY"},
		{"ORDERKEY", "SHIPINSTRUCT"},
	};
    
    std::string table = "lineitem184k.table";
    
    Node * G_prime = build_preliminary_solution(elements, table);
    
    return 0;
}

/*
 *Returns a solution tree including only terminal nodes (and root node).
 *
 *Sort terminal nodes in order of cardinality.
 *
 *Traverse through terminal nodes, adding them to G prime, the prelim solution tree
 *
 *Once all are added, update scan/sort connection between u and children
 *
 *fix_scan(u) finds a child node of u that brings the biggest cost reduction when its edge is turned
 *from sort to scan
 */
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
    root_info.push_back(-1);
    Node * root = newNode(root_info);
    
    //for each terminal node
    for(int v = 0; v < cardinalities.size(); v++) {
        std::pair<std::vector<int>, double> umin;
        //u min = argminu csort(u,v) | u in G'       find the parent with the smallest sort cost
        
        //Create new node for the terminal
        Node * nNode = newNode(cardinalities[v].first);
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << "ADD NODE TO THE TREE WITH GROUP BY: ";
        for (int k = 0; k < nNode->group.size(); k++)
            std::cout << nNode->group[k] << " | ";
            
        std::cout << std::endl;
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
       
        std::cout << "The tree is currently as follows: " << std::endl;
        preord(root);
        
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
    
    
    //If the node has no children, what would be the cost if you added it ?
    if (currentNode->children.size() == 0) {
        //dont do anything
        return current_min_node;
        
    }
    else {
        for (int i = 0; i < currentNode->children.size(); i++) {
            int contains_attribute = 0;
            
            
            
            for(int j = 0; j < newNode->group.size(); j++) {
                if (newNode->group[j] == currentNode->group[j])
                    contains_attribute = 1;
                
            }
            if (contains_attribute == 1 || currentNode->group[0] == -1) {

                Node * node_w_min_sort_cost = find_min_sort_cost(currentNode->children[i], newNode, file);

                if (sortCost(node_w_min_sort_cost->group, file) < sortCost(current_min_node->group, file))
                    current_min_node = node_w_min_sort_cost;
            }
        }
    }
    return current_min_node;
}

void preord(Node* root) 
{
    
    if (root == NULL) {
        return;
    }
    
    //Print root data
    std::cout << "Current node is: ";
    for (int j = 0; j < root->group.size(); j++) {
        std::cout << root->group[j];
    }
    std::cout << std::endl;
    
    std::cout << "This node's children are: ";
    for (int m = 0; m < root->children.size(); m++) {
        for (int k = 0; k < root->children[m]->group.size(); k++) 
            std::cout << root->children[m]->group[k];
        std::cout << std::endl;
    }
    std::cout << std::endl;
    //Print children data from left to right
    std::cout << "Iterating through children from left to right:" << std::endl;
    for (int i = 0; i < root->children.size(); i++) {
        preord(root->children[i]);
    }
     
} 

double estimateCardinality(std::vector<int> indices, std::string filename){
    
    //If this is the root, or insert table, cardinality is max at 1
    if (indices[0] == -1)
        return 1; 

	std::ifstream file;
	srand(time(0));
	int row = rand() % 20000+ 1; //Get random row from row 1 to 20,000 (we assume each table will have at least this many + 1000 rows)
	std::string current_line;
	std::vector<std::string> out;
	std::vector<std::string> out_trimmed;

	std::set<std::vector<std::string>> dv;

	file.open(filename.c_str());

	//Get to row'th row
	for(int x = 0; x < row; x++){
		std::getline(file,current_line);
	}

	//Sampling 500 random alternating lines
	for(int y = 0; y < 1000; y+=2){
		std::getline(file,current_line);
		tokenize(current_line, out);

		for(int a = 0; a < indices.size(); a++){
			for(int b = 0 ; b < out.size(); b++){
				if(indices[a] == b){
					out_trimmed.push_back(out[b]);
					break;
				}
			}
		}

		//Insert the grouped line into a set. Will only insert if not a dupe of something
		//already in set.
		dv.insert(out_trimmed);

		out.clear();
		out_trimmed.clear();

	}

	file.close();

	//This will return a number from 0 - 1, a 1 indicating that every single line was unique. So, the higher this number, the higher the cardinality estimated.
	return (dv.size()/500);

}

double sortCost(std::vector<int> indices, std::string filename){
	return estimateCardinality(indices,filename);
}

double scanCost(std::vector<int> indices, std::string filename){
	double car = estimateCardinality(indices,filename);
	return (car * log2(car));
}

int getColumnForLineitemElement(std::string element){
	if(element.compare("ORDERKEY") == 0){
		return 0;
	}
	else if(element.compare("PARTKEY") == 0){
		return 1;
	}
	else if(element.compare("SUPPKEY") == 0){
		return 2;
	}
	else if(element.compare("LINENUMBER") == 0){
		return 3;
	}
	else if(element.compare("QUANTITY") == 0){
		return 4;
	}
	else if(element.compare("EXTENDEDPRICE") == 0){
		return 5;
	}
	else if(element.compare("DISCOUNT") == 0){
		return 6;
	}
	else if(element.compare("TAX") == 0){
		return 7;
	}
	else if(element.compare("RETURNFLAG") == 0){
		return 8;
	}
	else if(element.compare("LINESTATUS") == 0){
		return 9;
	}
	else if(element.compare("SHIPDATE") == 0){
		return 10;
	}
	else if(element.compare("COMMITDATE") == 0){
		return 11;
	}
	else if(element.compare("RECEIPTDATE") == 0){
		return 12;
	}
	else if(element.compare("SHIPINSTRUCT") == 0){
		return 13;
	}
	else if(element.compare("SHIPMODE") == 0){
		return 14;
	}
	else if(element.compare("COMMENT") == 0){
		return 15;
	}
	else{
		return -1;
	}

}

//Modified from function from source: https://www.techiedelight.com/split-string-cpp-using-delimiter/
void tokenize(std::string const &str, std::vector<std::string> &out){
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of('|', end)) != std::string::npos){
		end = str.find('|', start);
		out.push_back(str.substr(start, end - start));
	}
}


