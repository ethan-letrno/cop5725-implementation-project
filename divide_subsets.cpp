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

ALGORITHM 6 - DIVIDE SUBSETS (u = Node to divide children of, k = Number of initial subsets)

1. Sort Children nodes of "u" by decreasing cardinality
2. Initialize k amount of subsets
3. For each v child node, check the scan cost of each Subset, and attach it to the smallest cost one.
	- Can only attach if SS U v != u
	- If SS empty, automatically attach it
4. If we run out of subsets that will work, add another to SS and attach to this new one.


*/
std::vector<Node *> DivideSubsets(Node * u, int k, std::string filename){

	int p = k;
	double lowcost = 2000000.0;
	int ss_to_join = -1;
	int indice_counter = -1;
	std::vector<std::pair<std::vector<int>,double>> cardinalities;
	std::vector<std::pair<std::vector<int>,int>> children_nodes;
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
		indice_counter++;

		/*std::cout<<"[TEST] Node containing { ";
		for(int i : v.first){
			std::cout<<i<<" ";
		}
		std::cout<<"} has cardinality "<<v.second<<"\n"; */

		for(int i = 0; i < SS.size(); i++){
			//if Subset is empty, cost is 0 and we should fill it.
			if(SS[i]->group.empty()){
				SS[i]->group.insert(SS[i]->group.end(), v.first.begin(), v.first.end());
				SS[i]->children.push_back(newNode(v.first));
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

				//std::cout<<"SS["<<ss_to_join<<" Group size = "<<SS[ss_to_join]->group.size()<<"\n";

				//Here, need to make sure were not inserting duplicate indicies
				for(int i : v.first){
					if(!std::count(SS[ss_to_join]->group.begin(), SS[ss_to_join]->group.end(), i)){
						SS[ss_to_join]->group.push_back(i);
					}
				}

				SS[ss_to_join]->children.push_back(newNode(v.first));

				/*If the newly inserted group gives the Subset 2 or more groups, make the individual ones child nodes of that SS.
				if(SS[ss_to_join]->group.size() == 2){
					SS[ss_to_join]->children.push_back(newNode({SS[ss_to_join]->group[0]}));
					SS[ss_to_join]->children.push_back(newNode({SS[ss_to_join]->group[1]}));
					std::cout<<"Adding initial two children to node"<<ss_to_join<<"\n";
				}
				else if(SS[ss_to_join]->group.size() > 2){
					SS[ss_to_join]->children.push_back(newNode(v.first));
					std::cout<<"Adding additional child to node"<<ss_to_join<<", Group size = "<<SS[ss_to_join]->group.size()<<"\n";
				}*/
			}
		}
		else{
			SS.push_back(newNode({}));
			SS.back()->group.insert(SS.back()->group.end(), v.first.begin(), v.first.end());
			SS[SS.size()-1]->children.push_back(newNode(v.first));
		}

		//Reset variables
		ss_to_join = -1;
		lowcost = 2000000.0;

	}

	//Now that all the subsets have been divided, remake the children of node u to be those in the subset. (Should we do this?)
	//std::cout<<"DIV SUBSETS: FOR K = "<<k<<" (Subset size is "<<SS.size()<<")"<<std::endl;
	for(Node * ss : SS){
		//IF DELETING THE OUTPUT KEEP THIS!!
		if(ss->children.size() == 1){
			ss->children.clear();
		}

		//for(int ch : ss->group){
		//	std::cout<<ch<<", ";			
		//}
		//std::cout<<"cardinality = "<<sortCost(ss->group, filename)<<", "<<ss->children.size()<<" Children."<<std::endl;

	} 
    
    return SS;

}
/* ALGORITHM 6 END */

