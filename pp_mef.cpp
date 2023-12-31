#include <iostream>
#include "array.h"
#include "mesh.h"

#include <fstream>
#include <vector>
#include <cmath>
#include <cassert>
#include <string>
#include "functions.h"

using namespace std;

enum DIM{NX0=2,NX1=3,NX2=4,NX3=5,NX4=6};




int main(){
    cout << "\n\t\tResolution par la MEF 2D" << endl;
    cout << "\t\t------------------------\n" << endl;
    
    
    std::cout << "\n START TESTS mesh class." << "\n\n";
    
    const char* filename = "data.txt";//"maillage60.txt";
    Mesh msh(filename);
    
    std::filebuf fb_msh;
    fb_msh.open("ecriture_msh.txt",std::ios::out);
    std::ostream os_msh(&fb_msh);
    os_msh << msh.nbs << "," << msh.nbt << "\n";
    
    for(int k=0;k<msh.nbs;k++){
        os_msh << msh.vertices[k] << "\n";        
    }
    dble aire = 0.0;
    for(int k=0;k<msh.nbt;k++){
        os_msh << "aire(" << k+1 << ")=" << msh.triangle[k].area << "\n";
        aire += msh.triangle[k].area;
    }
//     std::cout << "Somme de toutes les aires = " << aire << std::endl;
//     os_msh << "Somme des aires = " << aire ;
    
    std::cout << "\n END TESTS mesh class." << "\n\n\n\n\n";
    std::cout << "////////////////////////////////////////////////////////////" << std::endl;
    
    
    msh.vertices[3].display();
    
    R2 A0(msh.vertices[1].p),B0(msh.vertices[2].p), C0(msh.vertices[3].p);
    A0.print();B0.print();C0.print();
    R2 AB0(A0,B0), AC0(A0,C0), BC0(B0,C0);
    dble surface0 = AB0^AC0*0.5;
    std::cout << "surface = " << surface0 << std::endl;
    
    
    Array x(5,1);
    x.setCoef(0,0,-4);x.setCoef(1,0,2);x.setCoef(2,0,1);x.setCoef(3,0,0);x.setCoef(4,0,-10);
    vector<double>vx;
    vx.push_back(-4);vx.push_back(2);vx.push_back(1);vx.push_back(0);vx.push_back(-10);
    x.display("x");
    double aa(-8),bb(7);
    double valeurMax = MAX(aa,bb);
    double valeurMin = MIN(aa,bb);
    std::cout << "\tvaleurMax =" << valeurMax << ", valeurMin = " << valeurMin << "\n";
    double sommeDesValeursDuVecteur_x;
    sommeDesValeursDuVecteur_x = sum(x,0,x.getRowSize());
    std::cout << "\tsomme des valeurs du vecteur x = " << sommeDesValeursDuVecteur_x << "\n";

    
    
    
    /*
     * 
     * M.E.F 2D (P1 conforme)
     * 
     */
    std::cout << "\n\n\n\n";
    std::cout << "\t\t\t\tBEGINNING MEF" << std::endl;
    

    std::cout << "\t\t\t\tENDING    MEF" << std::endl;
    std::cout << "\n\n\n\n";
    
    
    
    
    
    std::filebuf fb;
    fb.open ("results.txt",std::ios::out);
    std::ostream os(&fb);
    
    /*
     * Ecriture des tests de la classe R2 dans le fichier results.txt
     */  
    vector<R2>vR2;
    R2 A,B(1,0),C(1,1),D(0,1),E(C),F,G,H,I;
    
    vR2.push_back(A);
    vR2.push_back(B);
    vR2.push_back(C);
    vR2.push_back(D); 
    vR2.push_back(E);
    F = B + C;
    vR2.push_back(F);
    G = F - B;
    vR2.push_back(G);
    H += C;
    vR2.push_back(H);
    I = B*D;
    vR2.push_back(I);
    dble dot_AB = B.operator,(C);
    dble vec_AB = E.operator^(F);
    for(auto pts:vR2){
        os << pts << "\n";
    }
    os << dot_AB << "\n";
    os << vec_AB << "\n";
    
    Vertex v1(1,C,3),v2(2,F,1),v3(3,D,0);
    vector<Vertex>vv;
    vv.push_back(v1);
    vv.push_back(v2);
    vv.push_back(v3);
    for(auto vtx:vv){
        os << vtx << "\n";
    }
    

    


    
    int NX;
    
    
    typedef Array mat;
    int r(NX0);
    switch(r)
    {
        case NX0:
            NX = NX0;
            break;
        case NX1:
            NX = NX1;
            break;
        case NX2:
            NX = NX2;
            break;
        case NX3:
            NX = NX3;
            break;
        case NX4:            
            NX = NX4;
            break;
        default:
            cout << "Error !" << endl;
            break;
    }
    
    cout << "--> nbre de ligne (ou de colonne) de la matrice carree = " << NX << endl;
    
    
    
    
    
    
    
    
    
    
    
    
    
    mat M(NX,NX), elemM_k(3,3);
    
    M.display("M");
    
    elemM_k.display("elemM_k");
    

    

    
    
    
    
    return 0;
}


