#include "array.h"
#include <iostream>
#include <fstream>
#include <vector>



using namespace std;




int main(){
    cout << "\n\ttests MEF 2D..." << endl;

    
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
    I.setCoef(1,0,-1);I.setCoef(1,1,2);
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
    cout << L(0,1) << endl;
    
    
    cout << "-------------" << endl;
    typedef Array Matrix;
    Matrix matA(3,3),matB(3,3),comatA, matC(matB),matD(4,4);
    
    matA(0,0)=1;matA(0,1)=-3;matA(0,2)=5;
    matA(1,0)=-8;matA(1,1)=10;matA(1,2)=2;
    matA(2,0)=4;matA(2,1)=1;matA(2,2)=-7;
    
    vector<Matrix> vMat;
    vMat.push_back(matA);
    vMat.push_back(matB);
 
    int k = 0;
    for(auto mat:vMat){
        std::string is = to_string(k);
        mat.display(is);
        k++;
    }
    
    comatA = matA.comat(2,0);
    comatA.display("comatA(2,0)");
    
    cout << comatA.det() << endl;
    cout << matA.det() << endl;
    
    matC(0,0)=2;matC(0,1)=1;matC(0,2)=3;
    matC(1,0)=1;matC(1,1)=1;matC(1,2)=1;
    matC(2,0)=0;matC(2,1)=0;matC(2,2)=1;
    cout << matC.det() << endl;
    matC.inv().display("inv(C)");
    
    
    matD(0,0)=5;matD(0,1)=1;matD(0,2)=2;matD(0,3)=7;
    matD(1,0)=3;matD(1,1)=0;matD(1,2)=0;matD(1,3)=2;
    matD(2,0)=1;matD(2,1)=3;matD(2,2)=4;matD(2,3)=5;
    matD(3,0)=2;matD(3,1)=0;matD(3,2)=0;matD(3,3)=3;
    matD.display("matD");
    cout << matD.det() << endl;
    
    // matrice inverse
    matD.inv().display("inv(matD)");
    
    I.inv().display("inv(I)");
    Array inv_I(2,2),Id;
    inv_I = I.inv();
    Id = I*inv_I;
    Id.display("Id = I*inv(I)");
    
    /*
     * 
     */
        /*
     * Affichage des resultats
     */
    
//     std::filebuf fb;
//     fb.open ("results.txt",std::ios::out);
//     std::ostream os(&fb);
//     os << A << "," << B << "," << C << "," << D << "\n";

//     std::ofstream ff("tests_comatA.txt");
//     comatA.sauvegarde(ff);
    
    
    return 0;
}
