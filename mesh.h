#ifndef __MESH_H__
#define __MESH_H__

#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <limits>
#include <cassert>
#include "array.h"

#include "functions.h"

typedef double dble;



// [Kel, Mel, V_rotV_x_el,V_rotV_y_el] = mat_elem(S1, S2, S3)





/********** class Label **********/
/*
 * Reference des numeros de triangle, de sommet
 */
class Label{
public:
    int num;
    Label(int nnum=0):num(nnum){};
    
    friend std::ostream& operator<<(std::ostream& f, const Label& lab){
        f << lab.num;
        return f;        
    }
    friend std::istream& operator>>(std::istream& f, Label& lab){
        f >> lab.num;
        return f;
    }

};/*END class Label */





/********** class R2 **********/

class R2{
public:    
    dble x,y;
    R2(/**/):x(0.0),y(0.0){}
    R2(dble xx,dble yy):x(xx),y(yy){}
    R2(const R2&p) = default;//copy constructor
    R2(R2 p1,R2 p2):x(p2.x-p1.x),y(p2.y-p1.y){}
    R2& operator=(const R2&p) = default;//copy by assignment
    
    R2  operator+ (const R2&p);
    R2  operator- (const R2&p);
    R2  operator* (const R2&p);
    R2& operator+=(const R2&p);
    R2& operator-=(const R2&p);
    R2  operator* (const dble& c);
    dble operator,(const R2&p);//dot product
    dble operator^(const R2&p);//vector product
    
    
    
    friend std::ostream& operator<<(std::ostream& f, const R2& p){
        f << p.x << " " << p.y;
        return f;
    }
    friend std::istream& operator>>(std::istream& f, R2& p){
        f >> p.x >> p.y;
        return f;
    }    
    void print(){
        std::cout << "(" << x << "," << y << ")" << std::endl;
    }
  
};/*END class R2 */

R2 R2::operator*(const dble& c)
{
    return R2(c*x,c*y);
}


dble R2::operator^(const R2& p)
{
    return (x*p.y - y*p.x);
}
dble R2::operator,(const R2& p)
{
    return (x*p.x + y*p.y);
}
R2 R2::operator*(const R2& p)
{
    return R2(x*p.x,y*p.y);
}
R2 & R2::operator+=(const R2& p)
{
    x += p.x;
    y += p.y;
    return (*this);
}
R2 & R2::operator-=(const R2& p)
{
    x -= p.x;
    y -= p.y;
    return (*this);
}
R2 R2::operator+(const R2& p)
{
    return R2(x+p.x,y+p.y);
}
R2 R2::operator-(const R2& p)
{
    return R2(x-p.x,y-p.y);
}



/********** class Vertex **********/

class Vertex: public R2,public Label{
public:
    int ns;
    int neb;
    R2 p;    // affichage du vecteur vv

    Vertex(/**/){};
    Vertex(int nns, R2 pp, int nneb):ns(nns),p(pp),neb(nneb){};
    void display();
    
    friend std::ostream& operator<<(std::ostream& f,const Vertex& v){
        f << v.ns << " " << v.p << " " << v.neb;
        return f;
    }
    friend std::istream& operator>>(std::istream& f, Vertex& v){
        f >> v.ns >> v.p >> v.neb;
        return f;
    }

};/*END class Vertex */

void Vertex::display()
{
    std::cout << ns << "," << p << "," << neb << std::endl;
}










/********** class Triangle **********/
class Triangle:public Label{
public:
    dble area;
    Vertex *vertices[3];
    
    Triangle(/**/){};
    Triangle(int numtri):Label(numtri){}
    void set(Vertex*v0,int i,int iA,int iB,int iC,int j);
    Vertex & operator[](int& i){
        assert(i>=0);assert(i<3);
        return *vertices[i];        
    }
    
};/*END class Triangle */


void Triangle::set(Vertex* v0, int i, int iA, int iB, int iC, int j)
{
    dble c = 0.5;
    vertices[0] = v0 + iA;
    vertices[1] = v0 + iB;
    vertices[2] = v0 + iC;
    num = j;

    R2 AB(vertices[0]->p , vertices[1]->p);
    R2 AC(vertices[0]->p , vertices[2]->p);
    area = AB^AC*c;
    assert(area>=0);
}







class Mesh{
public:
    int nbs,nbt;
    dble area = 0.0;

    Vertex *vertices;
    Triangle *triangle;
    Mesh(/**/){};
    Mesh(const char * filename);
    
};/*END class Mesh */


Mesh::Mesh(const char * filename)
{
    /*
     * 1-GENERATION du MAILLAGE
     * lecture maillage
     */
    std::ifstream f(filename);
    if(!f){
        std::cerr << "\tErreur: probleme ouverture du fichier ou fichier inexistant !"<< std::endl;
        exit(1);
    }   
    f >> nbs >> nbt;
    
    /*
     * remplissage des classes
     */
    std::cout << "nbre de sommet   (nbs) = " << nbs << std::endl;
    std::cout << "nbre de triangle (nbt) = " << nbt << std::endl;
    
    Array Coorneu(nbs,2), Refneu(nbs,1);
    
    // lecture des vertex...FIXME
    vertices = new Vertex[nbs];
    triangle = new Triangle[nbt];
    std::vector<Vertex>vv;
    for(int k=0;k<nbs;k++){
        f >> vertices[k];
        vv.push_back(vertices[k]);
//         vertices[k].display();
        Coorneu.setCoef(k,0,vertices[k].p.x);Coorneu.setCoef(k,1,vertices[k].p.y);Refneu.setCoef(k,0,vertices[k].neb);
    }
    
   
    /* definitions des tableaux "a la facon MATLAB+ M2S ENSTA MEF" */
    Array Numtri(nbt,3),Reftri(nbt,1);

    int nt, iA, iB, iC, nebt;
    for(int i=0;i<nbt;i++){
        f >> nt >> iA >> iB >> iC >> nebt;
        triangle[i].set(vertices, nt, iA-1, iB-1, iC-1, nebt);
        Numtri.setCoef(i,0,iA-1); Numtri.setCoef(i,1,iB-1); Numtri.setCoef(i,2,iC-1); Reftri.setCoef(i,0,nebt-1);
        area += triangle[i].area;
    }

    /* 2-FABRICATION DES MATRICES ELEMENTAIRES */
    Array KK(nbs,nbs),MM(nbs,nbs);//matrice rigidite et masse (INIT.)
    int I,J;
    for(int l=0;l<nbt;l++){        
        //...boucle sur les domaines... 
        //calcul des matrices elentaires Kel,Mel
//         std::cout << "triangle " << l << std::endl;
        Array Kel(3,3),Mel(3,3);
        double c = triangle[l].area;
        
        
        int S1 = Numtri.getCoef(l,0);//numero du sommet 0 du triangle l eauivalent a I ou J (voir suite)
        int S2 = Numtri.getCoef(l,1);//numero du sommet 1 du triangle l
        int S3 = Numtri.getCoef(l,2);//numero du sommet 2 du triangle l
//         std::cout << l << "," << S1 << "," << S2 << "," << S3 << std::endl;
        double x1 = Coorneu.getCoef(S1,0);double y1 = Coorneu.getCoef(S1,1);
        double x2 = Coorneu.getCoef(S2,0);double y2 = Coorneu.getCoef(S2,1);
        double x3 = Coorneu.getCoef(S3,0);double y3 = Coorneu.getCoef(S3,1);
//         std::cout << ",x1=" << x1 << ",x2=" << x2 << ",x3=" << x3 << std::endl;
//         std::cout << ",y1=" << y1 << ",y2=" << y2 << ",y3=" << y3 << std::endl;
       
        
        
        
        for(int i=0;i<3;i++){
            //numero global
            I = Numtri.getCoef(l,i);
            
            for(int j=0;j<3;j++){                
                if(i == j){
                    Mel(i,i) = (1./6.0);
                }else{
                    Mel(i,j) = (1./12.0);
                }              
  
                //numero global
                J = Numtri.getCoef(l,j);
                Mel(i,j) = Mel(i,j)*c;
                /* 3-ASSEMBLAGE DES MATRICES GLOBALES */
                MM(I,J) += Mel(i,j);//Assemblage de la matrice MASSE


                Array a(3,1),b(3,1);//cf cours ENS Cachan (TD MEF M2S)
                a(0,0) = y2-y3;b(0,0) = -x2+x3;
                a(1,0) = y3-y1;b(1,0) = -x3+x1;
                a(2,0) = y1-y2;b(2,0) = -x1+x2;
                
                a = a.operator/(2*c); b = b.operator/(2*c);
                double a_dot_b = a.operator,(b);//dot product (produit scalaire entre a et b
                Kel.setCoef(i,j,a_dot_b);
                Kel(i,j) = Kel(i,j)/(4*c);
                /* 3-ASSEMBLAGE DES MATRICES GLOBALES */
                KK(I,J) += Kel(i,j);

            }
        }//fin calcul matrice MASSE: MM // RAIDEUR: KK
    }
    
    /*  Affichage des matrices globales */
    // MASSE
//     MM.display("MM");//Affichage
    // RAIDEUR
//     KK.display("KK");
    
    
        
    
    /* 4-CONSTITUTION DU 2ND MEMBRE */
    /* 5-ELIMINATION DES CONDITIONS ESSENTIELLES (s'il y en a) */
    /* 6-RESOLUTION DU SYSTEME LINEAIRE */
    
//     EXEMPLE 1
    
    Array A22(2,2),b21(2,1),c21(2,1);
    A22.setCoef(0,0,2);A22.setCoef(0,1,-1);
    A22.setCoef(1,0,-1);A22.setCoef(1,1,1);
    A22.display("A22");
    
    b21.setCoef(0,0,1);b21.setCoef(1,0,0);
    b21.display("b21");
    
//     std::cout << "normeL2(b21) = " << b21.normeL2() << std::endl;
    Array A33(3,3),b31(3,1);
    A33.setCoef(0,0,4);A33.setCoef(0,1,2);A33.setCoef(0,2,1);
    A33.setCoef(1,0,-1);A33.setCoef(1,1,2);A33.setCoef(1,2,0);
    A33.setCoef(2,0,2);A33.setCoef(2,1,1);A33.setCoef(2,2,4);
    A33.display("A33");
    
    b31.setCoef(0,0,4);b31.setCoef(1,0,2);b31.setCoef(2,0,9);
    b31.display("b31");
    
//     c21 = A22*b21;
//     c21.display("c21");
    std::cout << "\tResolution systeme lineaire: Ax = b par la methode projective: GC" << std::endl;
    solve::projective::GC(A33,b31);
    std::cout << "\n";
    
    
    

    std::cout << "\tResolution systeme lineaire: Ax = b par la methode iterative: JACOBI" << std::endl;
    solve::iterative::JACOBI(A22,b21);
    std::cout << "\n";
    
    
//     EXEMPLE 2
//     Array A23(2,3),b34(3,4),c24(2,4);
//     
//     A23.setCoef(0,0,1);A23.setCoef(0,1,-1);A23.setCoef(0,2,3);
//     A23.setCoef(1,0,3);A23.setCoef(1,1,0);A23.setCoef(1,2,-2);
//     
//     b34.setCoef(0,0,3);b34.setCoef(0,1,1);b34.setCoef(0,2,0);b34.setCoef(0,3,-2);
//     b34.setCoef(1,0,-1);b34.setCoef(1,1,0);b34.setCoef(1,2,3);b34.setCoef(0,3,-1);
//     b34.setCoef(2,0,0);b34.setCoef(2,1,-4);b34.setCoef(2,2,0);b34.setCoef(0,3,-1);
//     
//     c24 = A23*b34;
    
    
    
    

    /* 7-POST-TRAITEMENTS NUMERIQUES et/ou GRAPHIQUES */
    std::cout << "\n\n\t--- Traitements numeriques: OK ---" << std::endl;


} //END class Mesh





#endif /*__MESH_H__*/
