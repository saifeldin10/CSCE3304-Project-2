# CSCE3304-Project-2
This is the github repo for the CSCE 3304 Digital Design II course. These are the files for the second project.

In order to use this code, copy the .cpp and .h files into an empty Visual Studio C++ Project and run the Local 
Windows Debugger. Make sure to have the input file in the same Project folder titled "input.txt".

The "input.txt" file must not have any unecessary spaces.
Example: net1 (1, 2 ,3) (1, 5, 6) (2, 3, 8) <-- This is an example of a good input line.
Make sure there are no spaces at the end of each line or any empty lines in the input file.
In order to change the file being processed, go to line 250 in file "main.cpp" and change the name of the file 
between the quotation marks.

Assumptions:

- If we wish to create a connection between Net1 and Net2, the source coordinates for Net1 and Net2 need to be 
empty and available.
- The difference in layer numbers in the same net should not exceed more than 1. 
  e.g. net1 (1, 2 ,3) (2, 5, 6) (3, 3, 8) is a valid input
       net1 (1, 2 ,3) (4, 5, 6) (2, 3, 8) is not a valid input
