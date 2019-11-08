
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>

struct Result{
	std::vector<std::string> group;
	int count;
};

int NaiveExecution(std::vector<std::vector<std::string>> elements, std::string table);
int getColumnForLineitemElement(std::string element);
void tokenize(std::string const &str, std::vector<std::string> &out);


int main(){

	std::string table = "lineitem184k.table";
	std::vector<std::vector<std::string>> elements = {
		{"ORDERKEY"},
		{"PARTKEY"},
		{"ORDERKEY", "PARTKEY"},
	}

	auto start = std::chrono::system_clock::now();
    NaiveExecution(elements, table);
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "Finished querying at " << std::ctime(&end_time)
              << ", elapsed time = " << elapsed_seconds.count() << " seconds\n";

}



/* 
Takes in list of sets of elements and a list of tables where elements[x] corresponds with tables[x]. 
Represent the query SELECT elements[x], COUNT(*) FROM tables[x] GROUP BY elements[x]. Call this "query x".
No execution plan, simply compute the queries one after another "naively".
We're just counting the number of rows wherein the elements specified are the same.
*/
int NaiveExecution(std::vector<std::vector<std::string>> elements, std::string table){

	//For each resultset[x] representing the query x, every element resultset[x][1], [2] and so on will house
	//a grouping that occurs in the table specified and the amount of times it occurs.
	std::vector<std::vector<Result>> resultset;
	std::ifstream file;
	std::string current_line;
	std::vector<std::string> out;
	std::vector<std::string> out_trimmed;
	int counted = 0;

	//For each query...
	for(int x = 0; x < elements.size(); x++){

		//Push a vector of results onto the 2d vector for this query.
		std::vector<Result> v;
		resultset.push_back(v);
		
		file.open(table.c_str());

		//Read each line in the table
		while(file >> current_line){

			//Split the current line on '|'
			tokenize(current_line, out);

			//Store into seperate vector the relevant elements
			for(int a = 0; a < elements[x].size(); a++){
				for(int b = 0; b < out.size(); b++){
					if(getColumnForLineitemElement(elements[x][a]) == b){
						out_trimmed.push_back(out[b]);
						break;
					}

				}
			}

			//If there are no results here...
			if(resultset[x].empty()){
				resultset[x].push_back(Result());
				resultset[x][0].group.insert(resultset[x][0].group.end(), out_trimmed);
				resultset[x][0].count = 1;
			}
			else{
				//Search through resultset[x] for matching gruoping, if found increment count.
				for(int y = 0; y < resultset[x].size(); y++){
					if(resultset[x][y].group == out_trimmed){
						resultset[x][y].count++;
						counted = 1;
						break;
					}
				}
				//If not already found, add to resultset[x]
				if(counted == 0){
					resultset[x].push_back(Result());
					resultset[x].back().group.insert(resultset.back().group.end(), out_trimmed);
					resultset[x].back().count = 1;
				}

				counted = 0;
			}

			out.clear();
			out_trimmed.clear();

		}

		file.close();
		file.clear();

	}

	//Having all results for each query, write them to a text file in current directory.

	/* OUTPUT CODE HERE */

	


	return 0;
}

int getColumnForLineitemElement(std::string element){
	switch(element){
		case "ORDERKEY":
			return 0;
		case "PARTKEY":
			return 1;
		case "SUPPKEY":
			return 2;
		case "LINENUMBER":
			return 3;
		case "QUANTITY":
			return 4;
		case "EXTENDEDPRICE":
			return 5;
		case "DISCOUNT":
			return 6;
		case "TAX":
			return 7;
		case "RETURNFLAG":
			return 8;
		case "LINESTATUS":
			return 9;
		case "SHIPDATE":
			return 10;
		case "COMMITDATE":
			return 11;
		case "RECEIPTDATE":
			return 12;
		case "SHIPINSTRUCT":
			return 13;
		case "SHIPMODE":
			return 14;
		case "COMMENT":
			return 15;
	}
}


//Modified from function from source: https://www.techiedelight.com/split-string-cpp-using-delimiter/
void tokenize(std::string const &str, std::vector<std::string> &out){
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of('|', end)) != std::string::npos){
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
}
