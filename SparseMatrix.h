#ifndef _SPARSE_MATRIX_H
#define _SPARSE_MATRIX_H
#include<iostream>
#include <fstream>
using namespace std;
  

class SparseMatrix{
    private:
    //col
    struct Col{
        int col_num;
        double value;
        Col *col_next;
    };
    //makes new colunm
    Col *make_col(int col_num,double value,Col *next_c){
            Col *c=new Col;
            c->col_num=col_num;
            c->value=value;
            c->col_next=next_c;
            return c;
    }
    //row
    struct Row{
        int row_num;
        Row *row_next;
        Col *col_h=nullptr;
    };
    //makes a new node
     Row * make_row(int row_num,Row *next_r,Col *c_head){
            Row *r=new Row;
            r->row_num=row_num;
            r->row_next=next_r;
            r->col_h=c_head;
            return r;
        }
    //head
    Row *row_head;
    Row *inverse_row_head; 
    public:
        //find the row.(change r to row found and also return true or false if found)
        bool find_row(int row_num,Row *&head,Row *&r);
        //find the colum then return bool and value
        bool find_col(int row_num,int col_num,Row *&head,Col *&c);
        //add make new row then add to rows (return row head)
        void add_to_rows(int row_num,Row *&row_head,Row *&row);
        //add colunm to row (returns col head)
        void  add_to_cols(int col_num,double value,Col *&col_head);
        //add to rows, colunm or both(returns row header).
        void add(int row_num,int col_num,double value,Row *&row_head);
        //makes sparse matrix
        SparseMatrix();
        //helper
        void print_file(ostream &file);
        void copy(Row *&row,Row *& head);
        void column_travers(Col *c1,Col *c2,int row1,Row *&head);
        void combined_matrixs(Row *ptr1,Row *ptr2);
        SparseMatrix convert_negative();
        void multiply_matrixs(Row *Curr_row,Row *inverse_head);
        //operation overload functions
        friend istream &operator >> (istream &file,SparseMatrix &s);
        friend ostream &operator << (ostream &file,SparseMatrix &s);
        friend SparseMatrix operator+ (SparseMatrix &s1,SparseMatrix &s2);
        friend SparseMatrix operator- (SparseMatrix &s1,SparseMatrix &s2); 
        friend SparseMatrix operator* (SparseMatrix &s1,SparseMatrix &s2);
        void print_matrix();
};

#endif