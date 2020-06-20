clear: clear
code: p4.cpp SparseMatrix.cpp
	g++ p4.cpp SparseMatrix.cpp
code1:	p4.cpp new_SparseMatrix.cpp
	g++ p4.cpp new_SparseMatrix.cpp 
test: code
	./a.out add a.txt b.txt c.txt
test1: code
	./a.out sub a.txt b.txt c.txt	
test2: code
	./a.out mul a.txt b.txt c.txt
test3:	
	./a.out add a2.txt b2.txt c2.txt
all: code test test1 test2		 	