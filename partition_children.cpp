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
void preord(Node*);
double get_cost(Node *, std::string);

int partition_children(Node *, int, std::string);



int main() {
    
    
    std::string table = "lineitem184k.table";
    
    //int strategy = partition_children(u, k, table, G_prime);
    
    return 0;
    
}


int partition_children(Node * u, int k, std::string filename, Node * G_prime) {
    //Find the best strategy to partition children of node u into at most k subsets
    
    //returns true if it can find a way to optimize u (split children of node u into smaller subsets)
    
    //recursively apply the splitting procedure on each child node of u
    int q = u->children.size();
    if (q <= 1)
        return 0;
    
    double C_min = get_cost(G_prime, filename);
                 
    std::vector< Node * > SS;
    
    if (k > q)
        k = q;
        
    for (int k_prime = 0; k_prime < k; k++) {
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
            
        //Get the cost of this child's children
        total_cost += get_cost(root->children[i], file);
    }
    
    
    return total_cost;
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

double scanCost(std::vector<int> indices, std::string filename){
	return estimateCardinality(indices,filename);
}

double sortCost(std::vector<int> indices, std::string filename){
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