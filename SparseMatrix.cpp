#include <iostream>
#include <fstream>
#include "SparseMatrix.h"
struct Row;
struct Col;
using namespace std;
//row and col functions//

//find row
bool SparseMatrix::find_row(int row_num,Row *&head,Row *&r){
    Row *ptr=head;
    while(ptr!=nullptr){
        if(ptr->row_num==row_num){
            r=ptr;
            return true;
        }
        ptr=ptr->row_next;
    }
    return false;
}

//find colunm
bool SparseMatrix::find_col(int row_num,int col_num,Row *&head,Col *&c){
    Row *r=nullptr;
    //see if the row exist
    if(find_row(row_num,head,r)){
        Col *ptr=r->col_h;
        //loops through colunms and see if exist
        while(ptr!=nullptr){
            if(ptr->col_num==col_num){
                c=ptr;
            }
            ptr=ptr->col_next;
        }
    }
    //return flase due to doesn't exist
    c=nullptr;
    return false;
}

//add row
void SparseMatrix::add_to_rows(int row_num,Row *&row_head,Row *&row){
    Row *ptr=row_head;
    Row *prev=nullptr;
    //if empty
    if(ptr==nullptr){
        Row *r= make_row(row_num,nullptr,nullptr);
        row_head=r;
        row=r;
    }
    //if not empty
    else{
        while(ptr!=nullptr){
            if(row_num > ptr->row_num){
                prev=ptr;
                ptr=ptr->row_next;
            }
            else{
                Row *r= make_row(row_num,ptr,nullptr);
                if(prev==nullptr){
                    row_head=r;
                }
                else{
                    prev->row_next=r;
                    row=r;
                }
                row=r;
                break;
            }
        } 
        //row at the end
        if(ptr==nullptr){
            Row *r= make_row(row_num,nullptr,nullptr);
            if(prev!=nullptr){
                prev->row_next=r;
            }
            else{
                row_head=r;
            }
            row=r;
        }   
    }
}

//add colunm
void  SparseMatrix::add_to_cols(int col_num,double value,Col *&col_head){
    Col *ptr=col_head;
    Col *prev=nullptr;
    if(ptr==nullptr){
        Col *c=make_col(col_num,value,nullptr);
        col_head=c;
    }
    //if it is not empty
    else{
        while(ptr!=nullptr){
            if(col_num > ptr->col_num){
                prev=ptr;
                ptr=ptr->col_next;
            }
            else{
                Col *c=make_col(col_num,value,ptr);
                if(prev==nullptr){
                    col_head=c;
                }
                else{
                    prev->col_next=c;
                }
                break;
            }
        }
        //if it goes to the end
        if(ptr==nullptr){
            Col *c=make_col(col_num,value,nullptr);
            if(prev!=nullptr){
                prev->col_next=c;
            }
            else{
                col_head=c;
            }
        }
    }
}

//add function
void SparseMatrix::add(int row_num,int col_num,double value,Row *&row_head){
    Row *row=nullptr;
    //see if row exists
    if(!(find_row(row_num,row_head,row))){
        add_to_rows(row_num,row_head,row);
    }
    //add to row with colunm
    add_to_cols(col_num,value,row->col_h);
}
//makes sparse matrix object
SparseMatrix::SparseMatrix(){
    row_head=nullptr;
    inverse_row_head=nullptr;
}
//helper

//copy row
void SparseMatrix::copy(Row *&row,Row *& head){
    Col *ptr=row->col_h;
    while(ptr!=nullptr){
        add(row->row_num,ptr->col_num,ptr->value,head);
        ptr=ptr->col_next;
    }
}

//print out matrix
 void SparseMatrix::print_file(ostream &file){
    Row *h=row_head;
    Col *c;
    while(h!=nullptr){
        c=h->col_h;
        while(c!=nullptr){
            file<<h->row_num<<" ";
            file<<c->col_num<<" "<<c->value<<endl;
            c=c->col_next;
        }
        h=h->row_next;
    }
    file<<"0 0 0"<<endl;
 }
 //go though columns
void SparseMatrix::column_travers(Col *c1,Col *c2,int row_num,Row *&head){
    //base case if both are nullptr (at end)
    if(c1 == nullptr && c2 == nullptr){
        return;
    }
    //see which one to add
    else{
        //if c1 is at the end
        if(c1==nullptr){
                add(row_num,c2->col_num,c2->value,head);
                column_travers(c1,c2->col_next,row_num,head);
            }
        //c2 is at end nullptr
        else if(c2==nullptr){
            add(row_num,c1->col_num,c1->value,head);
            column_travers(c1->col_next,c2,row_num,head);
        }
        else{
            //if they are the same
            if(c1->col_num==c2->col_num){
                double new_value;
                new_value=(c1->value) + (c2->value);
                if(new_value!=0){
                    add(row_num,c1->col_num,new_value,head);
                }
                column_travers(c1->col_next,c2->col_next,row_num,head);
            }
            //if c1 is smaller
            else if(c1->col_num < c2->col_num){
                add(row_num,c1->col_num,c1->value,head);
                column_travers(c1->col_next,c2,row_num,head);
            }
            //if c2 is smaller
            else if(c2->col_num < c1->col_num){
                add(row_num,c2->col_num,c2->value,head);
                column_travers(c1,c2->col_next,row_num,head);
            }
        }
    }
}
 //add to matrix
 void SparseMatrix::combined_matrixs(Row *ptr1,Row *ptr2){
     //base case if both are nullptr (at end)
    if(ptr1== nullptr && ptr2 == nullptr){
        return;
    }
    //see which one to add
    else{
        //if c1 is at the end
        if(ptr1==nullptr){
            copy(ptr2,row_head);
            combined_matrixs(ptr1,ptr2->row_next);
        }
        //c2 is at end nullptr
        else if(ptr2==nullptr){
            copy(ptr1,row_head);
            combined_matrixs(ptr1->row_next,ptr2);
        }
        else{
            //if they are the same
            if(ptr1->row_num==ptr2->row_num){
                column_travers(ptr1->col_h,ptr2->col_h,ptr1->row_num,row_head);
                combined_matrixs(ptr1->row_next,ptr2->row_next);
            }
            //if c1 is smaller
            else if(ptr1->row_num < ptr2->row_num){
                copy(ptr1,row_head);
                combined_matrixs(ptr1->row_next,ptr2);
            }
            //if c2 is smaller
            else if(ptr2->row_num < ptr1->row_num){
                copy(ptr2,row_head);
                combined_matrixs(ptr1,ptr2->row_next);
            }
        }
    }
 }

//makes negative matrixs
SparseMatrix SparseMatrix::convert_negative(){
    SparseMatrix negative_matrix;
    Row *&ptr=row_head;
    while(ptr!=nullptr){
        Col *&c=ptr->col_h;
        while(c!=nullptr){
            double num=(c->value)*-1;
            add(ptr->row_num,c->col_num,num,negative_matrix.row_head);
            c=c->col_next;
        }
        ptr=ptr->row_next;
    }
    return negative_matrix;
}
//multiply the matrix
void SparseMatrix::multiply_matrixs(Row *curr_row,Row *inverse_head){
    //base case
    if(curr_row==nullptr){
        return;
    }
    else{
        double value=0;
        Row *col_head=inverse_head;
        Col *c1;
        Col *c2;
        while(col_head!=nullptr){
            //reset
            c1=curr_row->col_h;
            while(c1!=nullptr){
            //reset
            c2=col_head->col_h;
                //find match col num
                while(c2!=nullptr){
                    if(c1->col_num==c2->col_num){
                        value+=(c1->value)*(c2->value);
                    }
                    c2=c2->col_next;
                }
            c1=c1->col_next;    
            }
            if(value!=0){
                add(curr_row->row_num,col_head->row_num,value,row_head);
                multiply_matrixs(curr_row->row_next,inverse_head);
            }
            col_head=col_head->row_next;
        }
    }
    multiply_matrixs(curr_row->row_next,inverse_head);
}
//read in matrix from file
istream &operator>>(istream &file,SparseMatrix &s){
    string num;
    int part=1;
    int row;
    int col;
    double value;
    while(file>>num){
        if(part==1){
            row=stoi(num);
            part++;
        }
        else if(part==2){
            col=stoi(num);
            part++;
        }
        else if(part==3){
            value=stod(num);
            part=1;
            if(value!=0){
                s.add(row,col,value,s.row_head);
                s.add(col,row,value,s.inverse_row_head);
            }
        }
    }
    return file;
} 

//read out new matrix to file   
ostream &operator<<(ostream &file,SparseMatrix &s){
    s.print_file(file);
    return file;
}
//adds two S matrix 
SparseMatrix operator+ (SparseMatrix &s1,SparseMatrix &s2){
    SparseMatrix new_s;
    new_s.combined_matrixs(s1.row_head,s2.row_head);   
    return new_s; 
}
//minus two S matrix
SparseMatrix operator- (SparseMatrix &s1,SparseMatrix &s2){
    SparseMatrix new_s;
    SparseMatrix negative;
    negative=s2.convert_negative();
    new_s.combined_matrixs(s1.row_head,negative.row_head);
    return new_s;
}
//times two S matrix
SparseMatrix operator* (SparseMatrix &s1,SparseMatrix &s2){
    SparseMatrix new_s;
    new_s.multiply_matrixs(s1.row_head,s2.inverse_row_head);
    return new_s;
}
void SparseMatrix::print_matrix(){
    Row *h=inverse_row_head;
    Col *c;
    while(h!=nullptr){
        c=h->col_h;
        while(c!=nullptr){
            cout<<h->row_num<<" ";
            cout<<c->col_num<<" "<<c->value<<endl;
            c=c->col_next;
        }
        h=h->row_next;
    }
}