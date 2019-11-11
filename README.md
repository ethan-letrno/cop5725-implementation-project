# cop5725-implementation-project

Implementation in C++ of the algorithms described in the 2016 ICDE paper "A Novel, Low-latency Algorithm for Multiple Group-By Query Optimization".

For each .table file, the naming convention is (name of table)(number of rows).table, wherein "k" is shorthand for thousand and "m" for million.

Naive solution with the following Group By's:

{"ORDERKEY"},
{"PARTKEY"},
{"ORDERKEY", "PARTKEY"}

Ran for 2219.46 seconds, or ~37 minutes. Naive solution with the following Group By's:

{"ORDERKEY"},
{"SHIPINSTRUCT"},
{"ORDERKEY", "SHIPINSTRUCT"}

Ran for 740.65 seconds, or ~12.5 minutes. 


Long time in the first could be because the 3rd group is the primary key for the table and/or we use vectors to store, and it takes O(n) time to search them.



TREE STRUCTURE:
----------------------------------------------
// Represents a node of a tree 
struct Node 
{ 
    std::vector<std::int>> group; 
   	std::vector<Node * >child; 
}; 
   
 // Utility function to create a new tree node 
Node * newNode(std::vector<std::string>> group) 
{ 
    Node * temp = new Node; 
    temp->group = group; 
    return temp; 
} 
----------------------------------------------

CARDINALITY ESTIMATOR
----------------------------------------------
double estimateCardinality(std::vector<int> indices, std::string filename){

    std::ifstream file;
	srand(time(0));
	int row = rand() % 20000 + 1; //Get random row from row 1 to 20,000 (we assume each table will have at least this many + 1000 rows)
	std::string current_line;
	std::vector<std::string> out;
	std::vector<std::string> out_trimmed;

	std::set<std::vector<std::string>> dv;

	std::cout<<"(from row "<<row<<") ";

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
----------------------------------------------




# To Do

1. 'optimized' solution works similarly to naive but executes based on tree structure from (3)
3. algorithms from paper that create an optimal tree
    - algorithm 1
    - algorithm 6
    

# Tentative Plan

1. by thursday november 14
    - algorithm 1, 6
    - status report
    - presentation slides
    
2. by thursday november 21
    - optimal solution done
    - algorithm 2, 3 done
    - algorithm essentially complete
    
3. by tuesday november 26
    - testing and performance eval
    - write final report
    
4. by tuesday, dec 3
    - final report due / presentation

    
