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

struct Result{
	std::vector<std::string> group;
	int count;
};

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


int main(){

	return 0;
}

std::string getElementForColumn(int element);
void tokenize(std::string const &str, std::vector<std::string> &out);
int getColumnForLineitemElement(std::string element);

void OptimalSolution(Node * root, std::vector<std::vector<int>> terminals, std::string filename){

	std::vector<Result> resultset;
	std::ifstream file;
	std::ofstream outfile;
	std::string current_line;
	std::vector<std::string> out;
	std::vector<std::string> out_trimmed;
	int counted = 0;
	int terminal = 0;
		
	file.open(filename.c_str());

	for( Node * n : root->children){

		//If the root is the actual root node, We need to read  input file as the original table.
		if(root->group.empty()){
			while(std::getline(file,current_line)){
				tokenize(current_line, out);

				for(int a = 0; a < n->group.size(); a++){
					for(int b = 0; b < out.size(); b++){
						if(n->group[a] == b){
							out_trimmed.push_back(out[b]);
							break;
						}
					}
				}

				//If there are no results here...
				if(resultset.empty()){
					resultset.push_back(Result());
					resultset[0].group.insert(resultset[0].group.end(), out_trimmed.begin(), out_trimmed.end());
					resultset[0].count = 1;
				}
				else{
					//Search through resultset[x] for matching gruoping, if found increment count.
					for(int y = 0; y < resultset.size(); y++){
						if(resultset[y].group == out_trimmed){
							resultset[y].count++;
							counted = 1;
							break;
						}
					}
					//If not already found, add to resultset[x]
					if(counted == 0){
						resultset.push_back(Result());
						resultset.back().group = out_trimmed;
						//resultset[x].back().group.insert(resultset.back().group.end(), out_trimmed.begin(), out_trimmed.end());
						resultset.back().count = 1;
					}

					counted = 0;
				}

				out.clear();
				out_trimmed.clear();

			}

		}
		
		//Otherwise, we realize the file is a temporary table, with counts of pairings, and use it accordingly.
		else{

			std::vector<int> columns;
			while(std::getline(file,current_line)){
				tokenize(current_line, out);

				//Get columns that we need to grab for this nodes group
				if(getColumnForLineitemElement(out[0]) != -1){
					for(int i : n->group){
						for(int x = 0; x < out.size(); x++){
							if(getColumnForLineitemElement(out[x]) == i){
								columns.push_back(x);
							}
						}
					}
				
				}
				else{

					for(int a = 0; a < columns.size(); a++){
						for(int b = 0; b < out.size(); b++){
							if(columns[a] == b){
								out_trimmed.push_back(out[b]);
								break;
							}
						}
					}

					//Now out_trimmed has the values for the group we want, and the end of "out" has the count for that row.

					//If there are no results here...
					if(resultset.empty()){
						resultset.push_back(Result());
						resultset[0].group.insert(resultset[0].group.end(), out_trimmed.begin(), out_trimmed.end());
						resultset[0].count = stoi(out[out.size()-1]);
					}
					else{
						//Search through resultset[x] for matching gruoping, if found increment count.
						for(int y = 0; y < resultset.size(); y++){
							if(resultset[y].group == out_trimmed){
								resultset[y].count += stoi(out[out.size()-1]);;
								counted = 1;
								break;
							}
						}
						//If not already found, add to resultset[x]
						if(counted == 0){
							resultset.push_back(Result());
							resultset.back().group = out_trimmed;
							//resultset[x].back().group.insert(resultset.back().group.end(), out_trimmed.begin(), out_trimmed.end());
							resultset.back().count = stoi(out[out.size()-1]);
						}

						counted = 0;
					}

				}

				
				out.clear();
				out_trimmed.clear();
				//push back onto resultset the vector of the groups columns plus whatever number is at the end of the line.


			}
		}

		//Now regardless of whether the resultet was gotten from the table or a temp file, it resides in resultset and needs to be either
			//written to an output file if it's a terminal group
			//written to a temp file if it has children nodes
			//or both

		for(std::vector<int> v : terminals){
			if(v == n->group){
				terminal = 1;
				break;
			}
		}
		

		if(terminal == 1){
			//If n's group is a terminal, write it to an output file.

			std::string output_filename = "optimal_output_for";
			for(int i : n->group){
				output_filename = output_filename + "_" + getElementForColumn(i);
			}
			output_filename = output_filename + ".txt";

			outfile.open(output_filename);

			for(int x = 0; x < resultset.size(); x++){
				for(int z = 0; z < resultset[x].group.size(); z++){
					outfile<<resultset[x].group[z]<<", ";
				}

				outfile<<"Count = "<<resultset[x].count<<std::endl;
			}

			outfile.close();
			outfile.clear();
		}

		if(!n->children.empty()){
			//If this node had children, make a temp file for them to draw from, and recursively call this function.

			std::string temp_filename = "t";
			for(int i : n->group){
				temp_filename = temp_filename + "_" + getElementForColumn(i);
			}
			temp_filename = temp_filename + ".txt";

			outfile.open(temp_filename);

			for(int i : n->group){
				outfile<<getElementForColumn(i)<<"|";
			}
			outfile<<std::endl;

			for(int x = 0; x < resultset.size(); x++){
				for(int z = 0; z < resultset[x].group.size(); z++){
					outfile<<resultset[x].group[z]<<"|";
				}

				outfile<<resultset[x].count<<std::endl;
			}

			outfile.close();
			outfile.clear();

			for(Node * m : n->children){
				OptimalSolution(m, terminals, temp_filename);
			}

		}

	}

	//Delete temp files once execution is complete.
	system("DEL t_*.txt");

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

std::string getElementForColumn(int element){

	switch (element) 
   { 
       case 0: return "ORDERKEY"; 
       case 1: return "PARTKEY"; 
       case 2: return "SUPPKEY"; 
       case 3: return "LINENUMBER"; 
       case 4: return "QUANTITY"; 
       case 5: return "EXTENDEDPRICE"; 
       case 6: return "DISCOUNT"; 
       case 7: return "TAX"; 
       case 8: return "RETURNFLAG"; 
       case 9: return "LINESTATUS"; 
       case 10: return "SHIPDATE"; 
       case 11: return "COMMITDATE"; 
       case 12: return "RECEIPTDATE"; 
       case 13: return "SHIPINSTRUCT"; 
       case 14: return "SHIPMODE"; 
       case 15: return "COMMENT"; 
       default: return "ERROR";  
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
