#ifndef __ARRAY_H__
#define __ARRAY_H__

/***************************************************************************
 * Name: array.h (classe tableau)
 * Author: BARRET
 * Source: G++
 * Purpose: classe Array testee dans le programme pricipal mef.cpp
 * Build: voir makefile
 * History: 12062023
 ***************************************************************************/

#include <iostream>
#include <string>
#include <cassert>



class Array{
public:
    int m,n;//row size, column size
    double **x;//pointer to a pointer to a double 
    
    Array(/**/):Array(1,1){};//default constructor
    Array(int sizeX, int sizeY);
    Array(const Array& tab);//copy constructor
    ~Array();//destructor
    Array& operator=(const Array& tab);//copy assignment
    
    void display(const char*name);
    
private:
    void allocArrays();//method to allocate the array x and the x[i] arrays

};




void Array::display(const char* name)
{
    std::cout << "\n\t" << std::endl;
    std::cout << name << std::endl;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            std::cout << x[i][j] << " ";
        }
        std::cout << "\n";        
    }
    std::cout << "\n";
}



void Array::allocArrays()
{
    x = new double *[m];
    for(int i=0;i<m;i++){
        x[i] = new double[n];
    }
}

Array::Array(int sizeX, int sizeY):m(sizeX),n(sizeY)
{
    allocArrays();
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            x[i][j] = 0.;
        }
    }
}

Array::Array(const Array& tab):m(tab.m),n(tab.n)
{
    allocArrays();
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            x[i][j] = tab.x[i][j];
        }
    }
}

Array::~Array()
{
    for(int i=0;i<m;i++){
        delete [] x[i];
    }
    delete []x;
}

Array & Array::operator=(const Array& tab)
{
    if(this == &tab){
        //avoid self assignment
        return (*this);
    }else{
        if(m != tab.m || n != tab.n){
            this->~Array();
            m = tab.m;
            n = tab.n;
            allocArrays();
        }
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                x[i][j] = tab.x[i][j];
            }
        }
        return (*this);
    }

}







#endif /**/
