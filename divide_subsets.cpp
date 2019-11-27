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

/*
bool sortbysecdesc(const std::pair<std::vector<int>,double> &a, 
                   const std::pair<std::vector<int>,double> &b) 
{ 
       return a.second>b.second; 
} */
   

/*
double estimateCardinality(std::vector<int> indices, std::string filename);
double sortCost(std::vector<int> indices, std::string filename);
double scanCost(std::vector<int> indices, std::string filename);
void DivideSubsets(Node * u, int k, std::string filename);

//COPIED FROM NAIVE.CPP
void tokenize(std::string const &str, std::vector<std::string> &out);
int getColumnForLineitemElement(std::string element);

int main(){

	Node * n = newNode({1, 7, 11, 14});
	n->children.push_back(newNode({1}));
	n->children.push_back(newNode({7}));
	n->children.push_back(newNode({11, 14}));
	DivideSubsets(n, 4, "lineitem184k.table");

	for(Node* c : n->children){
		std::cout<<"NODE: ";
		for(int i : c->group){
			std::cout<<i<<", ";
		}
		std::cout<<" with cardinality "<<estimateCardinality(c->group, "lineitem184k.table")<<"\n";
	}


	return 0;

 */



/* 

ALGORITHM 6 - DIVIDE SUBSETS (u = Node to divide children of, k = Number of initial subsets)

1. Sort Children nodes of "u" by decreasing cardinality
2. Initialize k amount of subsets
3. For each v child node, check the scan cost of each Subset, and attach it to the smallest cost one.
	- Can only attach if SS U v != u
	- If SS empty, automatically attach it
4. If we run out of subsets that will work, add another to SS and attach to this new one.


*/
void DivideSubsets(Node * u, int k, std::string filename){

	int p = k;
	double lowcost = 2.0;
	int ss_to_join = -1;
	std::vector<std::pair<std::vector<int>,double>> cardinalities;
	std::vector<Node *> SS;

	//Getting the cardinalities of each of u's child nodes
	for(int x = 0; x < u->children.size(); x++){
		cardinalities.push_back(std::make_pair(u->children[x]->group, estimateCardinality(u->children[x]->group, filename)));
	}

	//Now, cardinalities is a vector of pairs with each child node and it's cardinality estimate. Now sort on this:
	std::sort(cardinalities.begin(), cardinalities.end(), sortbysecdesc);

	//Fill SS with appropriate amount of subsets
	for(int y = 0; y < p; y++){
		SS.push_back(newNode({}));
	}

	//Going through each node in descending order of cardinality, check against each subset for which will give least cost from attach(). 
	for(std::pair<std::vector<int>,double> v : cardinalities){

		/*std::cout<<"[TEST] Node containing { ";
		for(int i : v.first){
			std::cout<<i<<" ";
		}
		std::cout<<"} has cardinality "<<v.second<<"\n"; */

		for(int i = 0; i < SS.size(); i++){
			//if Subset is empty, cost is 0 and we should fill it.
			if(SS[i]->group.empty()){
				SS[i]->group.insert(SS[i]->group.end(), v.first.begin(), v.first.end());
				ss_to_join = -2;
				break;
			}
			else{
				if(sortCost(SS[i]->group, filename) < lowcost && (SS[i]->group.size() + v.first.size()) != u->group.size()){
					ss_to_join = i;
					lowcost = sortCost(SS[i]->group, filename);
				}
			}

		}

		//So if we inserted into empty SS already do nothing,
		//If we have a SS we can put v into, put it in
		//Else create new SS, push v into the newly made one.
		if(ss_to_join != -1){
			if(ss_to_join == -2){ }
			else{
				SS[ss_to_join]->group.insert(SS[ss_to_join]->group.end(), v.first.begin(), v.first.end());

				//If the newly inserted group gives the Subset 2 or more groups, make the individual ones child nodes of that SS.
				if(SS[ss_to_join]->group.size() == 2){
					SS[ss_to_join]->children.push_back(newNode({SS[ss_to_join]->group[0]}));
					SS[ss_to_join]->children.push_back(newNode({SS[ss_to_join]->group[1]}));
				}
				else if(SS[ss_to_join]->group.size() > 2){
					SS[ss_to_join]->children.push_back(newNode(v.first));
				}
			}
		}
		else{
			SS.push_back(newNode({}));
			SS.back()->group.insert(SS.back()->group.end(), v.first.begin(), v.first.end());
		}

		//Reset variables
		ss_to_join = -1;
		lowcost = 2;

	}

	//Now that all the subsets have been divided, remake the children of node u to be those in the subset. (Should we do this?)
	u->children.clear();

	for(Node * ss : SS){
		if(!ss->group.empty())
			u->children.push_back(ss);
	}

}
/* ALGORITHM 6 END */

 /*

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
} */
