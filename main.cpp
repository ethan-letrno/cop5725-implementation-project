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
#include "build_preliminary_solution.h"


int main() {
    
    std::vector<std::vector<std::string>> elements = {
        {"ORDERKEY"},
        {"SHIPINSTRUCT"},
        {"SHIPMODE"},
		{"ORDERKEY", "SHIPINSTRUCT"},
	};
    
    std::string table = "lineitem184k.table";
    
    Node * G_prime = build_preliminary_solution(elements, table);
    //topdown split (G'.root, k)
    
    
}