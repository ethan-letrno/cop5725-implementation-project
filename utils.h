#ifndef _UTILS_H

struct Node 
{ 
    std::vector< int > group; 
   	std::vector<Node *>children; 
};

Node *newNode(std::vector< int > group);
bool sortbysecdesc(const std::pair<std::vector<int>,double> &a, const std::pair<std::vector<int>,double> &b);
void preord(Node* root);
void tokenize(std::string const &str, std::vector<std::string> &out);
double estimateCardinality(std::vector<int> indices, std::string filename);
double sortCost(std::vector<int> indices, std::string filename);
double scanCost(std::vector<int> indices, std::string filename);
int getColumnForLineitemElement(std::string element);


#endif