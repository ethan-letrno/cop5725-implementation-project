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
   
int main() {
    
    std::vector<std::vector<std::string>> elements = {
		{"ORDERKEY"},
		{"PARTKEY"},
		{"ORDERKEY", "PARTKEY"},
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
    
    //Create root node
    std::vector< int > root_info;
    Node * root = newNode(root_info);
    
    //for each terminal node
    for(int v = 0; v < cardinalities.size(); v++) {
        std::cout << cardinalities[v].second << std::endl;
        std::pair<std::vector<int>, double> umin;
        //u min = argminu csort(u,v) | u in G'       find the parent with the smallest sort cost
        
        //Create new node for the terminal
        Node * nNode = newNode(cardinalities[v].first);
        
        //Just add it to root, for now
        root->children.push_back(nNode);
        
        //G' <--- G' or v: add v to G'
        //E' <--- E' or esort(umin, v)
        //sortCost(cardinalities[v].first, file)
    }
    
    for(int m = 0; m < root->children.size(); m++)
        std::cout << root->children[m] << std::endl;
  
    //for u in G'
        //fix_scan(u)
   
    return root;
}

double estimateCardinality(std::vector<int> indices, std::string filename){

	std::ifstream file;
	srand(time(0));
	int row = rand() % 20000 + 1; //Get random row from row 1 to 20,000 (we assume each table will have at least this many + 1000 rows)
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
	return (dv.size()/500.0);

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


