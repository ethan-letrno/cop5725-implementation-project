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
#include "build_preliminary_solution.h"
#include "tds.h"


int main() {
    
    std::vector<std::vector<std::string>> elements = {
        {"ORDERKEY"},
        {"SHIPINSTRUCT"},
        {"COMMITDATE"}, 
		//{"ORDERKEY", "SHIPINSTRUCT"},
        {"ORDERKEY", "SHIPINSTRUCT", "COMMITDATE"}
	};
    
    std::string table = "184000.table";
    
    Node * G_prime = build_preliminary_solution(elements, table);
    TopDownSplit(G_prime, 2, table, G_prime);
    
    preord(G_prime);
    
    
}