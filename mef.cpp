#include "array.h"
#include <iostream>

using namespace std;




int main(){
    cout << "\n\ttests MEF 2D..." << endl;

    Array *mat = new Array[6];
    Array A(2,3),B(A),C,D(3,3),E(A),tE,F,G(2,2),H(G),I(2,2),J(I);
    A.setCoef(1,2,-10.);
    A.display("A");
    B.setCoef(0,0,3.);B.setCoef(1,0,-7.);
    B.setCoef(0,1,4.);B.setCoef(1,1,1);
    B.setCoef(0,2,-5);B.setCoef(1,2,3);
    B.display("B");
    C = A.operator+(B);
    C.display("C");
    D.display("D");
    E += B;
    E.display("E");
    
    tE = E.transpose();
    tE.display("transp(E)");
    
    F = E - C;
    F.display("F");

    G.setCoef(0,0,1);G.setCoef(0,1,-1);
    G.setCoef(1,0,3);G.setCoef(1,1,5);
    G.display("G");
    H += G;
    H -= G;
    H.display("H");
    I.setCoef(0,0,1);I.setCoef(0,1,1);
    I.setCoef(1,0,0);I.setCoef(1,1,1);
    I.display("I");
    J = I*I;
    J.display("J");
    Array K = I.transpose()*I, L(2,2),M(2,2),N(2,2);
    K.display("K");
//     cout << "trace(F)=" << F.trace() << endl;
    cout << "K22=" << K.getCoef(1,1) << endl;
    L = K;    
    L = L/2;
    L.display("L/2");
    N = I;
    cout << I.operator==(J) << endl;
    cout << I.operator==(N) << endl;
    
    
    
    return 0;
}
