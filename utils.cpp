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

bool sortbysecdesc(const std::pair<std::vector<int>,double> &a, 
                   const std::pair<std::vector<int>,double> &b) 
{ 
       return a.second>b.second; 
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
    //std::cout << "Iterating through children from left to right:" << std::endl;
    for (int i = 0; i < root->children.size(); i++) {
        preord(root->children[i]);
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

	

	std::string length ="";
	for(char i : filename){
		if(i != '.'){
			length.push_back(i);
		}
		else break;
	}

	//Returns estimate of actual number of rows in table.
	return (std::stod(length)*(dv.size()/500.0));

}

double scanCost(std::vector<int> indices, std::string filename){
	return estimateCardinality(indices,filename);
}

double sortCost(std::vector<int> indices, std::string filename){
    if (indices[0] == -1)
        return (184000 * log2(184000));
    
	double car = estimateCardinality(indices,filename);
    return (car * log2 (car));
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


