#ifndef _BUILD_PRELIMINARY_SOLUTION_H
struct Node 
{ 
    std::vector< int > group; 
   	std::vector<Node *>children; 
};

Node * build_preliminary_solution(std::vector< std::vector< std::string > >, std::string);

#endif