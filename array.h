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
#include "math.h"



class Array{
public:
    int m,n;//row size, column size
    double **x;//pointer to a pointer to a double 
    
    Array(/**/):Array(1,1){};//default constructor
    Array(int sizeX, int sizeY);
    Array(const Array& tab);//copy constructor
    ~Array();//destructor
    Array& operator=(const Array& tab);//copy assignment
    
    
    Array operator+ (const Array& tab);
    Array operator- (const Array& tab);
    Array operator* (const Array& tab);
    Array operator* (const double& c );
    Array operator/ (const double& c );
    Array operator+=(const Array& tab);
    Array operator-=(const Array& tab);
    bool  operator==(const Array& tab);
    double& operator()(int i,int j);
    
    Array transpose ();
    double trace();
    
    void setCoef(int i,int j, double val);
    double getCoef(int i, int j);
    
    void display(const std::string& name);
    
    Array comat(int io, int jo);//matrice dont on a supprime la ieme ligne(io) et la jeme colonne(jo)
    double det();
    
    Array inv();//matrice inverse
    
private:
    void allocArrays();//method to allocate the array x and the x[i] arrays

};


/*
 * Inverse d'une matrice
 */
Array Array::inv()
{
    Array tmp(m,n),coMat(m,n);
    Array delta_ij(m-1,n-1),A_ij(m-1,n-1);
    assert(this->det() != 0.0);
    if(m==1){
        double val = this->getCoef(0,0);
        coMat.setCoef(0,0,val);
    }else{
        //recursivite
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                delta_ij = this->comat(i,j);
                A_ij = delta_ij.operator*(pow(-1,i+j)); 
                double val = A_ij.det();
                coMat.setCoef(i,j,val);
            }
        }        
    }
    tmp = coMat.transpose().operator/(this->det());
    return tmp;
}



Array Array::operator*(const double& c)
{
    Array tmp(m,n);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            tmp.x[i][j] = c*x[i][j];
        }
    }
    return tmp;    
}




double Array::det()
{
    // Developpement du determinant par la formule des cofacteurs
    int io = 0;//on developpe par rapport a la 1ere ligne de la sous-matrice.
    
    Array A(m,n),coMat(m-1,n-1),cofact(m-1,n-1),delta_ioj(m-1,n-1),A_ioj(m-1,n-1);
    double a_ioj, d = 0.0; 
    if(m==1){
        return this->x[0][0];
    }else{
        //recursivite
        for(int j=0;j<n;j++){
            delta_ioj = this->comat(io,j);
            A_ioj = delta_ioj.operator*(pow(-1,io+j));            
            a_ioj = this->getCoef(io,j);
            d += a_ioj*(A_ioj.det());
        }        
    }
    return d;
}


Array Array::comat(int io, int jo)
{
    assert(m == n);//matrice carree de taille m
    Array comat(m-1,n-1), delta(m-1,n-1);
    int k(0),l;
    for(int i=0;i<m;i++){
        if(i != io){            
            l = 0;
            for(int j=0;j<n;j++){
                if(j != jo){
                    double val = this->getCoef(i,j);
                    delta.x[k][l] = val;                    
                    l++;
                }
            }            
            k++;
        }        
    }
    comat = delta;
    return comat;    
}


double & Array::operator()(int i, int j)
{
    assert(i<m);assert(j<n);
    return x[i][j];
}


double Array::trace()
{
    assert(m==n);
    double s(0.0);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            if(i == j){
                s += x[i][i];
            }            
        }        
    }
    return s;
}


void Array::setCoef(int i, int j, double val)
{
    assert(i<m);assert(j<n); 
    x[i][j] = val;
}
double Array::getCoef(int i, int j)
{
    assert(i<m);assert(j<n);
    return x[i][j];
}



Array Array::transpose()
{
    Array t(n,m);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            t.x[j][i] = x[i][j];
        }
    }
    return t;
}


Array Array::operator+(const Array& tab)
{ 
    Array tmp(tab.m,tab.n);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            tmp.x[i][j] = x[i][j] + tab.x[i][j];
        }
    }
    return tmp;
}

Array Array::operator-(const Array& tab)
{ 
    Array tmp(tab.m,tab.n);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            tmp.x[i][j] = x[i][j] - tab.x[i][j];
        }
    }
    return tmp;
}

Array Array::operator*(const Array& tab)
{
    assert(n==tab.m);
    Array tmp(tab.m,tab.n);
    for(int i=0;i<m;i++){        
        for(int j=0;j<n;j++){            
            double s(0.0);
            for(int k=0;k<tab.m;k++){
                s += x[i][k]*tab.x[k][j];
            }
            tmp.setCoef(i,j,s);  
        }        
    }
    return tmp;
}

Array Array::operator/(const double& c)
{
    assert(c != 0);
    Array tmp(m,n);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            tmp.x[i][j] = x[i][j]/c;
        }
    }
    return tmp;
}

bool Array::operator==(const Array& tab)
{
    bool test = true;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            if(x[i][j] != tab.x[i][j]){
                test = false;
                break;
            }
        }
    }

    return test;
}




Array Array::operator+=(const Array& tab)
{
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
           x[i][j] += tab.x[i][j];
        }
    }
    return(*this);
}

Array Array::operator-=(const Array& tab)
{
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
           x[i][j] -= tab.x[i][j];
        }
    }
    return(*this);
}




void Array::display(const std::string& name)
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
