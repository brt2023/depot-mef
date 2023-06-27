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

typedef double dble;






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
        vertices[k].display();
        Coorneu.setCoef(k,0,vertices[k].p.x);Coorneu.setCoef(k,1,vertices[k].p.y);Refneu.setCoef(k,0,vertices[k].neb);
    }
    
    std::cout << "\n-------------------------------------------------------------------\n";
    
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
        std::cout << "triangle " << l << std::endl;
        Array Kel(3,3),Mel(3,3);
        double c = triangle[l].area;     
        
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

                MM(I,J) += Mel(i,j);//Assemblage de la matrice MASSE
            }
        }
    }
    MM.display("MM");
    
    
        
    /* 3-ASSEMBLAGE DES MATRICES GLOBALES */
    /* 4-CONSTITUTION DU 2ND MEMBRE */
    /* 5-ELIMINATION DES CONDITIONS ESSENTIELLES (s'il y en a) */
    /* 6-RESOLUTION DU SYSTEME LINEAIRE */
    /* 7-POST-TRAITEMENTS NUMERIQUES et/ou GRAPHIQUES */


} //END class Mesh





#endif /*__MESH_H__*/
