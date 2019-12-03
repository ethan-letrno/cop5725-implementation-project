#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <set>
#include <ctime>
#include <bits/stdc++.h> 
#include <math.h>  
#include <utility>
#include "utils.h"
#include "build_preliminary_solution.h"
#include "tds.h"

struct Result{
    std::vector<std::string> group;
    int count;
};

void OptimalSolution(Node * root, std::vector<std::vector<int>> terminals, std::string filename);
std::string getElementForColumn(int element);

int main() {

    std::vector<std::vector<int>> indices;
    std::vector<int> ind;
    
    std::vector<std::vector<std::string>> elements = {
        {"LINENUMBER","QUANTITY"}, //1
        {"LINENUMBER","DISCOUNT"},
        {"LINENUMBER","TAX"},
        {"LINENUMBER","RETURNFLAG"},
        {"LINENUMBER","LINESTATUS"},
        {"LINENUMBER","SHIPINSTRUCT"},
        {"LINENUMBER","SHIPMODE"},
        //{"QUANTITY","DISCOUNT"},
        {"QUANTITY","TAX"},
        //{"QUANTITY","RETURNFLAG"},
        {"QUANTITY","LINESTATUS"},
        //{"QUANTITY","SHIPINSTRUCT"},
        {"QUANTITY","SHIPMODE"},
        {"DISCOUNT","TAX"},
        {"DISCOUNT","RETURNFLAG"},
        {"DISCOUNT","LINESTATUS"},
        {"DISCOUNT","SHIPINSTRUCT"},
        {"DISCOUNT","SHIPMODE"},
        {"TAX","RETURNFLAG"},
        {"TAX","LINESTATUS"},
        {"TAX","SHIPINSTRUCT"},
        {"TAX","SHIPMODE"},
        {"RETURNFLAG","LINESTATUS"},
        {"RETURNFLAG","SHIPINSTRUCT"},
        {"RETURNFLAG","SHIPMODE"},
        {"LINESTATUS","SHIPINSTRUCT"},
        {"LINESTATUS","SHIPMODE"},
        {"SHIPINSTRUCT","SHIPMODE"} //28
	};

    for(std::vector<std::string> v : elements){
        for(std::string s : v){
            ind.push_back(getColumnForLineitemElement(s));
        }
        indices.push_back(ind);
        ind.clear();
    }
    
    std::string table = "2000000.table";
  
    auto start = std::chrono::system_clock::now();  
    Node * G_prime = build_preliminary_solution(elements, table);
    TopDownSplit(G_prime, 3, table, G_prime);
    
    //preord(G_prime);

    OptimalSolution(G_prime, indices, table);
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "Finished querying at " << std::ctime(&end_time)
              << ", elapsed time = " << elapsed_seconds.count() << " seconds\n";

    
    
}

void OptimalSolution(Node * root, std::vector<std::vector<int>> terminals, std::string filename){

    std::vector<Result> resultset;

    std::ifstream file;
    std::ofstream outfile;
    std::string current_line;
    std::vector<std::string> out;
    std::vector<std::string> out_trimmed;
    int counted = 0;
    int counter = 0;
    int terminal = 0;
        
    

    for( Node * n : root->children){

        file.open(filename.c_str());
        file.seekg(0, std::ios::beg);

        //If the root is the actual root node, We need to read  input file as the original table.
        if(root->group.size() >= 15){
            while(std::getline(file,current_line)){
                tokenize(current_line, out);

                counter++;
                if(counter%100000 == 0)
                    std::cout<<"Still working.... ("<<counter<<")\n";

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
            terminal = 0;

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
            char buffer[20];
            sprintf(buffer,"%d",resultset.size());
            std::string temp_filename ="";

            for(char c : buffer){
                temp_filename = temp_filename + c;
            }

            temp_filename += ".t";
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

            //for(Node * m : n->children){
                OptimalSolution(n, terminals, temp_filename);
            //}

        }

    resultset.clear();
    file.close();
    file.clear();

    }

    //Delete temp files once execution is complete.
    //system("DEL t_*.txt");

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
