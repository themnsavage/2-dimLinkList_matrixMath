#include <iostream>
#include <fstream>
#include "SparseMatrix.h"
using namespace std;

int main(int argc, char **argv) {
    
    if (argc < 5) {
        cerr << "Not enough arguments" << endl;
        return 5;
    }

    SparseMatrix m1, m2, m;
    ifstream infile1, infile2;

    infile1.open(argv[2]);
    if (infile1.fail()) {
        cerr << "Unable to open " << argv[2] << endl;
        infile1.clear();
        return 2;
    }

    infile2.open(argv[3]);
    if (infile2.fail()) {
        cerr << "Unable to open " << argv[3] << endl;
        infile2.clear();
        return 3;
    }

    infile1 >> m1;
    infile2 >> m2;
    infile1.close();
    infile2.close();
    m2.print_matrix();
    string op(argv[1]);
    if (op=="add") {
        m=m1+m2;
    }
    else if (op=="sub") {
        m=m1-m2;
    }
    else if (op=="mul") {
        m=m1*m2;
    }
    else {
        cerr << "No such operation" << endl;
        return 1;
    }
    
    ofstream outfile;
    outfile.open(argv[4]);
    if (outfile.fail()) {
        cerr << "Unable to open " << argv[4] << endl;
        outfile.clear();
        return 4;
    }
    outfile << m;

    outfile.close();

    return 0;
}