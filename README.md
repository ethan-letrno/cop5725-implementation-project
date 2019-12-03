# COP5725 FALL 2019 PROJECT 

Implementation in C++ of the algorithms described in the 2016 ICDE paper "A Novel, Low-latency Algorithm for Multiple Group-By Query Optimization".

Worked on by Ethan Letourneau and Daniel Kane.

"Installing" the programs is a matter of just downloading the repository and extracting the files to a common folder, along with the file "2000000.table" given to the instructor via email or USB.

To compile the naive solution, run this command: "g++ -std=c++11 naive.cpp"
To run the naive solution, edit first the files "elements" 2D string vector to include the groupings you desire to be computed - the groupings used in our experiments are detailed in groups.txt. They are in order 1-28 in the 2D vector in the main function. Then recompile and run "a.out" or "a.exe" depending on your machine.

To compile the optimal solution, run "make" executing the makefile.
To run the optimal solution, alter the "elements" 2D string vector in "main.cpp" similarly to the above instruction. Both the naive and optimal solutions will output "Still working..." every 100,000 lines it processes so that you know it hasn't frozen. The result files will be outputted to the CWD, and are of the format of the files in the "results" folder.
