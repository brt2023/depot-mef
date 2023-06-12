#include "array.h"
#include <iostream>

using namespace std;




int main(){
    cout << "\n\ttests MEF 2D..." << endl;

    Array *mat = new Array[6];
    Array A(2,3),B(A),C,D(3,3);
    A.display("A");
    B.display("B");
    C.display("C");
    D.display("D");

    

    
    
    
    return 0;
}
